#include <string>
#include <iostream>
#include <irrKlang.h>

using namespace std;
using namespace irrklang;

int main()
{
    const string musicName = "musics/loser_baby.wav";
    cout << "Test compiling and running !" << endl;

    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine) {
        cerr << "Could not start sound engine" << endl;
        return 1;
    }

    engine->play2D(musicName.c_str(), true);

    cout << "Press Enter to exit..." << endl;
    cin.get();

    engine->drop();
    return 0;
}

//compiller build
//++ -o playmp3 main.cpp -I/usr/local/lib/irrKlang/include -L/usr/local/lib -lIrrKlang -pthread
