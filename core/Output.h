#ifndef NGPP_OUTPUT_H
#define NGPP_OUTPUT_H

#include <list>

namespace NGPP {
    namespace Core {
        using namespace std;

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
            void Send(T data)
            {
                this->lastValue = data;
                for (auto subscriber : this->subscribers) {
                    subscriber->Receive(data);
                }
            }

            void Pipe(IInput<T> *subscriber)
            {
                this->subscribers.push_back(subscriber);
                subscriber->Receive(this->lastValue);
            }
        private:
            list<IInput<T>*> subscribers;
            T lastValue;
        };
    }
}

#endif //NGPP_OUTPUT_H
