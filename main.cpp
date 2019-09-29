#include <iostream>

#include "LightDispatcher.h"

int main() {
  lightDispatcher::LightDispatcher d;
  d.start();
  d.dispatch([]() {
    std::cout << "Hello, World!" << std::endl;
  });
  std::this_thread::sleep_for(std::chrono::seconds(1));
  d.stop();
  return 0;
}
