#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//SDL
#include <string>

#include "Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "Rosalila/RosalilaSound/RosalilaSound.h"
#include "STGUtility/STGUtility.h"
#include "STGMenu/STGMenu.h"
#include "STGMenu/ControllerConfig.h"
#include "STGMenu/Intro.h"
#include "STGMenu/StageSelect.h"
#include "Rosalila/RosalilaUtility/RosalilaUtility.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    clearLog();

    setReceiver(new Receiver());
    setRosalilaGraphics(new RosalilaGraphics());

    Sound*sonido = new Sound();
    string path_menu = assets_directory+"menu/main_menu.svg";
    map<string,Button*> controls = ControllerConfig();
    intro();
    stageSelect();

    Menu* menu=new Menu(getRosalilaGraphics(),getReceiver(),sonido,(char*)path_menu.c_str(),controls);
    menu->playMusic();

    menu->loopMenu();
    return 0;
}
