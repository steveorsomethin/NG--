#ifndef NGPP_TRANSITION_COMPONENT_H
#define NGPP_TRANSITION_COMPONENT_H

#include "../math/math.h"
#include "../core/Time.h"
#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Output.h"
#include "../core/Transformer.h"

#include <list>

using namespace std;
using namespace placeholders;

namespace NGPP {
    namespace Components {
        template<class T>
        struct Tween {
            Tween(T *property, T value, float duration, float remainingDuration) : 
                property(property),
                value(value),
                duration(duration),
                remainingDuration(remainingDuration) {};

            T *property;
            T value;
            const float duration;
            float remainingDuration;
        };

        template<class T>
        class TweenManager {
        public:
            bool Integrate(Time time) {
                int elapsedTime = time.elapsedTime;
                bool changed = false;

                for (auto tween : this->tweens) {
                    T *property = tween->property;
                    T value = tween->value;
                    bool finished = true;

                    *property += (value - *property) * elapsedTime / tween->remainingDuration;
                    tween->remainingDuration -= elapsedTime;

                    if (tween->remainingDuration > 0 && *property != value)  {
                        finished = false;
                    } else {
                        *property = value;
                    }

                    if (finished) {
                        this->tweens.remove(tween);
                        //delete tween;
                    }

                    changed = true;
                }

                return changed;
            }

            void Tween(T *property, T value, float duration) {
                this->tweens.push_back(new NGPP::Components::Tween<T>(property, value, duration, duration));
            }

        private:
            list<NGPP::Components::Tween<T>*> tweens;
        };

        class TweenComponent : public Component, public ITick {
        public:
            TweenComponent(float duration, IInput<Vector3> *input, IOutput<Vector3> *output) :
                Component(),
                duration(duration),
                input(input),
                output(output)
            {
                this->input->OnReceive(bind(&TweenComponent::Tween, this, _1));
            }

            TweenComponent(float duration) :
                TweenComponent(duration, new Input<Vector3>(), new Output<Vector3>())
            {
            }

            void Tween(Vector3 value)
            {
                this->tweenManager.Tween(&(this->value.x), value.x, this->duration);
                this->tweenManager.Tween(&(this->value.y), value.y, this->duration);
                this->tweenManager.Tween(&(this->value.z), value.z, this->duration);
            }

            void Tick(Time time)
            {
                if (this->tweenManager.Integrate(time)) {
                    this->output->Send(this->value);
                }
            }

            IInput<Vector3> *input;
            IOutput<Vector3> *output;
            Vector3 value;
            float duration = 0;
        private:
            TweenManager<float> tweenManager;
        };
    }
}

#endif //NGPP_TRANSITION_COMPONENT_H