#include <OpenGL>
#include <vector>
#include <sstream>
#include <string>

typedef unsigned int u32;
typedef unsigned long long ull;

namespace _helpers_ {

}

namespace gl {
    inline const u32 Float  = GL_FLOAT;
    inline const u32 Double = GL_DOUBLE;
    inline const u32 Int    = GL_INT;
    inline const u32 UInt   = GL_UNSIGNED_INT;
    inline const u32 Byte   = GL_BYTE;
    inline const u32 UByte  = GL_UNSIGNED_BYTE;
    typedef struct _buffer_ {
        u32 id;
        u32 dtype;
        bool is_used;
    } buffer;
    typedef struct _program_ {
        u32 id;
        bool is_used;
    } program;
    typedef struct _vao_ {
        u32 id;
        u32 render_method;
        u32 dtype;
        ull size;
        std::vector<buffer*>& buffers;
        std::vector<u32>& attribArrays;
        bool is_used;
    } vertexArray;
    typedef GLFWwindow window;
    window* context;
    void createContext(int width, int height, const char* title) {
        glfwInit();
        gl::context = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(gl::context);
        gladLoadGL();
    }

    bool windowIsClosed() {
        return glfwWindowShouldClose(gl::context);
    }

    void nextFrame() {
        glfwPollEvents();
        glfwSwapBuffers(gl::context);
    }

    template<typename T>
    buffer* createBuffer(const std::vector<T>& data, u32 dtype) {
        static buffer buf = buffer{.dtype=dtype};
        glGenBuffers(1, &buf.id);
        glBindBuffer(GL_ARRAY_BUFFER, buf.id);
        T* ptr = &data.front();
        glBufferData(GL_ARRAY_BUFFER, data.size(), ptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return &buf;
    }
    program* createProgram(const std::string& vsh_src, const std::string& fsh_src, u32& success, std::string& infoLog) {
        int s;
        char vlog[8192];
        char flog[8192];
        char plog[8192];
        success = 1;
        
        u32 vid, fid;

        static program shader_program = program{};
        vid = glCreateShader(GL_VERTEX_SHADER);
        fid = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cvsh = vsh_src.c_str();
        glShaderSource(vid, 1, &cvsh, nullptr);
        glCompileShader(vid);
        glGetShaderiv(vid, GL_COMPILE_STATUS, &s);
        if (!s) {
            std::string l;
            glGetShaderInfoLog(vid, 8192, nullptr, vlog);
            l.assign(vlog);
            infoLog.append(l);
            success = 0;
        }

        const char* cfsh = fsh_src.c_str();
        glShaderSource(fid, 1, &cfsh, nullptr);
        glCompileShader(fid);
        glGetShaderiv(fid, GL_COMPILE_STATUS, &s);
        if (!s) {
            std::string l;
            glGetShaderInfoLog(fid, 8192, nullptr, flog);
            l.assign(flog);
            infoLog.append(l);
            success = 0;
        }

        shader_program.id = glCreateProgram();
        glAttachShader(shader_program.id, vid);
        glAttachShader(shader_program.id, fid);
        glLinkProgram(shader_program.id);
        glGetProgramiv(shader_program.id, GL_LINK_STATUS, &s);
        if (!s) {
            std::string l;
            glGetProgramInfoLog(fid, 8192, nullptr, plog);
            l.assign(plog);
            infoLog.append(l);
            success = 0;
        }

        glDeleteShader(vid);
        glDeleteShader(fid);

        return &shader_program;
    }
    void bufferAttribute(const buffer* buf, u32 index, u32 pushSize, u32 dtype, u32 stride, u32 firstElement) {
        ull dtsize = 0;
        if (dtype == gl::Float || dtype == gl::Int || dtype == gl::UInt)
            dtsize = sizeof(float);
        else if (dtype == gl::Byte || dtype == gl::UByte)
            dtsize = sizeof(char);
        else if (dtype == gl::Double)
            dtsize = sizeof(double);
        glBindBuffer(GL_ARRAY_BUFFER, buf->id);
        glVertexAttribPointer(index, pushSize, dtype, GL_FALSE, stride * dtsize, (void*)(firstElement * dtsize));
    }
    void enableVertexAttribute(u32 index) {
        glEnableVertexAttribArray(index);
    }
    vertexArray* createVertexArray_simple() {
        
    }
}