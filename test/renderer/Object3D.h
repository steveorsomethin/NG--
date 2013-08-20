#include <renderer/Object3D.h>
#include <glm/glm.hpp>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace NGPP::Renderer;
using namespace ::testing;

TEST(Object3D, ApplyMatrix)
{
    Object3D obj;
    float theta = 30.0f;
    float c = cos(theta);
    float s = sin(theta);

    glm::mat4 mat;
    glm::mat4 mat2 = glm::scale(3.0f, 2.0f, 1.0f);

    mat[1][1] = c;
    mat[2][1] = -s;
    mat[1][2] = s;
    mat[2][2] = c;

    obj.ApplyMatrix(mat);
    obj.ApplyMatrix(mat2);

    assert(true);
}
