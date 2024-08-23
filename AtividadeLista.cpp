// Includes and setup code remain the same

#include <iostream>
#include <vector>
#include <cmath> // For trigonometric functions

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char* TITLE = { "star" };
GLFWwindow* gWindow = NULL;

// Define PI manually
const float PI = 3.14159265358979f;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"\n"
"out vec3 vert_color;"
"void main()"
"{"
"   vert_color = color;"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"
"void main()"
"{"
"    frag_color = vec4(vert_color, 1.0f);"
"}";

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime);
void Spin(std::vector<float>& points, float elapsedTime);
bool initOpenGL();

enum State {
    SPINNING,
    BOUNCING,
    IDLE
};

State currentState = BOUNCING; // Start with bouncing
float stateStartTime = glfwGetTime(); // Track when the current state started

int main(void)
{
    if (!initOpenGL())
    {
        std::cerr << "Initialization failed" << std::endl;
        return -1;
    }

    GLuint vao, vbo;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fs);

    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    
    std::vector<float> frozenPoints;
    float currentAngle = 0.0f;

    bool bouncing = true;
    bool spinning = false;

    while (!glfwWindowShouldClose(gWindow))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<float> points = {
            0.0f,  0.42f,  0.0f,     1.0f, 1.0f, 0.0f, // Top point
           -0.28f, -0.375f,  0.0f,     1.0f, 1.0f, 0.0f, // Bottom left
            0.42f,  0.135f,  0.0f,     1.0f, 1.0f, 0.0f, // Right top
           -0.42f,  0.135f,  0.0f,     1.0f, 1.0f, 0.0f, // Left top
            0.28f, -0.375f,  0.0f,     1.0f, 1.0f, 0.0f, // Bottom right
            0.0f,  0.42f,  0.0f,     1.0f, 1.0f, 0.0f, // Top point
        };

        frozenPoints.resize(points.size(), 0.0f);

        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - stateStartTime;

        switch (currentState)
        {
        case BOUNCING:
            Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
            if (elapsedTime >= 4.8f) // Bounce for 5 seconds
            {
                currentState = SPINNING;
                stateStartTime = currentTime; // Reset start time for spinning
            }
            break;

        case SPINNING:
            points = frozenPoints;
            Spin(points, elapsedTime);
            if (elapsedTime >= 3.0f) // Spin for 6 seconds
            {
                frozenPoints = points;
                currentState = IDLE;
                stateStartTime = currentTime; // Reset start time
            }
            break;

        case IDLE:
            // Use the stored positions, effectively freezing the star
            points = frozenPoints;
            if (elapsedTime >= 2.0f) 
            {
                currentState = SPINNING;
                stateStartTime = currentTime; // Reset start time
            }
            break;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_DYNAMIC_DRAW);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_LOOP, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow);
    }

    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime)
{
    float bounce = glm::abs(glm::sin(elapsedTime * 2.0f));
    for (int i = 1; i < 32; i += 6)
    {
        points[i] += bounce * 0.2f; // Adjust y-coordinate of Bottom left
    }
}

void Spin(std::vector<float>& points, float elapsedTime) {
    // Define the spinning speed (degrees per second)
    const float spinSpeed = -30.0f;

    // Calculate the angle for this frame based on spinSpeed and elapsedTime
    float angle = spinSpeed * elapsedTime;

    // Limit the angle to a specific range to avoid overflow and excessive spin speed
    if (angle > 360.0f) {
        angle = fmod(angle, 360.0f);
    }

    // Rotation matrix for rotating around the z-axis
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Movement in the x-axis
    float moveX = glm::sin(elapsedTime * 0.3f); // Smooth x-axis movement

    // Translation matrix to move the star in the x direction
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(moveX, 0.0f, 0.0f));

    // Combine translation and rotation transformations
    glm::mat4 transform = translation * rotation;

    // Apply transformation to each vertex
    for (int i = 0; i < points.size(); i += 6) {
        glm::vec4 pos = glm::vec4(points[i], points[i + 1], 0.0f, 1.0f); // z-coordinate is 0.0f
        pos = transform * pos; // Apply transformation
        points[i] = pos.x;
        points[i + 1] = pos.y;
    }
}


bool initOpenGL()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}
