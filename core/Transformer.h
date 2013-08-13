#ifndef NGPP_TRANSFORMER_H
#define NGPP_TRANSFORMER_H

#include <functional>

#include "Input.h"
#include "Output.h"

namespace NGPP {
    namespace Core {
        using namespace std;
        using namespace std::placeholders;

        template<class TInput, class TOutput>
        class ITransformer {
        public:
            virtual void Transform(TInput value) = 0;
        };

        template<class TInput, class TOutput>
        class Transformer : public ITransformer<TInput, TOutput> {
        public:
            Transformer(IInput<TInput> *input, IOutput<TOutput> *output) :
                input(input),
                output(output)
            {
                this->input->OnReceive(bind(&Transformer::Transform, this, _1));
            }

            IInput<TInput> *input;
            IOutput<TOutput> *output;
        };
    }
}

#endif //NGPP_TRANSFORMER_H