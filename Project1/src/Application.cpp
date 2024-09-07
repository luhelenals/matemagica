#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include "glm/glm.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLCheckError(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "ERROR in file " << file << ": " << error << std::endl;
        std::cout << "Function: " << function << " in line " << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

struct PointsAndIndices {
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};

static struct ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
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
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return {
        ss[0].str(),
        ss[1].str()
    };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
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
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

std::vector<float> generateHeartVertices(float scale = 1.0f, int num_segments = 50) {
    std::vector<float> vertices;
    vertices.push_back(0.0f); // Center of the heart
    vertices.push_back(0.0f);

    for (int i = 0; i <= num_segments; ++i) {
        float angle = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = scale * (16.0f * sin(angle) * sin(angle) * sin(angle));
        float y = scale * (13.0f * cos(angle) - 5.0f * cos(2.0f * angle) - 2.0f * cos(3.0f * angle) - cos(4.0f * angle));
        vertices.push_back(x / 16.0f);
        vertices.push_back(y / 16.0f);
    }
    return vertices;
}

PointsAndIndices generateTree() {
    std::vector<float> positions = {
        // positions        // colors (R, G, B)
        // Tree trunk
        -0.1f, -0.5f,      0.55f, 0.27f, 0.07f,
         0.1f, -0.5f,      0.55f, 0.27f, 0.07f,
         0.1f,  0.0f,      0.55f, 0.27f, 0.07f,
        -0.1f,  0.0f,      0.55f, 0.27f, 0.07f,
        // Tree leaves
        -0.5f,  0.0f,      0.0f, 0.8f, 0.0f,
         0.5f,  0.0f,      0.0f, 0.8f, 0.0f,
         0.0f,  0.8f,      0.0f, 0.8f, 0.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,  // tree trunk
        4, 5, 6            // tree leaves
    };
    
    return { positions, indices };
}

PointsAndIndices generateRGBTriangle() {
    std::vector<float> positions = {
        // positions        // colors (R, G, B)
        -0.5f,  -0.5f,      1.0f, 0.0f, 0.0f,
         0.5f,  -0.5f,      0.0f, 1.0f, 0.0f,
         0.0f,   0.5f,      0.0f, 0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
       0, 1, 2
    };

    return { positions, indices };
}

PointsAndIndices generateStar(float radiusOuter = 1.0f, float radiusInner = 0.5f) {
    PointsAndIndices result;

    const int numPoints = 5;
    const float angleStep = glm::radians(360.0f / numPoints);

    // Generate positions
    for (int i = 0; i < numPoints; ++i) {
        float outerAngle = i * angleStep;
        float innerAngle = outerAngle + angleStep / 2.0f;

        // Outer point
        result.positions.push_back(radiusOuter * cos(outerAngle));
        result.positions.push_back(radiusOuter * sin(outerAngle));
        result.positions.push_back(0.0f); // Z-coordinate

        // Inner point
        result.positions.push_back(radiusInner * cos(innerAngle));
        result.positions.push_back(radiusInner * sin(innerAngle));
        result.positions.push_back(0.0f); // Z-coordinate
    }

    // Generate indices for drawing the star
    for (int i = 0; i < numPoints; ++i) {
        int outerIndex = i * 2;
        int innerIndex = outerIndex + 1;
        int nextOuterIndex = (outerIndex + 2) % (numPoints * 2);
        int nextInnerIndex = (innerIndex + 2) % (numPoints * 2);

        result.indices.push_back(outerIndex);
        result.indices.push_back(innerIndex);
        result.indices.push_back(nextOuterIndex);

        result.indices.push_back(innerIndex);
        result.indices.push_back(nextInnerIndex);
        result.indices.push_back(nextOuterIndex);
    }

    return result;
}

PointsAndIndices generateHouse() {
    std::vector<float> positions = {
        // positions        // colors (R, G, B)
        // Walls
       -0.5f,  -0.5f,      1.0f, 1.0f, 0.0f,
        0.5f,  -0.5f,      1.0f, 1.0f, 0.0f,
        0.5f,   0.5f,      1.0f, 1.0f, 0.0f,
       -0.5f,   0.5f,      1.0f, 1.0f, 0.0f,
        // Roof
        0.5f,   0.5f,      1.0f, 0.0f, 0.0f,
       -0.5f,   0.5f,      1.0f, 0.0f, 0.0f,
        0.0f,   0.8f,      1.0f, 0.0f, 0.0f,
        // Door
       -0.1f,  -0.5f,      0.0f, 0.0f, 1.0f,
        0.1f,  -0.5f,      0.0f, 0.0f, 1.0f,
        0.1f,   0.1f,      0.0f, 0.0f, 1.0f,
       -0.1f,   0.1f,      0.0f, 0.0f, 1.0f,
        // Grass
       -1.0f,  -0.5f,      0.0f, 1.0f, 0.0f,
        1.0f,  -0.5f,      0.0f, 1.0f, 0.0f,
        1.0f,  -1.0f,      0.0f, 1.0f, 0.0f,
       -1.0f,  -1.0f,      0.0f, 1.0f, 0.0f,
    };

    std::vector<unsigned int> indices = {
       0, 1, 2, 2, 3, 0,
       4, 5, 6,
       7, 8, 9, 9, 10, 7,
       11, 12, 13, 13, 14, 11
    };

    return { positions, indices };
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello! :)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "error :(" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    PointsAndIndices data = generateStar(1.0f);
    std::vector<float> positions = data.positions;
    std::vector<unsigned int> indices = data.indices;

    GLuint VAO, VBO, EBO;

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    // Generate and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Define position attribute (assuming 3 floats per position: x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and use shader program
    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // Draw the star
        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}