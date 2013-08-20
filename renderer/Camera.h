#ifndef NGPP_CAMERA_H
#define NGPP_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include "Object3D.h"

namespace NGPP {
    namespace Renderer {
        class Camera : public Object3D {
        public:
            Camera(glm::mat4 projection) : Object3D(),
                projection(projection)
            {
            }

            void LookAt(glm::vec3 target)
            {
                this->target = target;
            }

            void UpdateMatrix()
            {
                this->matrix = this->projection * glm::lookAt(
                    this->position,
                    this->target,
                    this->up
                );
            }

            glm::vec3 target;
            glm::mat4 projection;
        };
    }
}

#endif //NGPP_CAMERA_H