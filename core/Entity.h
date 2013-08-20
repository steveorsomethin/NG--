#ifndef NGPP_ENTITY_H
#define NGPP_ENTITY_H

#include <map>

#include "Component.h"

namespace NGPP {
    namespace Core {
        using namespace std;

        class Entity {
        public:
            Entity(int id) : id(id) {}
            Entity(int id, map<string, Component*> components) :
                id(id),
                components(components)
            {
                for (auto pair : components) {
                    pair.second->entity = this;
                }
            }

            template<typename T>
            T GetComponent(string key)
            {
                return reinterpret_cast<T>(this->components[key]);
            }

            void SetComponent(string key, Component *component)
            {
                component->entity = this;
                this->components[key] = component;
            }

            int id;
            map<string, Component*> components;
        };
    }
}

#endif //NGPP_ENTITY_H