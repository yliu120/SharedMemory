# Provides minimal compilation support for zeromq.

genrule(
    name = "platform_hpp",
    outs = ["platform.hpp"],
    cmd = select({
        "@bazel_tools//src/conditions:darwin": """cat > $@ <<"EOF"
#ifndef __ZMQ_PLATFORM_HPP_INCLUDED__
#define __ZMQ_PLATFORM_HPP_INCLUDED__

#define ZMQ_BUILD_DRAFT_API
#define ZMQ_HAVE_ATOMIC_INTRINSICS 1
#define ZMQ_HAVE_EVENTFD_CLOEXEC 1
#define ZMQ_HAVE_IFADDRS 1
#define ZMQ_HAVE_SO_KEEPALIVE 1
#define ZMQ_HAVE_TCP_KEEPCNT 1
#define ZMQ_HAVE_TCP_KEEPINTVL 1
#define ZMQ_HAVE_UIO 1
#define ZMQ_USE_EPOLL 0
#define ZMQ_USE_EPOLL_CLOEXEC 1
#define ZMQ_POLL_BASED_ON_POLL 1
#define ZMQ_USE_TWEETNACL 0
#define ZMQ_IOTHREAD_POLLER_USE_KQUEUE 1

#ifdef _AIX
  #define ZMQ_HAVE_AIX
#endif

#if defined ANDROID
  #define ZMQ_HAVE_ANDROID
#endif

#if defined __CYGWIN__
  #define ZMQ_HAVE_CYGWIN
#endif

#if defined __MINGW32__
  #define ZMQ_HAVE_MINGW32
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
  #define ZMQ_HAVE_FREEBSD
#endif

#if defined __hpux
  #define ZMQ_HAVE_HPUX
#endif

#if defined __linux__
  #define ZMQ_HAVE_LINUX
#endif

#if defined __NetBSD__
  #define ZMQ_HAVE_NETBSD
#endif

#if defined __OpenBSD__
  #define ZMQ_HAVE_OPENBSD
#endif

#if defined __VMS
  #define ZMQ_HAVE_OPENVMS
#endif

#if defined __APPLE__
  #define ZMQ_HAVE_OSX
#endif

#if defined __QNXNTO__
  #define ZMQ_HAVE_QNXNTO
#endif

#if defined(sun) || defined(__sun)
  #define ZMQ_HAVE_SOLARIS
#endif

/* #undef ZMQ_HAVE_WINDOWS */

#endif
EOF""",
        "//conditions:default": """cat > $@ <<"EOF"
#ifndef __ZMQ_PLATFORM_HPP_INCLUDED__
#define __ZMQ_PLATFORM_HPP_INCLUDED__

#define ZMQ_BUILD_DRAFT_API
#define ZMQ_HAVE_ATOMIC_INTRINSICS 1
#define ZMQ_HAVE_EVENTFD 1
#define ZMQ_HAVE_EVENTFD_CLOEXEC 1
#define ZMQ_HAVE_IFADDRS 1
#define ZMQ_HAVE_SOCK_CLOEXEC 1
#define ZMQ_HAVE_SO_KEEPALIVE 1
#define ZMQ_HAVE_SO_PEERCRED 1
#define ZMQ_HAVE_TCP_KEEPCNT 1
#define ZMQ_HAVE_TCP_KEEPIDLE 1
#define ZMQ_HAVE_TCP_KEEPINTVL 1
#define ZMQ_HAVE_UIO 1
#define ZMQ_USE_EPOLL 1
#define ZMQ_POLL_BASED_ON_POLL 1
#define ZMQ_IOTHREAD_POLLER_USE_EPOLL 1
#define ZMQ_USE_EPOLL_CLOEXEC 1
#define HAVE_STRNLEN 1
#define ZMQ_USE_CV_IMPL_STL11 1
#define ZMQ_USE_BUILTIN_SHA1 1

#ifdef _AIX
  #define ZMQ_HAVE_AIX
#endif

#if defined ANDROID
  #define ZMQ_HAVE_ANDROID
#endif

#if defined __CYGWIN__
  #define ZMQ_HAVE_CYGWIN
#endif

#if defined __MINGW32__
  #define ZMQ_HAVE_MINGW32
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
  #define ZMQ_HAVE_FREEBSD
#endif

#if defined __hpux
  #define ZMQ_HAVE_HPUX
#endif

#if defined __linux__
  #define ZMQ_HAVE_LINUX
#endif

#if defined __NetBSD__
  #define ZMQ_HAVE_NETBSD
#endif

#if defined __OpenBSD__
  #define ZMQ_HAVE_OPENBSD
#endif

#if defined __VMS
  #define ZMQ_HAVE_OPENVMS
#endif

#if defined __APPLE__
  #define ZMQ_HAVE_OSX
#endif

#if defined __QNXNTO__
  #define ZMQ_HAVE_QNXNTO
#endif

#if defined(sun) || defined(__sun)
  #define ZMQ_HAVE_SOLARIS
#endif

/* #undef ZMQ_HAVE_WINDOWS */

#endif
EOF""",
    }),
)

cc_library(
    name = "libzmq",
    srcs = glob([
        "src/*.cpp",
        "src/*.hpp",
        "external/sha1/*.h",
        "external/sha1/*.c",
    ],
    exclude = [
        "src/ws_*.cpp",
        "src/wss_engine.hpp",
        "src/wss_engine.cpp",
    ],
    ),
    hdrs = [
        "include/zmq.h",
        "include/zmq_utils.h",
        "src/zmq_draft.h",
        "platform.hpp",
    ],
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = [
        "//visibility:public",
    ],
)