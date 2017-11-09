// Manual MapReduce implementation
// counts words in files. BERRRYYY NIIICE

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

#include "MapReduce.h"

std::vector<std::string> MapWords()
{
  return std::vector<std::string>();
}

std::vector<std::string> ReduceWords()
{
  return std::vector<std::string>();
}

void main()
{
  const std::vector<std::string> filenames = { "a.txt", "b.txt", "c.txt" };
  const std::string              folder    = "../Data/";

  // key - filename, value - file content
  std::map<std::string, std::string> Data;

  for (auto && filename : filenames)
  {
    const auto && path = folder + filename;

    std::ifstream fileStream(path);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    Data[filename] = buffer.str();
  }

  auto result = MapReduce(Data, MapWords, ReduceWords);
}