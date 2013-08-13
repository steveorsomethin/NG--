#ifndef NGPP_RENDER_COMPONENT_H
#define NGPP_RENDER_COMPONENT_H

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../math/math.h"
#include "../core/Time.h"
#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Entity.h"

namespace NGPP {
    namespace Components {
        using namespace std;
        using namespace std::placeholders;
        using namespace NGPP::Core;
        using namespace NGPP::Math;

        class RenderComponent : public Component, public ITick {
        public:
            RenderComponent(
                Entity *entity,
                GLuint positionUniform,
                IInput<Vector3> *positionInput
            ) :
                Component(entity),
                positionUniform(positionUniform),
                positionInput(positionInput)
            {
                positionInput->OnReceive(bind(&RenderComponent::ReceivePosition, this, _1));
            }

            RenderComponent(Entity *entity, GLuint positionUniform) :
                RenderComponent(entity, positionUniform, new Input<Vector3>())
            {
            }

            void ReceivePosition(Vector3 value) {
                this->position = glm::vec4(value.x, value.y, value.z, value.z);
            }

            void Tick(Time time) {
                glUniform4fv(this->positionUniform, 1, glm::value_ptr(this->position));
            }

            IInput<Vector3> *positionInput;
            glm::vec4 position;
            GLuint positionUniform;
        };
    }
}

#endif //NGPP_RENDER_COMPONENT_H