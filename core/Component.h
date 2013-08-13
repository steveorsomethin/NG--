#ifndef NGPP_COMPONENT_H
#define NGPP_COMPONENT_H

namespace NGPP {
    namespace Core {
        class Entity;

        class Component {
        public:
            Component(Entity *entity) : entity(entity) {}

            Entity *entity;
        };
    }
}

#endif //NGPP_COMPONENT_H