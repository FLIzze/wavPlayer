#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <vector>
#include <map>
#include <filesystem>

using namespace std;

extern vector<filesystem::path> filePaths;
extern int songIndex;
extern vector<map<string, string>> metaDatas;
extern bool isPause;

#endif 
