#ifndef NGPP_INPUT_H
#define NGPP_INPUT_H

#include <functional>

using namespace std;

namespace NGPP {
    namespace Core {
        template<class T>
        class IInput {
        public:
            virtual ~IInput() {}
            virtual void Receive(T data) = 0;
            virtual void OnReceive(function<void(T)> onReceive) = 0;
        };

        template<class T>
        class Input : public IInput<T> {
        public:
            void Receive(T data) {
                this->onReceive(data);
            }

            void OnReceive(function<void(T)> onReceive) {
                this->onReceive = onReceive;
            }
        private:
            function<void(T)> onReceive;
        };
    }
}

#endif //NGPP_INPUT_H
