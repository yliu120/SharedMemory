#include <zmq.h>
#include <cstring>
#include <iostream>
#include <thread>

const int NUM_PRODUCERS = 3;
const int NUM_CONSUMERS = 1;

void producer(int id) {
  void* context = zmq_ctx_new();
  void* socket = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(socket, "tcp://localhost:5555");

  for (int i = 0; i < 5; ++i) {
    std::string msg =
        "Producer " + std::to_string(id) + " message " + std::to_string(i);
    int num_sent = zmq_send(socket, msg.data(), msg.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  zmq_send(socket, "", 0, 0);
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void consumer(int id) {
  void* context = zmq_ctx_new();
  void* socket = zmq_socket(context, ZMQ_PULL);
  zmq_bind(socket, "tcp://*:5555");

  while (true) {
    zmq_msg_t message;
    zmq_msg_init(&message);
    zmq_msg_recv(&message, socket, 0);
    std::string msg = std::string(static_cast<char*>(zmq_msg_data(&message)),
                                  zmq_msg_size(&message));
    std::cout << "Consumer " << id << " received: " << msg << std::endl;
    if (msg.empty()) {
      break;
    }
    zmq_msg_close(&message);
  }

  zmq_close(socket);
  zmq_ctx_destroy(context);
}

int main() {
  std::thread producers[NUM_PRODUCERS];
  std::thread consumers[NUM_CONSUMERS];

  for (int i = 0; i < NUM_PRODUCERS; ++i) {
    producers[i] = std::thread(producer, i);
  }

  for (int i = 0; i < NUM_CONSUMERS; ++i) {
    consumers[i] = std::thread(consumer, i);
  }

  for (int i = 0; i < NUM_PRODUCERS; ++i) {
    producers[i].join();
  }

  for (int i = 0; i < NUM_CONSUMERS; ++i) {
    consumers[i].join();
  }

  return 0;
}