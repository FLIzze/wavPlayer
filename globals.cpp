#include "globals.h"
#include <vector>
#include <map>
#include <filesystem>

std::vector<std::filesystem::path> filePaths;
int songIndex = 0;
bool isPause = true;
std::vector<std::map<std::string, std::string>> metaDatas;

