#pragma once

#include <thread>
#include <vector>

#include "blockingconcurrentqueue.h"

namespace lightDispatcher {

using Functor = std::function<void()>;

class LightDispatcher {
public:
  explicit LightDispatcher(size_t numThreads = 1)
      : m_numThreads(numThreads), m_running(false) {}

  template <typename T> void dispatch(T &&f) {
    m_queue.enqueue(std::forward<T>(f));
  }

  void stop() {
    m_running.store(false);
    // add handlers for initiating stop check in threads
    for (size_t i = 0; i < numThreads(); ++i) {
      m_queue.enqueue(Functor{});
    }
    for (auto &t : m_threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  void start() {
    if (!m_running.exchange(true)) {
      for (size_t i = 0; i < numThreads(); ++i) {
        m_threads.emplace_back(std::bind(&LightDispatcher::handleQueue, this));
      }
    }
  }

  size_t numThreads() const { return m_numThreads; }

private:
  void handleQueue() {
    while (m_running) {
      Functor f;
      m_queue.wait_dequeue(f);
      callFunctor(f);
    }
  }

  static void callFunctor(const Functor &functor) {
    if (functor) {
      functor();
    }
  }

  moodycamel::BlockingConcurrentQueue<Functor> m_queue;
  std::vector<std::thread> m_threads;
  std::atomic_bool m_running;
  const size_t m_numThreads;
};

} // namespace lightDispatcher
