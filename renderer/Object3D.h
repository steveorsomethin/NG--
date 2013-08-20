#ifndef NGPP_OBJECT3D_H
#define NGPP_OBJECT3D_H

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

namespace NGPP {
    namespace Renderer {
        using namespace std;

        class Object3D {
        public:
            Object3D()
            {
                this->position = glm::vec3(0.0f);
                this->up = glm::vec3(0.0f, 1.0f, 0.0f);
                this->scale = glm::vec3(1.0f);
            }

            void ApplyMatrix(glm::mat4 &matrix)
            {
                this->matrix *= matrix;
                glm::vec4 pos = this->matrix[3];

                this->position = glm::vec3(pos.x, pos.y, pos.z);
                this->rotation = glm::quat(glm::extractMatrixRotation(this->matrix));

                this->scale = glm::vec3(
                    glm::length(this->matrix[0]),
                    glm::length(this->matrix[1]),
                    glm::length(this->matrix[2])
                );
            }

            void UpdateMatrix()
            {
                this->matrix =
                    glm::translate(this->position) *
                    glm::mat4_cast(this->rotation) *
                    glm::scale(this->scale);
            }

            void SetRotationFromAxisAngle(glm::vec3 axis, float angle)
            {
                this->rotation = glm::angleAxis(angle, axis);
            }

            Object3D *parent;
            list<Object3D*> children;
            glm::vec3 up;
            glm::vec3 position;
            glm::vec3 scale;
            glm::vec4 color;
            glm::quat rotation;
            glm::mat4 matrix;
            glm::mat4 world;
            bool matrixWorldNeedsUpdate;
        };
    }
}

#endif //NGPP_OBJECT3D_H