
#define SDL_MAIN_HANDLED

#include "Engine.h"
#include "GameObjectLoader.h"
#include "Textures.h"

int main(int argc, char* argv[])
{
    if (!Engine::init("Garbage Game Demo", 800, 600))
        return -1;

    // Load textures.
    SDL_Renderer* renderer = Engine::getRenderer();

    Textures::load("player", "../assets/squirrel.png", renderer);
    Textures::load("enemy", "../assets/squirrel.png", renderer);

    // Load objects from XML.
    auto objects = GameObjectLoader::load("../data/scene.xml");

    if (objects.empty())
    {
        SDL_Log("Failed to load scene!");
        Engine::clean();
        return -1;
    }

    // Transfer ownership to the engine.
    for (auto& object : objects)
    {
        Engine::addGameObject(std::move(object));
    }

    // Run the engine.
    Engine::run();

    return 0;
}
