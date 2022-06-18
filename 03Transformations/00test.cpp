#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

int main()
{

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans(1.0f);
    trans = glm::translate(trans,glm::vec3(1.0f,1.0f,0.0f));
    vec = trans*vec;
    printf("%.2f\t%.2f\t%.2f\\n",vec.x,vec.y,vec.z);
    return 0;
}