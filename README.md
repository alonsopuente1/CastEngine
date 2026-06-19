# Wolf3D-style game engine

## Prerequisites

Makefile must be installed to compile library.
compiled with g++ however can be changed in the makefile itself

Linux has additional prerequisites. see Linux Build Instructions

## Build instructions
There is support for linux and for windows. There will not be a macOS port. Compiler this uses is g++. Compiling with other compilers may not work because of __FUNCTION__ macro in the logger.h file in the include folder. If compiling with another compiler be wary of this. For linux you must have some packages installed yourself. More details for linux below.


### Windows
cd into the root directory of repo then run
```
make -j
```

### Linux
you must have the following packages installed
SDL2
SDL2_image
SDL2_ttf
SDL2_mixer

once you have these installed you can compile from scratch

simply run
'''
make -j
'''

## Usage

once the library is compiled, your game must be compiled with SDL2 as well. 
any scenes you define must inherit the IScene interface. example of scene class being declared

ExampleScene.hpp
'''C++
#include "castengine/scene.hpp"

namespace CastEngine
{
    class Texture;
}

class Example : public CastEngine::IScene
{

public:

    using CastEngine::Scene::Scene;

    void OnEnter() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void OnExit() override;

    void OnPause() override;
    void OnResume() override;

private:

    Texture* mExampleTex = nullptr;

};
'''

after your scene has been declared and implemented, it can be used in the game with the Game class' method ChangeScene. before the game's Run method is called, the Game object must be instantiated and have been changed to a scene. example below

main.cpp
'''C++

#include "castengine/game.hpp"

#include "ExampleScene.hpp"

int main(int argc, char** argv)
{
    CastEngine::Game game;

    // scene has been created and pushed into the game's internal
    // scene stack. 
    game.ChangeScene<ExampleScene>();

    // must be called after a valid call to change scene,
    // will return immediately if there is no scene
    game.Run();

    return 0;
}

'''

if you want a scene that has entities and maps to interact with, your scene should also implement the IWorld interface. this interface is so that the entities can interact with a map without having direct access to the scene. 
