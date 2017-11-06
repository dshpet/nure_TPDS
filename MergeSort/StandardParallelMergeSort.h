#pragma once

#include <thread>
#include <vector>
#include <algorithm>

template <class T>
void MergeSortPart(
    T _begin,
    T _end
  )
{
  // lazy stuff
  std::sort(_begin, _end);
}

template <class T>
void Merge(
    T _begin,
    T _median,
    T _end
  )
{
  // inefficient copy
  std::vector<typename T::value_type> left(_begin, _median);
  std::vector<typename T::value_type> right(_median, _end);

  // lazy hack
  left .push_back(std::numeric_limits<typename T::value_type>::max());
  right.push_back(std::numeric_limits<typename T::value_type>::max());

  auto itl = std::begin(left);
  auto itr = std::begin(right);

  while (_begin != _end) 
  {
    if (*itl <= *itr)
      *_begin++ = *itl++; // push left
    else
      *_begin++ = *itr++; // push right
  }
}

template <class T>
void MergeSort(
    T _begin,
    T _end
  )
{
  auto size = std::distance(_begin, _end);
  T median = _begin + size / 2;
  
  // 2 threads for now
  std::thread left (MergeSortPart<T>, _begin, median);
  std::thread right(MergeSortPart<T>, median, _end);
  
  left.join();
  right.join();

  Merge<T>(_begin, median, _end);
}