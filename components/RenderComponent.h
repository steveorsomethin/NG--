#ifndef NGPP_RENDER_COMPONENT_H
#define NGPP_RENDER_COMPONENT_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../math/math.h"
#include "../core/Time.h"
#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Entity.h"
#include "../renderer/Object3D.h"

namespace NGPP {
    namespace Components {
        using namespace std;
        using namespace std::placeholders;
        using namespace NGPP::Core;
        using namespace NGPP::Math;
        using namespace NGPP::Renderer;

        class RenderComponent : public Component {
        public:
            RenderComponent(IInput<Vector3> *positionInput) :
                Component(),
                positionInput(positionInput)
            {
                positionInput->OnReceive(bind(&RenderComponent::ReceivePosition, this, _1));
            }

            RenderComponent() :
                RenderComponent(new Input<Vector3>())
            {
            }

            void ReceivePosition(Vector3 value)
            {
                this->object3d.position = glm::vec3(value.x, value.y, value.z);
                this->object3d.scale = glm::vec3(0.75);
                this->object3d.UpdateMatrix();
            }

            IInput<Vector3> *positionInput;
            Object3D object3d;
        };
    }
}

#endif //NGPP_RENDER_COMPONENT_H