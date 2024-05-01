/**
 * Example of drawing a basic colored opengl triangle.
 */
#include <moderngl>

void draw_triangle() {
    using namespace std;
    /**
     * Create a <font color="#78DCE8">vector</font> with data of our triangle.
     * <p><i><font color="#78DCE8">Vectors</font> are used for <font color="#A9DC76">safety</font> and avoidance of possible <font color="#FF6188">memory leaks</font>.</i></p>
     */
    vector<float> triangleMesh{
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    /**
     * Create a context by simply unsing <font color="#A9DC76">gl</font><font color="#FF6188">::</font><font color="#A9DC76">createContext</font> function.
     */
    gl::createContext(1000, 500, "Hello");
    /**
     * Now we need to create a <font color="#78DCE8">shader program</font> which will work on gpu.
     * Start from creating strings with text of shaders and buffer string for reporting <font color="#FF6188">errors</font>.
     */
    string vertexShader, fragmentShader, logBuffer;
    /**
     * Write <font color="#78DCE8">shader data</font> to variables with strings. We will load shader files.
     */
    helpers::readFile("triangle.vsh", vertexShader); helpers::readFile("triangle.fsh", fragmentShader);
    /**
     * Create a shader program with <font color="#A9DC76">gl</font><font color="#FF6188">::</font><font color="#A9DC76">createProgram</font> function.
     */
    gl::program* shaderProgram = gl::createProgram(vertexShader, fragmentShader, logBuffer);
    /**
     * Now create an array buffer with <font color="#A9DC76">gl</font><font color="#FF6188">::</font><font color="#A9DC76">createBuffer</font> function.
     * <p><i>You have to specify the <font color="#78DCE8">opengl type</font> of stored data</i></p>
     */
    gl::buffer* triangleBuffer = gl::createBuffer(triangleMesh, gl::Float);
    gl::vertexArray* vertexArrayObject = gl::createVertexArray(shaderProgram, 3);
    gl::bindBuffer(vertexArrayObject, triangleBuffer, "3 3", "0 1", 6*sizeof(float));
}