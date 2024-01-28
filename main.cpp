#include <moderngl>

using namespace std;

int main() {
    vector<float> mesh{
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    gl::createContext(1000, 500, "Hello");


}
