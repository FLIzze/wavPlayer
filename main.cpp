#include <iostream>
#include "sound.h"
#include "gui.h"
#include "globals.h"
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string filePath = "./musics/";
vector<fs::path> filePaths;
int songIndex = 0;

int main() 
{
  try 
  {
    for (const auto& entry : fs::directory_iterator(filePath)) 
    {
      filePaths.push_back(entry.path());
      std::cout << entry.path();
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

  if (!initSoundEngine()) 
  {
    cerr << "Failed to initialize sound engine" << endl;
    return 1;
  }

  if (!playMusic()) 
  {
    cerr << "Failed to play music" << endl;
    cleanupSoundEngine();
    return 1;
  }

  displayWindow();

  runFLTK();

  cleanupSoundEngine();

  return 0;
}

