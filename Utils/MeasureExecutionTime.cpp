/*
  Class found on stackoverflow
  https://stackoverflow.com/a/61886741
*/

#include <chrono>
#include <iostream>

class MeasureExecutionTime
{
  private:
    const std::chrono::steady_clock::time_point begin;
    const std::string caller;

  public:
    MeasureExecutionTime(const std::string &caller) : caller(caller), begin(std::chrono::steady_clock::now())
    {
    }
    ~MeasureExecutionTime()
    {
        const auto duration = std::chrono::steady_clock::now() - begin;
        std::cout << "ExecutionTime For " << caller << " is "
                  << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << "μs\n";
    }
};

#ifndef MEASURE_FUNCTION_EXECUTION_TIME
#define MEASURE_FUNCTION_EXECUTION_TIME const MeasureExecutionTime measureExecutionTime(__FUNCTION__);
#endif