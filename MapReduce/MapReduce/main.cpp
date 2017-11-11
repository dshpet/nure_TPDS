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
using DataSource   = std::vector<std::string>;

MapResult MapWords(
    DataSource::const_iterator _DataBegin, // should get a chunk of global data
    DataSource::const_iterator _DataEnd
  )
{
  std::map<std::string, size_t> Intermediate;
  
  for (auto & it = _DataBegin; it != _DataEnd; ++it)
    Intermediate[*it]++;

  return Intermediate;
}

ReduceResult ReduceWords(
    const std::vector<MapResult> & _MapResults
  )
{
  ReduceResult Result;

  for (const auto & mapResult : _MapResults)
    for (const auto pair : mapResult)
      Result[pair.first] += pair.second;

  return Result;
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

  auto result = MapReduce(Data, MapWords, ReduceWords);

  std::cout << "Word count results:\n";
  for (const auto & pair : result)
    std::cout << "  " << std::setw(10) << pair.first << " -> " << pair.second << "\n";

  int WAIT;
  std::cin >> WAIT;
}