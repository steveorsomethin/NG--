#ifndef NGPP_OBSERVABLE_VALUE_H
#define NGPP_OBSERVABLE_VALUE_H

#include <list>
#include <functional>
#include <core/IDisposable.h>

namespace NGPP {
    namespace Core {
        using namespace std;

        template<class T>
        class ObservableValueSubscription : public IDisposable {
        public:

            ObservableValueSubscription(T subscriber, list<T> subscriptions) :
                subscriber(subscriber),
                subscriptions(subscriptions)
            {
            }

            void Dispose() {
                this->subscriptions.remove(subscriber);
            }

        private:
            T subscriber;
            list<T> subscriptions;
        };

        template<class T>
        class ObservableValue {
        public:
            ObservableValue(T value)
            {
                this->value = value;
            }

            void operator =(T value)
            {
                this->value = value;

                for (auto subscriber : this->subscriberObservables) {
                    *subscriber = value;
                }

                this->receiver(value);
            }

            void operator =(ObservableValue<T> &obs)
            {
                this->value = obs.value;

                for (auto subscriber : this->subscriberObservables) {
                    *subscriber = obs.value;
                }

                this->receiver(value);
            }

            operator const T&() const
            {
                return this->value;
            }

            ObservableValueSubscription<ObservableValue<T>*> PipeTo(ObservableValue<T> *subscriber)
            {
                return this->PipeTo(subscriber, false);
            }

            ObservableValueSubscription<ObservableValue<T>*> PipeTo(ObservableValue<T> *subscriber, bool immediate)
            {
                this->subscriberObservables.push_back(subscriber);

                if (immediate) {
                    *subscriber = this->value;
                }

                return ObservableValueSubscription<ObservableValue<T>*>(subscriber, this->subscriberObservables);
            }

            void SetReceiver(function<void(T)> receiver, bool immediate)
            {
                this->receiver = receiver;

                if (immediate) {
                    this->receiver(this->value);
                }
            }

            T value;

            private:
                list<ObservableValue<T>*> subscriberObservables;
                function<void(T)> receiver = [](T){};
        };
    }
}

#endif // NGPP_OBSERVABLE_VALUE_H