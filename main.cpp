#include <moderngl>

using namespace std;
using namespace gl;

int main() {
    const vector mesh{
        0.f, 0.f, 1.f, 0.f, 0.f,
        -1.f, -1.f, 0.f, 1.f, 0.f,
        -1.f, 1.f, 0.f, 0.f, 1.f
    };
    createContext(1000, 500, "Garrys mod");

    string v, f, l;
    helpers::readFile("shaders/vertex.vsh", v);
    helpers::readFile("shaders/fragment.fsh", f);
    const program *program = createProgram(v, f, l);
    const buffer *buf = createBuffer(mesh, Float);
    const vertexArray *vertexArray = createVertexArray(program, 4, GL_QUADS);
    bindBuffer(vertexArray, buf, {2, 3}, {0, 1});

    while (!windowIsClosed()) {
        nextFrame();
        render(vertexArray);
    }
}