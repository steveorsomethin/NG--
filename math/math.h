#ifndef NGPP_MATH_H
#define NGPP_MATH_H

namespace NGPP {
    namespace Math {
        class Vector2 {
        public:
            float x;
            float y;
        };

        class Vector3 {
        public:
            Vector3() {}
            Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
        };

        class Vector4 {
        public:
            float x;
            float y;
            float z;
            float w;
        };

        class Matrix2 {
        public:
            int components[2][2];
        };

        class Matrix3 {
        public:
            int components[3][3];
        };

        class Matrix4 {
        public:
            int components[4][4];
        };

        class Quaternion {

        };
    }
}

#endif //NGPP_MATH_H
