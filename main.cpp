#include <iostream>
#include <vector>

#include "sound.h"
#include "gui.h"
#include <filesystem>
#include "metadata.h"
#include "globals.h"

using namespace std;
namespace fs = std::filesystem;

int main() 
{
  try 
  {
    for (const auto& entry : fs::directory_iterator("./musics/")) 
    {
      filePaths.push_back(entry.path());
    }
  }
  catch (const std::exception&) 
  {
    return 1;
  }

  if (filePaths.empty())
  {
    cerr << "No musics files found.";
    return 1;
  } 

  if (!playMusic()) 
  {
    cerr << "Failed to play music" << endl;
    return 1;
  }

  getMetaData();

  displayWindow();

  runFLTK();

  return 0;
}

