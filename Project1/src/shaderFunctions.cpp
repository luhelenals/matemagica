#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
<<<<<<< HEAD
=======
    std::string GeometrySource;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
};

static struct ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
<<<<<<< HEAD
        NONE = -1, VERTEX = 0, FRAGMENT = 1
=======
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
<<<<<<< HEAD
=======
            else if (line.find("geometry") != std::string::npos) {
                // set mode to geometry
                type = ShaderType::GEOMETRY;
            }
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return {
        ss[0].str(),
        ss[1].str(),
<<<<<<< HEAD
=======
        ss[2].str()
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    };
}

static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //&source[0]
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    //error handling ->
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        
        if (type == GL_VERTEX_SHADER) {
            std::cout << "Failed to compile vertex shader" << std::endl;
        } else if (type == GL_FRAGMENT_SHADER) {
            std::cout << "Failed to compile fragment shader" << std::endl;
<<<<<<< HEAD
=======
        } else if (type == GL_GEOMETRY_SHADER) {
            std::cout << "Failed to compile geometry shader" << std::endl;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
        }

        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

<<<<<<< HEAD
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
=======
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glAttachShader(program, gs);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
<<<<<<< HEAD
=======
    glDeleteShader(gs);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56

    return program;
}