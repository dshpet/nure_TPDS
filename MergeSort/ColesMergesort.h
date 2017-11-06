#pragma once

#include <thread>
#include <vector>
#include <algorithm>

template <class T>
size_t FindPosition(
    T _begin, 
    T _end, 
    T _val
  )
{
  T lowerBound = _begin;
  T upperBound = _end;

  while (true)
  {
    T index = lowerBound + std::distance(lowerBound, upperBound) / 2;

    if (*_val == *index)
      return std::distance(_begin, index);

    if (*_val < *index)
    {
      upperBound = index - 1;
      if (lowerBound > upperBound)
        return std::distance(_begin, index);
    }
    else
    if (*_val > *index)
    {
      lowerBound = index + 1;
      if (lowerBound > upperBound)
        return std::distance(_begin, index + 1);
    }
  }

  return -1;
}

// https://stanford.edu/~rezab/dao/notes/lecture03/cme323_lec3.pdf
template <class T>
std::vector<typename T::value_type> ParallelMerge(
    T _begin,
    T _median,
    T _end
  )
{
  size_t sizeA = std::distance(_begin, _median);
  size_t sizeB = std::distance(_median, _end);
  // required by algorithm
  bool IsSizeEqual = sizeA == sizeB;

  std::vector<typename T::value_type> result(sizeA + sizeB);
  
  std::thread left([&]() {
    for (size_t i = 0; i < sizeA; i++)
    {
      T a = _begin + i;
      int rankA = i;
      int rankB = std::min(FindPosition<T>(_median, _end, a), sizeB);
      int rankM = rankA + rankB;

      result[rankM] = *a;

      // printf("value %d | rankA %d | rankB %d | rankM %d \n", *a, rankA, rankB, rankM);
    }
  });

  std::thread right([&]() {
    for (size_t i = sizeA; i < sizeA + sizeB; i++)
    {
      T b = _begin + i;
      int rankA = std::min(FindPosition<T>(_begin, _median, b), sizeA);
      int rankB = i - sizeB;
      int rankM = rankA + rankB;

      result[rankM] = *b;

      // printf("value %d | rankA %d | rankB %d | rankM %d \n", *b, rankA, rankB, rankM);
    }
  });

  left.join();
  right.join();

  return result;
}

template <class T>
std::vector<typename T::value_type> ColeMergeSort(
    T _begin,
    T _end
  )
{
  auto size = std::distance(_begin, _end);
  T median = _begin + size / 2;
  
  // 2 threads for now
  std::thread left([&]() { std::sort(_begin, median); });
  std::thread right([&]() { std::sort(median, _end); });
  
  left.join();
  right.join();

  return ParallelMerge<T>(_begin, median, _end);
}