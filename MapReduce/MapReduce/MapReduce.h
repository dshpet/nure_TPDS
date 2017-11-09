#pragma once

#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <algorithm>

//
// Helpers
//

class ThreadGroup :
  public std::vector<std::thread>
{
public:
  ThreadGroup() = default;
  ~ThreadGroup() { JoinAll(); };

  void JoinAll()
  {
    for (auto && t : *this)
      t.join();
  }
};

//
// Algoritm
//

// Not very generic. Main purpose to separate algorithm and actual functions (and not to pass std::function)
template <typename Mapper, typename Reducer>
std::vector<std::pair<std::string, size_t>> MapReduce(
    std::map<std::string, std::string> _Data, // should also be template
    Mapper  &&                         _MapFunction,
    Reducer &&                         _ReduceFunction
  )
{
  std::vector<std::pair<std::string, size_t>> temp;

  // TODO make parallel blocks for each stage
  auto threadsCount = std::thread::hardware_concurrency();
  auto size = std::distance(_Data.begin(), _Data.end());

  // Map

  // todo make blocks and threads
  std::vector<std::pair<std::string, size_t>> MapperResults = _MapFunction(_Data.begin(), _Data.end());
  // todo write to file or check vector for concurent usage
  temp.push_back(MapperResults);

  // Sort
  std::sort(temp.begin(), temp.end());

  // Reduce
  

  return temp;
}