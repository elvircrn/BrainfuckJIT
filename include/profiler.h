#ifndef BRAINFUCKJIT_PROFILER_H
#define BRAINFUCKJIT_PROFILER_H

#include <chrono>
#include <iostream>

#define BEGIN_PROFILER auto t1 = std::chrono::high_resolution_clock::now();
#define END_PROFILER auto t2 = std::chrono::high_resolution_clock::now(); auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count(); std::cout << '\n' << (duration / 1000000.0) << " s" << std::endl;

#endif //BRAINFUCKJIT_PROFILER_H
