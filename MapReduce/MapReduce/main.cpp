// Manual MapReduce implementation
// counts words in files. BERRRYYY NIIICE

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "MapReduce.h"

using MapResult    = std::map<std::string, size_t>;
using ReduceResult = std::map<std::string, size_t>;
using Accumulator  = ThreadSafeContainer<ReduceResult>;
using DataSource   = std::vector<std::string>;

MapResult MapWords(
    DataSource::const_iterator _DataBegin,
    DataSource::const_iterator _DataEnd
  )
{
  std::map<std::string, size_t> Intermediate;
  
  for (auto & it = _DataBegin; it != _DataEnd; ++it)
    Intermediate[*it]++;

  return Intermediate;
}

void ReduceWords(
    Accumulator     & Accumulator,
    const MapResult & _MapResult
  )
{
  for (const auto pair : _MapResult)
    Accumulator.Set(pair.first, Accumulator.Get(pair.first) + pair.second);
}

void main()
{
  const std::vector<std::string> filenames = { "a.txt", "b.txt", "c.txt" };
  const std::string              folder    = "../Data/";

  DataSource Data;

  for (auto && filename : filenames)
  {
    const auto && path = folder + filename;

    std::ifstream fileStream(path);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    std::string word;
    while (buffer >> word)
    {
      std::string::iterator lastchars = word.end() - (word.size() > 1 ? 1 : 0);
      word.erase(
        std::remove_if(
          lastchars,
          word.end(),
          [](const char & c) -> bool
          { 
            const std::string stupidchars = ",.;!?";
            return std::find(stupidchars.cbegin(), stupidchars.cend(), c) != stupidchars.end();
          }
        ),
        word.end()
      );

      Data.push_back(word);
    }
  }

  ReduceResult reduseResult;
  Accumulator result(&reduseResult);
  MapReduce(Data, MapWords, ReduceWords, result);

  std::cout << "Word count results:\n";
  for (const auto & pair : result.Raw())
    std::cout << "  " << std::setw(10) << pair.first << " -> " << pair.second << "\n";

  int WAIT;
  std::cin >> WAIT;
}