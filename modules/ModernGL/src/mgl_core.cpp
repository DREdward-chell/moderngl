#include <OpenGL>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

typedef unsigned int u32;
typedef unsigned long long ull;

namespace helpers {
    void readFile(const char* name, std::string& buf) {
        std::ifstream fin(name);
        std::stringstream reader;
        reader << fin.rdbuf();
        fin.close();
        buf.assign(reader.str());
        reader.clear();
    }
    int count_items(const std::string& str) {
        bool s = false;
        int c = 0;
        for (char i : str)
            if (!isgraph(i) && s) {
                c++;
                s = false;
            } else if (isgraph(i)) {
                s = true;
            }
        if (s) c++;
        return c;
    }
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
        program* shader;
    } vertexArray;
    typedef GLFWwindow window;
    window* context;
    void createContext(int width, int height, const char* title) {
        glfwInit();
        gl::context = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(gl::context);
        gladLoadGL();
        glViewport(0, 0, width, height);
    }
    inline bool windowIsClosed() {
        return glfwWindowShouldClose(gl::context);
    }

    void nextFrame() {
        glfwPollEvents();
        glfwSwapBuffers(gl::context);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template<typename T>
    buffer* createBuffer(const std::vector<T>& data, u32 dtype) {
        u32 id;
        buffer* buf;
        buf = new buffer{.dtype=dtype, .size=data.size(), .tsize=sizeof(T)};
        glGenBuffers(1, &id);
        buf->id = id;
        glBindBuffer(GL_ARRAY_BUFFER, id);
        const T* ptr = &data[0];
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), ptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return (buffer*)buf;
    }
    program* createProgram(const std::string& vsh_src, const std::string& fsh_src, std::string& infoLog) {
        int s;
        char vlog[1024];
        char flog[1024];
        char plog[1024];
        
        u32 vid, fid;

        program* shader_program;
        shader_program = new program{};

        vid = glCreateShader(GL_VERTEX_SHADER);
        fid = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cvsh = vsh_src.c_str();
        glShaderSource(vid, 1, &cvsh, nullptr);
        glCompileShader(vid);
        glGetShaderiv(vid, GL_COMPILE_STATUS, &s);
        if (!s) {
            glGetShaderInfoLog(vid, 1024, nullptr, vlog);
            infoLog.append(vlog);
        }

        const char* cfsh = fsh_src.c_str();
        glShaderSource(fid, 1, &cfsh, nullptr);
        glCompileShader(fid);
        glGetShaderiv(fid, GL_COMPILE_STATUS, &s);
        if (!s) {
            glGetShaderInfoLog(fid, 1024, nullptr, flog);
            infoLog.append(flog);
        }

        shader_program->id = glCreateProgram();
        glAttachShader(shader_program->id, vid);
        glAttachShader(shader_program->id, fid);
        glLinkProgram(shader_program->id);
        glGetProgramiv(shader_program->id, GL_LINK_STATUS, &s);
        if (!s) {
            glGetProgramInfoLog(shader_program->id, 1024, nullptr, plog);
            infoLog.append(plog);
        }

        glDeleteShader(vid);
        glDeleteShader(fid);

        return (program*)shader_program;
    }
    void useProgram(const program* shader) {
        glUseProgram(shader->id);
    }
    u32 getTypeSize(u32 glType) {
        if (glType == gl::Float || glType == gl::Int || glType == gl::UInt)
            return sizeof(float);
        else if (glType == gl::Byte || glType == gl::UByte)
            return sizeof(char);
        else if (glType == gl::Double)
            return sizeof(double);
        return 0;
    }
    inline void bufferAttribute(const buffer* buf, u32 index, u32 pushSize, u32 stride, u32 firstElement) {
        glBindBuffer(GL_ARRAY_BUFFER, buf->id);
        glVertexAttribPointer(index, pushSize, buf->dtype, GL_FALSE, stride, (void*)(firstElement * buf->tsize));
    }
    inline void enableVertexAttribute(u32 index) {
        glEnableVertexAttribArray(index);
    }
    vertexArray* createVertexArray(program* shader, ull render_cycles, u32 render_method=GL_TRIANGLES) {
        vertexArray* vao;
        vao = new vertexArray{.render_method=render_method, .size=render_cycles, .shader=shader};
        glGenVertexArrays(1, &vao->id);
        glBindVertexArray(vao->id);
        return (vertexArray*)vao;
    }
    inline void useVertexArray(const vertexArray* vao) {
        glBindVertexArray(vao->id);
    }
    void bindBuffer(vertexArray* vao, buffer* buf, const std::string& format, const std::string& locations, ull full_stride) {
        useVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, buf->id);
        std::stringstream reader1, reader2;
        reader1 << format; reader2 << locations;
        int f, l, elem = 0;
        const int items = helpers::count_items(format);
        for (int i = 0; i < items; i++) {
            reader1 >> f; reader2 >> l;
            bufferAttribute(buf, l, f, full_stride, elem);
            enableVertexAttribute(l);
            elem += f;
        }
        reader1.clear(); reader2.clear();
    }
    inline void render(const vertexArray* vao) {
        gl::useProgram(vao->shader);
        gl::useVertexArray(vao);
        glDrawArrays(vao->render_method, 0, vao->size);
    }
}