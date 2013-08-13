#ifndef NGPP_POSITION_COMPONENT_H
#define NGPP_POSITION_COMPONENT_H

#include "../math/math.h"
#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Output.h"
#include "../core/Entity.h"

namespace NGPP {
    namespace Components {
        using namespace std;
        using namespace std::placeholders;
        using namespace NGPP::Core;
        using namespace NGPP::Math;

        class PositionComponent : public Component {
        public:
            PositionComponent(Entity *entity, IInput<Vector3> *input, IOutput<Vector3> *output) :
                Component(entity),
                input(input),
                output(output)
            {
                input->OnReceive(bind(&PositionComponent::SetPosition, this, _1));
            }

            PositionComponent(Entity *entity) :
                PositionComponent(entity, new Input<Vector3>(), new Output<Vector3>())
            {
            }

            void SetPosition(Vector3 value) {
                this->position = value;
                this->output->Send(value);
            }

            void SetX(float value) {
                this->position.x = value;
                this->output->Send(this->position);
            }

            void SetY(float value) {
                this->position.y = value;
                this->output->Send(this->position);
            }

            void SetZ(float value) {
                this->position.z = value;
                this->output->Send(this->position);
            }

            float GetX() {
                return this->position.x;
            }

            float GetY() {
                return this->position.y;
            }

            float GetZ() {
                return this->position.z;
            }

            IInput<Vector3> *input;
            IOutput<Vector3> *output;
            Vector3 position;
        };
    }
}

#endif //NGPP_POSITION_COMPONENT_H