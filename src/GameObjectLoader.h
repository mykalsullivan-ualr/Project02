
#pragma once

#include <tinyxml2.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ControllerComponent.h"
#include "SlideComponent.h"
#include "Behavior1Component.h"
#include "Behavior2Component.h"
#include "Textures.h"
#include "Engine.h"

class GameObjectLoader
{
public:
    static std::vector<std::unique_ptr<GameObject>> load(
        const std::string& filePath)
    {
        std::vector<std::unique_ptr<GameObject>> objects;

        std::unordered_map<std::string, GameObject*> objectMap;

        GameObject* player = nullptr;

        struct PendingComponent
        {
            GameObject* object;
            tinyxml2::XMLElement* element;
            std::string type;
        };

        std::vector<PendingComponent> pendingComponents;

        tinyxml2::XMLDocument doc;

        if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
        {
            SDL_Log("Failed to load XML: %s", filePath.c_str());
            SDL_Log("%s", doc.ErrorStr());
            return {};
        }

        auto* scene = doc.FirstChildElement("Scene");

        if (!scene)
        {
            SDL_Log("XML file must have a Scene root element");
            return {};
        }

        // PASS 1: Create objects and non-reference components.

        for (auto* element = scene->FirstChildElement("GameObject");
             element;
             element = element->NextSiblingElement("GameObject"))
        {
            const char* id = element->Attribute("id");

            if (!id || objectMap.count(id))
            {
                SDL_Log("GameObject has missing or duplicate ID");
                return {};
            }

            auto gameObject = std::make_unique<GameObject>();

            GameObject* objectPtr = gameObject.get();

            objectMap[id] = objectPtr;

            // Identify player.
            const char* objectType = element->Attribute("type");

            if (objectType && std::string(objectType) == "player")
            {
                if (player)
                {
                    SDL_Log("Multiple players found");
                    return {};
                }

                player = objectPtr;
            }

            for (auto* component = element->FirstChildElement();
                 component;
                 component = component->NextSiblingElement())
            {
                const std::string type = component->Name();

                // BODY COMPONENT
                if (type == "BodyComponent")
                {
                    double x = component->DoubleAttribute("x", 0);
                    double y = component->DoubleAttribute("y", 0);

                    gameObject->add<BodyComponent>(x, y);
                }

                // SPRITE COMPONENT
                else if (type == "SpriteComponent")
                {
                    const char* key =
                        component->Attribute("textureKey");

                    if (!key)
                    {
                        SDL_Log("SpriteComponent missing textureKey");
                        continue;
                    }

                    if (!Textures::get(key))
                    {
                        if (!Textures::load(
                            key,
                            key,
                            Engine::getRenderer()))
                        {
                            SDL_Log("Failed to load texture: %s", key);
                            continue;
                        }
                    }

                    gameObject->add<SpriteComponent>(key);
                }

                // CONTROLLER COMPONENT
                else if (type == "ControllerComponent")
                {
                    double speed =
                        component->DoubleAttribute("speed", 5.0);

                    gameObject->add<ControllerComponent>(speed);
                }

                // SLIDE COMPONENT
                else if (type == "SlideComponent")
                {
                    double speed =
                        component->DoubleAttribute("speed", 5.0);

                    int left =
                        component->IntAttribute("left", 0);

                    int right =
                        component->IntAttribute("right", Engine::width);

                    gameObject->add<SlideComponent>(
                        speed,
                        left,
                        right
                    );
                }

                // AI COMPONENTS: DEFER UNTIL PASS 2
                else if (type == "Behavior1Component" ||
                         type == "Behavior2Component")
                {
                    pendingComponents.push_back({
                        objectPtr,
                        component,
                        type
                    });
                }

                else
                {
                    SDL_Log("Unknown component: %s", type.c_str());
                }
            }

            objects.push_back(std::move(gameObject));
        }

        // PASS 2: Resolve references and create AI components.

        for (const auto& pending : pendingComponents)
        {
            auto* component = pending.element;

            // Default target is the player.
            const char* targetID = component->Attribute("target");

            std::string targetName =
                targetID ? targetID : "player";

            auto it = objectMap.find(targetName);

            if (it == objectMap.end())
            {
                SDL_Log("AI target not found: %s", targetName.c_str());
                return {};
            }

            GameObject* target = it->second;

            // FLEE AI
            if (pending.type == "Behavior1Component")
            {
                double speed =
                    component->DoubleAttribute("speed", 3.0);

                double range =
                    component->DoubleAttribute("detectionRange", 200.0);

                pending.object->add<Behavior1Component>(
                    *target,
                    speed,
                    range
                );
            }

            // FOLLOW AI
            else if (pending.type == "Behavior2Component")
            {
                double speed =
                    component->DoubleAttribute("speed", 2.0);

                double distance =
                    component->DoubleAttribute("stopDistance", 100.0);

                pending.object->add<Behavior2Component>(
                    *target,
                    speed,
                    distance
                );
            }
        }

        SDL_Log("Loaded %zu GameObjects", objects.size());
        return objects;
    }
};
