#ifndef NGPP_CAMERA_COMPONENT_H
#define NGPP_CAMERA_COMPONENT_H

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
#include "../renderer/Camera.h"

namespace NGPP {
    namespace Components {
        using namespace std;
        using namespace std::placeholders;
        using namespace NGPP::Core;
        using namespace NGPP::Math;
        using namespace NGPP::Renderer;

        class CameraComponent : public Component {
        public:
            CameraComponent(
                glm::mat4 projection,
                IInput<Vector3> *positionInput,
                IInput<Vector3> *targetInput
            ) :
                positionInput(positionInput),
                targetInput(targetInput),
                camera(Camera(projection))
            {
                positionInput->OnReceive(bind(&CameraComponent::ReceivePosition, this, _1));
                targetInput->OnReceive(bind(&CameraComponent::ReceiveTarget, this, _1));
            }

            CameraComponent(glm::mat4 projection) :
                CameraComponent(projection, new Input<Vector3>(), new Input<Vector3>())
            {
            }

            void ReceivePosition(Vector3 value)
            {
                this->camera.position = glm::vec3(value.x, value.y, value.z);
                this->camera.UpdateMatrix();
            }

            void ReceiveTarget(Vector3 value)
            {
                this->camera.target = glm::vec3(value.x, value.y, value.z);
                this->camera.UpdateMatrix();
            }

            
            IInput<Vector3> *positionInput;
            IInput<Vector3> *targetInput;
            Camera camera;
        };
    }
}

#endif //NGPP_CAMERA_COMPONENT_H