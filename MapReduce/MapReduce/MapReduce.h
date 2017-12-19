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

template<typename Container>
class ThreadSafeContainer
{
public:
  ThreadSafeContainer(Container * _Container) : m_Container(_Container), m_Lock() {};

  void Set(const typename Container::key_type & _Key, const typename Container::mapped_type & _Value)
  {
    m_Lock.lock();
    (*m_Container.get())[_Key] = _Value;
    m_Lock.unlock();
  }

  typename Container::mapped_type Get(const typename Container::key_type & _Key) const
  {
    return (*m_Container.get())[_Key];
  }

  const Container & Raw()
  {
    return (*m_Container.get());
  }

private:
  std::mutex                 m_Lock;
  std::shared_ptr<Container> m_Container;
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

template <typename DataSource, typename Mapper, typename Reducer, typename Accumulator>
Accumulator & MapReduce(
    const DataSource & _Data,
    Mapper  &&         _MapFunction,
    Reducer &&         _ReduceFunction,
    Accumulator &      _Accumulator
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
  threads.clear();

  for (auto mapResult : results)
  {
    threads.emplace_back(
        std::thread(
            [=, &_Accumulator]()
            {
              _ReduceFunction(_Accumulator, mapResult);
            }
          )
      );
  }

  threads.JoinAll();

  return _Accumulator;
}