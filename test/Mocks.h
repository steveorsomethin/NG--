#ifndef NGPP_MOCKS_H
#define NGPP_MOCKS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/Input.h>
#include <core/Output.h>

namespace NGPP {
    namespace Core {
        using namespace ::testing;

        template<class T>
        class MockInput : public IInput<T> {
        public:
            MOCK_METHOD1_T(Receive, void(T data));
            void OnReceive(function<void(T)> onReceive) {}
        };

        template<class T>
        class MockOutput : public IOutput<T> {
        public:
            MOCK_METHOD1_T(Send, void (T data));
            void Pipe(IInput<T> *subscriber) {}
        };
    }
}

#endif //NGPP_MOCKS_H
