
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/unsynchronizedlyricsframe.h>
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

        if (tag)
        {
            metadata["title"] = tag->title().toCString(true);
            metadata["artist"] = tag->artist().toCString(true);
            metadata["album"] = tag->album().toCString(true);
            metadata["comment"] = tag->comment().toCString(true);
            metadata["genre"] = tag->genre().toCString(true);
            metadata["date"] = tag->year() > 0 ? std::to_string(tag->year()) : "";
            metadata["track"] = std::to_string(tag->track());

            // Extract lyrics if available
            TagLib::ID3v2::FrameList frames = tag->frameListMap()["USLT"];
            if (!frames.isEmpty())
            {
                TagLib::ID3v2::UnsynchronizedLyricsFrame *lyricsFrame = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frames.front());
                if (lyricsFrame)
                {
                    metadata["lyrics"] = lyricsFrame->text().toCString(true);
                }
            }
            else 
            {
              metadata["lyrics"] = "No lyrics for this song.";
            }
        }
        else
        {
            std::cout << "No ID3 tag found for: " << filePaths[i] << std::endl;
        }

        metaDatas.push_back(metadata);
    }
}
