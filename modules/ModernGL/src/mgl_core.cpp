#ifndef MGL_CORE_CPP
#define MGL_CORE_CPP

#include <OpenGL>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

typedef unsigned int u32;
typedef unsigned long long ull;

namespace helpers {
    void readFile(const char *name, std::string &buf) {
        std::ifstream fin(name);
        std::stringstream reader;
        reader << fin.rdbuf();
        fin.close();
        buf.assign(reader.str());
        reader.clear();
    }

    int count_items(const std::string &str) {
        bool s = false;
        int c = 0;
        for (const char i: str)
            if (!isgraph(i) && s) {
                c++;
                s = false;
            } else if (isgraph(i)) {
                s = true;
            }
        if (s) c++;
        return c;
    }

    int calculate_stride(const std::vector<int> &v, const int dsize) {
        int s = 0;
        for (const int i: v)
            s += i;
        return s * dsize;
    }
}

namespace gl {
    inline constexpr u32 Float = GL_FLOAT;
    inline constexpr u32 Double = GL_DOUBLE;
    inline constexpr u32 Int = GL_INT;
    inline constexpr u32 UInt = GL_UNSIGNED_INT;
    inline constexpr u32 Byte = GL_BYTE;
    inline constexpr u32 UByte = GL_UNSIGNED_BYTE;

    typedef struct _buffer_ {
        u32 id;
        u32 dtype;
        ull size;
        ull tsize;
    } buffer;

    typedef struct _program_ {
        u32 id;
        bool is_used;
    } program;

    typedef struct _vao_ {
        u32 id;
        u32 render_method;
        ull size;
        const program *shader;
    } vertexArray;

    typedef GLFWwindow window;
    window *context;

    void createContext(const int width, const int height, const char *title) {
        glfwInit();
        context = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(context);
        gladLoadGL();
        glViewport(0, 0, width, height);
    }

    inline bool windowIsClosed() {
        return glfwWindowShouldClose(context);
    }

    inline void clearColor(const float r, const float g, const float b, const float a = 1.0) {
        glClearColor(r, g, b, a);
    }

    void nextFrame() {
        glfwPollEvents();
        glfwSwapBuffers(context);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template<typename T>
    buffer *createBuffer(const std::vector<T> &data, const u32 dtype) {
        u32 id;
        auto buf = new buffer{.dtype = dtype, .size = data.size(), .tsize = sizeof(T)};
        glGenBuffers(1, &id);
        buf->id = id;
        glBindBuffer(GL_ARRAY_BUFFER, id);
        const T *ptr = &data[0];
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), ptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return buf;
    }

    program *createProgram(const std::string &vsh_src, const std::string &fsh_src, std::string &infoLog) {
        int s;
        const auto shader_program = new program{};
        const u32 vid = glCreateShader(GL_VERTEX_SHADER);
        const u32 fid = glCreateShader(GL_FRAGMENT_SHADER);
        const char *cvsh = vsh_src.c_str();
        glShaderSource(vid, 1, &cvsh, nullptr);
        glCompileShader(vid);
        glGetShaderiv(vid, GL_COMPILE_STATUS, &s);
        if (!s) {
            char vlog[1024];
            glGetShaderInfoLog(vid, 1024, nullptr, vlog);
            infoLog.append(vlog);
        }
        const char *cfsh = fsh_src.c_str();
        glShaderSource(fid, 1, &cfsh, nullptr);
        glCompileShader(fid);
        glGetShaderiv(fid, GL_COMPILE_STATUS, &s);
        if (!s) {
            char flog[1024];
            glGetShaderInfoLog(fid, 1024, nullptr, flog);
            infoLog.append(flog);
        }
        shader_program->id = glCreateProgram();
        glAttachShader(shader_program->id, vid);
        glAttachShader(shader_program->id, fid);
        glLinkProgram(shader_program->id);
        glGetProgramiv(shader_program->id, GL_LINK_STATUS, &s);
        if (!s) {
            char plog[1024];
            glGetProgramInfoLog(shader_program->id, 1024, nullptr, plog);
            infoLog.append(plog);
        }
        glDeleteShader(vid);
        glDeleteShader(fid);
        return shader_program;
    }

    void useProgram(const program *shader) {
        glUseProgram(shader->id);
    }

    u32 getTypeSize(const u32 glType) {
        if (glType == Float || glType == Int || glType == UInt)
            return sizeof(float);
        if (glType == Byte || glType == UByte)
            return sizeof(char);
        if (glType == Double)
            return sizeof(double);
        return 0;
    }

    inline void bufferAttribute(const buffer *buf, const u32 index, const u32 pushSize, const u32 stride,
                                const u32 firstElement) {
        glBindBuffer(GL_ARRAY_BUFFER, buf->id);
        glVertexAttribPointer(index, static_cast<int>(pushSize), buf->dtype, GL_FALSE, static_cast<int>(stride),
                              reinterpret_cast<void *>(firstElement * buf->tsize));
    }

    inline void enableVertexAttribute(const u32 index) {
        glEnableVertexAttribArray(index);
    }

    vertexArray *createVertexArray(const program *shader, const ull render_cycles,
                                   const u32 render_method = GL_TRIANGLES) {
        const auto vao = new vertexArray{.render_method = render_method, .size = render_cycles, .shader = shader};
        glGenVertexArrays(1, &vao->id);
        glBindVertexArray(vao->id);
        return vao;
    }

    inline void useVertexArray(const vertexArray *vao) {
        glBindVertexArray(vao->id);
    }

    void bindBuffer(const vertexArray *vao, const buffer *buf,
                    const std::vector<int> &format, const std::vector<int> &locations) {
        useVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, buf->id);
        int elem = 0;
        const ull full_stride = helpers::calculate_stride(format, static_cast<int>(getTypeSize(buf->dtype)));
        for (int i = 0; i < format.size(); i++) {
            const int size = format[i], location = locations[i];
            bufferAttribute(buf, location, size, full_stride, elem);
            enableVertexAttribute(location);
            elem += size;
        }
    }

    inline void render(const vertexArray *vao) {
        useProgram(vao->shader);
        useVertexArray(vao);
        glDrawArrays(vao->render_method, 0, static_cast<int>(vao->size));
    }
}

#endif //MGL_CORE_CPP
