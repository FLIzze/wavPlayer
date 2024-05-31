#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "globals.h"

using namespace std;

void getMetaData() 
{
  for (int i = 0; i < filePaths.size(); i++)
  {
    map<string, string> metadata;

    TagLib::MPEG::File file(filePaths[i].c_str()); 
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

    if(tag) {
      metadata["title"] = tag->title().toCString(true);
      metadata["artist"] = tag->artist().toCString(true);
    } else {
      std::cout << "No ID3 tag found for: " << filePaths[i] << std::endl;
    }
    metaDatas.push_back(metadata);
  }
}
