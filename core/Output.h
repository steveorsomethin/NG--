#ifndef NGPP_OUTPUT_H
#define NGPP_OUTPUT_H

#include <list>

using namespace std;

namespace NGPP {
    namespace Core {
        template<class T>
        class IInput;

        template<class T>
        class IOutput {
        public:
            virtual void Send(T data) = 0;
            virtual void Pipe(IInput<T> *subscriber) = 0;
        };

        template<class T>
        class Output : public IOutput<T> {
        public:
            void Send(T data) {
                for (auto subscriber : this->subscribers) {
                    subscriber->Receive(data);
                }
            }

            void Pipe(IInput<T> *subscriber) {
                this->subscribers.push_back(subscriber);
            }
        private:
            list<IInput<T>*> subscribers;
        };
    }
}

#endif //NGPP_OUTPUT_H
