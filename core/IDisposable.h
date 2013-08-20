#ifndef NGPP_IDISPOSABLE_H
#define NGPP_IDISPOSABLE_H

namespace NGPP {
    namespace Core {
        class IDisposable {
        public:
            virtual void Dispose() = 0;
        };
    }
}

#endif // NGPP_IDISPOSABLE_H