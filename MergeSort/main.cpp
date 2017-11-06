//
// Merge sort all in one
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "StandardParallelMergeSort.h"
#include "ColesMergesort.h"

void main()
{
  std::vector<int> testData1{ 1,2,6,7,3,4,8,9 };
  std::vector<int> testData2{ 1,2,6,7,3,4,8,9 };

  constexpr size_t testSize = 1024;
  std::vector<int> randomTestData1(testSize);
  std::vector<int> randomTestData2(testSize);
  for (auto i = 0; i < testSize; i++)
  {
    randomTestData1[i] = i;
    randomTestData2[i] = i;
  }
  std::mt19937 rng(std::random_device{}());
  std::shuffle(randomTestData1.begin(), randomTestData1.end(), rng);
  std::shuffle(randomTestData2.begin(), randomTestData2.end(), rng);

  MergeSort(testData1.begin(), testData1.end());
  auto res = ColeMergeSort(testData2.begin(), testData2.end());

  // merge sort naive
  {
    auto t1 = std::chrono::high_resolution_clock::now();
    MergeSort(randomTestData1.begin(), randomTestData1.end());
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << duration << std::endl;
  }
  // amazing superoptimal Cole's merge sort
  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto res2 = ColeMergeSort(randomTestData2.begin(), randomTestData2.end());
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << duration << std::endl;
  }

  getchar();
}