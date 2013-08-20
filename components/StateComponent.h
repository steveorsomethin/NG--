#ifndef NGPP_STATE_COMPONENT_H
#define NGPP_STATE_COMPONENT_H

#include "../core/Component.h"
#include "../core/Input.h"
#include "../core/Output.h"

namespace NGPP {
    namespace Components {
        using namespace std;
        using namespace std::placeholders;

        class StateComponent : public Component {
        public:
            StateComponent(IInput<int> *input, IOutput<int> *output) :
                Component(),
                input(input),
                output(output)
            {
                input->OnReceive(bind(&StateComponent::SetState, this, _1));
            }

            StateComponent() :
                StateComponent(new Input<int>(), new Output<int>())
            {
            }

            void SetState(int value)
            {
                this->state = value;
                this->output->Send(value);
            }

            IInput<int> *input;
            IOutput<int> *output;
            int state;
        };
    }
}

#endif //NGPP_STATE_COMPONENT_H