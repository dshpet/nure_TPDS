#pragma once

#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <algorithm>
#include <mutex>

//
// Helpers
//

class ThreadGroup :
  public std::vector<std::thread>
{
public:
  void JoinAll()
  {
    for (auto && t : *this)
      t.join();
  }
};

template<class F>
struct return_type_of;

// function pointer
template<class R, class... Args>
struct return_type_of<R(*)(Args...)> : public return_type_of<R(Args...)>
{};

template<class R, class... Args>
struct return_type_of<R(&)(Args...)> : public return_type_of<R(Args...)>
{};

template<class R, class... Args>
struct return_type_of<R(Args...)>
{
  using type = R;
};

//
// Algoritm
//

// Not very generic. Main purpose to separate algorithm and actual functions (and not to pass std::function)
template <typename DataSource, typename Mapper, typename Reducer>
typename return_type_of<Reducer>::type MapReduce(
    const DataSource & _Data, // should also be template
    Mapper  &&         _MapFunction,
    Reducer &&         _ReduceFunction
  )
{
  using MapResult = typename return_type_of<Mapper>::type;
  std::vector<MapResult> results;

  ThreadGroup threads;
  std::mutex mutex;

  auto size = std::distance(_Data.begin(), _Data.end());
  auto threadsCount = std::thread::hardware_concurrency();
  size_t chunkSize = size / threadsCount;

  // Map
  auto chunkBegin = _Data.begin();
  while (chunkBegin != _Data.end())
  {
          auto   chunkEnd         = chunkBegin;
    const size_t remainingData    = std::distance(chunkBegin, _Data.end());
    const size_t currentChunkSize = std::min(chunkSize, remainingData);

    std::advance(chunkEnd, currentChunkSize);

    threads.emplace_back(
        std::thread(
            [=, &results, &mutex]()
            {
              mutex.lock();
              results.emplace_back(_MapFunction(chunkBegin, chunkEnd));
              mutex.unlock();
            }
          )
      );

    std::advance(chunkBegin, currentChunkSize);
  }

  threads.JoinAll();

  // Sort
  std::sort(results.begin(), results.end());

  // Reduce
  return _ReduceFunction(results);
}