#ifndef NGPP_ENTITY_H
#define NGPP_ENTITY_H

namespace NGPP {
    namespace Core {
        class Entity {
        public:
            Entity(int id) : id(id) {}

            int id;
        };
    }
}

#endif //NGPP_ENTITY_H