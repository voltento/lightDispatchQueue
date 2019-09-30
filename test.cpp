#include <iostream>
#include <atomic>

#include "LightDispatcher.h"



int main() {
  std::atomic_int counter{0};
  lightDispatcher::LightDispatcher d;
  d.start();
  d.dispatch([&counter]() {
    ++counter;
  });
  std::this_thread::sleep_for(std::chrono::seconds(1));
  d.stop();
  assert(counter == 1);
  return 0;
}
