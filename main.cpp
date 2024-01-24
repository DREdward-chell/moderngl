#include <iostream>
#include <moderngl>

using namespace std;

int main() {
    vector<float> d{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    gl::createContext(1000, 1000, "Hello");
    string vertex, fragment, log;
    helpers::readFile("shaders/vertex.vsh", vertex);
    helpers::readFile("shaders/fragment.fsh", fragment);
    gl::program* shaderProgram = gl::createProgram(vertex, fragment, log);
    vertex.clear(); fragment.clear(); log.clear();

    gl::buffer* t_buf = gl::createBuffer(d, gl::Float);
    gl::vertexArray* triangle = gl::createVertexArray_simple(t_buf);

    while (!gl::windowIsClosed()) {
        gl::useProgram(shaderProgram);
        gl::useVertexArray(triangle);
        gl::render(triangle);
        gl::nextFrame();
    }
}
