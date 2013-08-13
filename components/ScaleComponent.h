#ifndef NGPP_SCALE_COMPONENT_H
#define NGPP_SCALE_COMPONENT_H

#include "../math/math.h"
#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Output.h"
#include "../core/Transformer.h"

using namespace std;
using namespace std::placeholders;

namespace NGPP {
    namespace Components {
        class ScaleComponent : public Transformer<Vector3, Vector3> {
        public:
            ScaleComponent(float scaleFactor, IInput<Vector3> *input, IOutput<Vector3> *output) :
                Transformer(input, output),
                scaleFactor(scaleFactor) {}

            void Transform(Vector3 value) {
                value.x *= this->scaleFactor;
                value.y *= this->scaleFactor;
                value.z *= this->scaleFactor;

                this->output->Send(value);
            }

        private:
            float scaleFactor = 1.0f;
        };
    }
}

#endif //NGPP_SCALE_COMPONENT_H
