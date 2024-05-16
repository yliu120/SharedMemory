#include <cassert>
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <random>
#include <string_view>

#include <benchmark/benchmark.h>
#include <zmq.h>

#include "agents.h"
#include "threadpool.h"

namespace prototype {

constexpr int kNumThreads = 32;
constexpr int kNumProducers = 2;
constexpr int kNumConsumers = 1;
constexpr int kNumSignals = 32;
constexpr std::string_view kAddress = "tcp://localhost:5666";

struct ProducerContext {
  void* producer = nullptr;
  void* producer_context = nullptr;
};

std::unique_ptr<Signal> RandomSignalProducer(int index) {
  auto signal = std::make_unique<Signal>();
  signal->index = index;

  std::mt19937 generator;
  std::uniform_int_distribution<int> dist(0, 255);
  for (int i = 0; i < kImageSize; ++i) {
    signal->signals[i] = static_cast<char>(dist(generator));
  }
  return signal;
}

static void BM_MessagePassing(benchmark::State& state) {
  std::unique_ptr<Processor> processor = MakeDefaultProcessor();

  std::vector<ProducerContext> producer_contexts;
  for (auto _ : state) {
    state.PauseTiming();
    ThreadPool pool(kNumThreads * 2);

    // Synchronization mechanism that allows the first thread to bind and others
    // to connect.
    bool is_bound = false;
    std::mutex mu;
    std::condition_variable init_done;

    state.ResumeTiming();
    // Registers producers.
    for (int i = 0; i < kNumProducers; i++) {
      pool.EnqueueWork([i, &producer_contexts] {
        void* producer_context = zmq_ctx_new();
        void* producer = zmq_socket(producer_context, ZMQ_PUSH);
        int producer_comm = -1;

        producer_comm = zmq_connect(producer, kAddress.data());

        for (int j = 0; j < kNumSignals / kNumProducers; j++) {
          auto signal = RandomSignalProducer(i * 10000 + j);
          std::string serialized = signal->SerializeToString();
          // Blocking mode.
          int num_sent =
              zmq_send(producer, serialized.data(), serialized.size(), 0);
          assert(num_sent == sizeof(Signal));
          (void)num_sent;
        }

        zmq_send(producer, "", 0, 0);
        ProducerContext context = {
            .producer = producer,
            .producer_context = producer_context,
        };
        producer_contexts.push_back(std::move(context));
      });
    }

    // Registers consumers.
    for (int i = 0; i < kNumConsumers; i++) {
      pool.EnqueueWork([p = processor.get(), &mu, &init_done, &is_bound, i] {
        void* consumer_context = zmq_ctx_new();
        void* consumer = zmq_socket(consumer_context, ZMQ_PULL);

        int consumer_comm = zmq_bind(consumer, kAddress.data());
        assert(consumer_comm == 0);
        (void)consumer_comm;

        for (int j = 0; j < kNumSignals / kNumConsumers; j++) {
          std::string buffer(sizeof(Signal), '\0');
          // Blocking mode.

          int size = zmq_recv(consumer, buffer.data(), buffer.size(), 0);
          if (size == 0) {
            break;
          }
          // Process and consume.
          auto deserialized = Signal::Deserialize(buffer);
          p->InPlaceProcess(&deserialized);
        }

        zmq_close(consumer);
        zmq_ctx_destroy(consumer_context);
      });
    }
    pool.Quiesce();
  }
  // for (auto& context : producer_contexts) {
  //   zmq_close(context.producer);
  //   zmq_ctx_destroy(context.producer_context);
  // }
}

// Register the function as a benchmark
BENCHMARK(BM_MessagePassing);

}  // namespace prototype

BENCHMARK_MAIN();
