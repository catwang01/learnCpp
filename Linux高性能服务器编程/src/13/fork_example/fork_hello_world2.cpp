// fork_hello_world2.cpp
#include <stdio.h>
#include <unistd.h>
int main() {
  // 建立子行程
  fork(); // 1 -> 2

  fork(); // each child processes will produce another two new processes.
          // 2 -> 4
  printf("Hello world!\n");
  return 0;
}