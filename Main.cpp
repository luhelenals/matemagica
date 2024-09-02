// Includes and setup code remain the same
#include <iostream>
#include <vector>
#include <cmath> // For trigonometric functions
#include "shaderFunctions.cpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
//#define GLEW_STATIC

// Variáveis globais
const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char* TITLE = { "Matemagica" };
GLFWwindow* gWindow = NULL;
const float PI = 3.14159265358979f;

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime);
void Spin(std::vector<float>& points, float elapsedTime, float direction, glm::vec3(center));
void CreateDetachingLines(std::vector<float>& points, int point1, int point2, float y1, float y2);
void DrawDetachedLines(GLuint& linesVBO, GLuint& linesVAO, GLuint shader, const std::vector<float>& detachedLinesPoints);
void Rotate(std::vector<float>& object1, std::vector<float>& object2, float elapsedTime, glm::vec3 center1, glm::vec3 center2);
bool initOpenGL();
std::vector<float> generatePentagram(float radius, glm::vec2 center);
glm::vec2 GetIntersectionPoint(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d);
std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangles(std::vector<float>& points);
void DrawShape(unsigned int vbo, unsigned int vao, unsigned int shader, std::vector<float>& points, int mode, int count);
std::vector<float> ChangeColor(std::vector<float> points, int nVertex, float r, float g, float b);
std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangleLines(std::vector<float>& points); 
std::vector<float> GenerateRectangle(std::vector<float> l1, std::vector<float> l2, std::vector<float> l3);
glm::vec3 CalculateCenter(const std::vector<float>& vertices);

enum State {
    SPINNING,
    BOUNCING,
    IDLE,
    ROTATING,
    MAKINGRECTANGLE,
    RECTANGLE,
    TRIANGLE,
    STOPPED
};

State currentState = BOUNCING; // Start with bouncing
float stateStartTime = glfwGetTime(); // Track when the current state started
std::vector<float> detachedLinesPoints;

int main(void)
{
    if (!initOpenGL())
    {
        std::cerr << "Initialization failed" << std::endl;
        return -1;
    }
    float lineWidthRange[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
    std::cout << "Supported line width range: " << lineWidthRange[0] << " - " << lineWidthRange[1] << std::endl;


    ShaderProgramSource source = ParseShader("res/shaders/star.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource, source.GeometrySource);

    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    GLuint linesVBO, linesVAO;
    glGenBuffers(1, &linesVBO);
    glGenVertexArrays(1, &linesVAO);
        
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

    bool bouncing = true;
    bool spinning = false;
    bool hasSpinned = false;
    bool spinningForward = true;
    
    std::vector<float> actualPoints;
    std::vector<float> frozenPoints;
    std::vector<float> rectangle;
    std::vector<float> line;

    while (!glfwWindowShouldClose(gWindow))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<float> points = generatePentagram(0.3f, glm::vec2(0.0f, 0.0f));
        std::vector<float> triangleDC1, triangleAB2, triangleAE3;
        std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(points);
        std::vector<float> triangle2 = ChangeColor(triangleDC1, 3, 0.7890625f, 0.01953125f, 0.453125f);
        std::vector<float> l1, l2, l3;
        std::tie(l1, l2, l3) = GetTriangleLines(triangleDC1);

        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - stateStartTime;

        switch (currentState)
        {
        case BOUNCING:
            Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
            if (elapsedTime >= 4.7f) // Bounce for 5 seconds
            {
                if (hasSpinned) {
                    currentState = ROTATING;
                    stateStartTime = currentTime; // Reset start time
                }
                else {
                    //frozenPoints = points;
                    currentState = SPINNING;
                    stateStartTime = currentTime; // Reset start time
                }
            }
            if (hasSpinned)
                points = ChangeColor(points, 5, 1.0f, 0.21484375f, 0.62890625f);
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            break;

        case SPINNING:
            glm::vec3 center = CalculateCenter(points);
            Spin(points, elapsedTime, (spinningForward ? 1.0f : -1.0f), (spinningForward ? center : CalculateCenter(actualPoints)));
            if (elapsedTime >= 10.0f && !spinningForward) // Spin for 6 seconds
            {
                currentState = ROTATING;
                stateStartTime = currentTime; // Reset start time
                hasSpinned = true;
            }
            else if (abs(points[12] - points[18] <= 0.001) && spinningForward)
            {
                actualPoints = points;
                currentState = IDLE;
                spinningForward = false;
                stateStartTime = currentTime;
            }

            if (abs(points[0] - points[6]) <= 0.001 && points[0] > 0.4 && !spinningForward && detachedLinesPoints.size() < 12)
            {
                CreateDetachingLines(points, 0, 6, points[1], points[7]);
            }

            if (abs(points[12] - points[18]) <= 0.001 && points[0] > 0.0 && points[0] < 0.5  && !spinningForward && detachedLinesPoints.size() < 24)
            {
                CreateDetachingLines(points, 12, 18, triangleDC1[7], points[13]);
            }

            if (abs(points[24] - points[0]) <= 0.001 && points[0] > -0.5 && !spinningForward && detachedLinesPoints.size() < 36)
            {
                CreateDetachingLines(points, 24, 0, triangleAB2[13], points[25]);
            }

            if (abs(points[6] - points[12]) <= 0.001 && points[0] < -0.0 && !spinningForward && detachedLinesPoints.size() < 48)
            {
                CreateDetachingLines(points, 6, 12, triangleAB2[13], triangleDC1[7]);
            }
            
            DrawDetachedLines(linesVBO, linesVAO, shader, detachedLinesPoints);
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            break;

        case IDLE:
            if (elapsedTime >= 1.0f)// 2.0f) // Stay put seconds
            {
                currentState = SPINNING;
                stateStartTime = currentTime; // Reset start time
            }
            DrawShape(vbo, vao, shader, frozenPoints, GL_LINE_LOOP, 6);
            break;

        case ROTATING:
            Rotate(triangleAE3, triangleAB2, elapsedTime, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            if (abs(triangleAE3[12] - triangleAB2[12]) < 0.001f && abs(triangleAE3[13] - triangleAB2[13]) < 0.001f)
            {
                currentState = TRIANGLE;
                stateStartTime = currentTime; // Reset start time
            }
            
            DrawShape(vbo, vao, shader, triangleDC1, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, triangleAB2, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, triangleAE3, GL_LINE_LOOP, 3);

            break;

        case TRIANGLE:
            if (elapsedTime >= 1.0f){//1.0f) {// Stay put seconds
                
                DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, triangleDC1, GL_LINE_LOOP, 3);

                if (elapsedTime >= 3.0f) {
                    currentState = MAKINGRECTANGLE;
                    stateStartTime = currentTime; // Reset start time
                }
            } else {
                DrawShape(vbo, vao, shader, triangleDC1, GL_LINE_LOOP, 3);
            }
            break;

        case MAKINGRECTANGLE:
            DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);
            Rotate(l3, l2, elapsedTime, glm::vec3(l3[0], l3[1], l3[2]), glm::vec3(l2[0], l2[1], l2[2]));
            if (abs(l1[0] - l2[6]) < 0.001f && abs(l1[6] - l3[6]) < 0.001f) {
                currentState = RECTANGLE;
                stateStartTime = currentTime;   
                rectangle = GenerateRectangle(l1, l2, l3);
            }
            DrawShape(vbo, vao, shader, l1, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l2, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l3, GL_LINES, 2);
            break;

        case RECTANGLE:            
            if (elapsedTime > 0.5f) {
                rectangle = ChangeColor(rectangle, 6, 1.0f, 0.658823f, 0.490196f);
                std::vector<float> rectangle2 = rectangle;
                rectangle2 = ChangeColor(rectangle2, 6, 0.741176f, 0.1215686f, 0.0f);
                DrawShape(vbo, vao, shader, rectangle2, GL_TRIANGLES, 6);
                if (elapsedTime > 5.0f) {
                    currentState = STOPPED;
                    stateStartTime = currentTime;
                }
            }
            else DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);
    
            DrawShape(vbo, vao, shader, rectangle, GL_LINE_LOOP, 6);
        case STOPPED:
            
            break;
        }

        glBindVertexArray(0);
        glfwSwapBuffers(gWindow);
    }

    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime)
{
    float bounce = glm::abs(glm::sin(elapsedTime * 4.0f));

    for (int i = 1; i < 32; i += 6)
    {
        points[i] += bounce * 0.2f; // Adjust y-coordinate of Bottom left
    }
}

void Spin(std::vector<float>& points, float elapsedTime, float direction, glm::vec3 center)
{
    // Ensure direction is either -1 or 1
    direction = (direction < 0) ? -1.0f : 1.0f;

    // Calculate the rotation angle based on the direction
    
    float angle = elapsedTime * direction * -50.0f; // Spinning speed with direction
   
    if (direction == -1) {
        angle += 198;
        if (angle >= 360) angle -= 360;
    }

    // Apply rotation around the z-axis (ensuring no unintended z-axis movement)
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Calculate the movement in the x-axis
    float moveX = elapsedTime * 0.2f * direction; // Scale movement by some factor

    // Move the star back to its original center plus the x-axis translation
    glm::mat4 toNewCenter = glm::translate(glm::mat4(1.0f), glm::vec3(center.x + moveX, 0.0f, 0.0f));

    // Combine the transformations: move to origin, rotate, and then move to the new center
    glm::mat4 transform = toNewCenter * rotation;

    // Apply the transformation to each vertex
    for (int i = 0; i < points.size(); i += 6)
    {
        glm::vec4 pos = glm::vec4(points[i], points[i + 1], 0.0f, 1.0f); // z-coordinate is 0.0f
        pos = transform * pos; // Apply the transformation
        points[i] = pos.x;
        points[i + 1] = pos.y;
    }
}

void CreateDetachingLines(std::vector<float>& points, int point1, int point2, float y1, float y2)
{
    std::cout << "Entrou" << points[0] << std::endl;
    // Only create a line after enough time has passed.
    if (detachedLinesPoints.size() < 30 * 6) // Assume 5 lines with 6 vertices each
    {
        // Create a vertical line segment at the star's current position.
        detachedLinesPoints.insert(detachedLinesPoints.end(),
            {
                points[point1], y1, 0.0f, 1.0f, 1.0f, 1.0f, // First vertex of the line
                points[point2], y2, 0.0f, 1.0f, 1.0f, 1.0f  // Second vertex of the line
            });
    }
}

void DrawDetachedLines(GLuint& linesVBO, GLuint& linesVAO, GLuint shader, const std::vector<float>& detachedLinesPoints)
{
    // Bind the VBO and update it with the current line points.
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, detachedLinesPoints.size() * sizeof(float), detachedLinesPoints.data(), GL_DYNAMIC_DRAW);

    // Use the provided shader.
    glUseProgram(shader);

    // Bind the VAO and set up the vertex attributes for the line data.
    glBindVertexArray(linesVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    // Draw the detached lines.
    glDrawArrays(GL_LINES, 0, detachedLinesPoints.size() / 6);

    // Unbind VAO and shader program after drawing.
    glBindVertexArray(0);
    glUseProgram(0);
}

void Rotate(std::vector<float>& object1, std::vector<float>& object2, float elapsedTime, glm::vec3 center1, glm::vec3 center2) {
    float angleDegrees = elapsedTime * -40.0f;
    // Converte o ângulo de graus para radianos
    float angleRadians = glm::radians(angleDegrees);

    // Cria a matriz de rotação para o objeto 1 (horário)
    glm::mat4 rotationMatrix1 = glm::rotate(glm::mat4(1.0f), -angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Cria a matriz de rotação para o objeto 2 (anti-horário)
    glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Aplica a rotação aos vértices do objeto 1
    for (size_t i = 0; i < object1.size(); i += 6) {
        glm::vec4 vertex(object1[i], object1[i + 1], object1[i + 2], 1.0f);
        vertex -= glm::vec4(center1, 0.0f); // Translada para a origem
        vertex = rotationMatrix1 * vertex;  // Aplica a rotação
        vertex += glm::vec4(center1, 0.0f); // Translada de volta
        object1[i] = vertex.x;
        object1[i + 1] = vertex.y;
        object1[i + 2] = vertex.z;
    }

    for (size_t i = 0; i < object2.size(); i += 6) {
        glm::vec4 vertex2(object2[i], object2[i + 1], object2[i + 2], 1.0f);
        vertex2 -= glm::vec4(center2, 0.0f); // Translada para a origem
        vertex2 = rotationMatrix2 * vertex2;  // Aplica a rotação
        vertex2 += glm::vec4(center2, 0.0f); // Translada de volta
        object2[i] = vertex2.x;
        object2[i + 1] = vertex2.y;
        object2[i + 2] = vertex2.z;
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

void DrawShape(unsigned int vbo, unsigned int vao, unsigned int shader, std::vector<float>& points, int mode, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_DYNAMIC_DRAW);
    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, count);
}

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangleLines(std::vector<float>& points) {
    std::vector<float> l1 = {
        points[0], points[1], points[2], points[3], points[4], points[5],
        points[6], points[7], points[8], points[9], points[10], points[11]
    };

    std::vector<float> l2 = {
        points[0], points[1], points[2], points[3], points[4], points[5],
        points[12], points[13], points[14], points[15], points[16], points[17]
    };

    std::vector<float> l3 = {
        points[6], points[7], points[8], points[9], points[10], points[11],
        points[12], points[13], points[14], points[15], points[16], points[17]
    };

    return std::make_tuple(l1, l2, l3);
}

std::vector<float> ChangeColor(std::vector<float> points, int nVertex, float r, float g, float b) {
    // Determine o tamanho do bloco (6 ou 7) dependendo se "a" existe
    size_t blockSize = (points.size() % 7 == 0) ? 7 : 6;

    // Itera sobre o vetor ajustando as posições de r, g, b
    for (size_t i = 3; i < points.size(); i += blockSize) {
        points[i] = r;     // Define o valor de r
        points[i + 1] = g; // Define o valor de g
        points[i + 2] = b; // Define o valor de b
        // Se blockSize for 7, points[i + 3] será o valor 'a', que não é alterado
    }
    return points;
}

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangles(std::vector<float>& points) {
    // Pontos da estrela
    glm::vec2 a(points[0], points[1]);
    glm::vec2 b(points[6], points[7]);
    glm::vec2 c(points[12], points[13]);
    glm::vec2 d(points[18], points[19]);
    glm::vec2 e(points[24], points[25]);

    glm::vec2 intersection1 = GetIntersectionPoint(d, e, b, c); // triangle dc1
    glm::vec2 intersection2 = GetIntersectionPoint(b, c, a, e); // triangle ab2
    glm::vec2 intersection3 = GetIntersectionPoint(d, e, a, b); // triangle ae3

    std::vector<float> triangleDC1 = {
        d[0], d[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        c[0], c[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        intersection1[0], intersection1[1], 0.0f,  1.0f, 0.21484375f, 0.62890625f,
    };

    std::vector<float> triangleAB2 = {
        a[0], a[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        b[0], b[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        intersection2[0], intersection2[1], 0.0f,  1.0f, 0.21484375f, 0.62890625f,
    };

    std::vector<float> triangleAE3 = {
        a[0], a[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        e[0], e[1], 0.0f,                          1.0f, 0.21484375f, 0.62890625f,
        intersection3[0], intersection3[1], 0.0f,  1.0f, 0.21484375f, 0.62890625f,
    };

    return std::make_tuple(triangleDC1, triangleAB2, triangleAE3);
}

glm::vec3 CalculateCenter(const std::vector<float>& vertices) {
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    int vertexCount = vertices.size() / 6; // Assuming each vertex has x, y, z, r, g, b

    for (int i = 0; i < vertexCount; ++i) {
        center.x += vertices[i * 6 + 0];
        center.y += vertices[i * 6 + 1];
    }

    center /= static_cast<float>(vertexCount);
    center.z = 1.0f;
    return center;
}


// Function to generate the points of the star
std::vector<float> generatePentagram(float radius, glm::vec2 center) {
    std::vector<float> points;
    const int numPoints = 5;
    const float angleIncrement = 2.0f * PI / numPoints;
    std::vector<int> order = { 0, 2, 4, 1, 3, 0 }; // Ordem desejada dos pontos

    std::vector<glm::vec2> tempPoints(numPoints);

    // Gerar os pontos normalmente
    for (int i = 0; i < numPoints; ++i) {
        float angle = i * angleIncrement + PI/2; // Começar no topo (-PI/2 radianos)
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        tempPoints[i] = glm::vec2(x, y);
    }

    // Adicionar os pontos na ordem especificada com z = 0.0f e cor (1.0f, 1.0f, 0.0f)
    for (int idx : order) {
        points.push_back(tempPoints[idx].x);
        points.push_back(tempPoints[idx].y);
        points.push_back(0.0f);       // z-coordinate
        points.push_back(1.0f);       // Red component of color
        points.push_back(1.0f);       // Green component of color
        points.push_back(0.0f);       // Blue component of color
    }

    return points;
}

std::vector<float> GenerateRectangle(std::vector<float> l1, std::vector<float> l2, std::vector<float> l3) {
    
    std::vector<float> points = {
    l1[0], l1[1], l1[2], 1.0f, 0.21484375f, 0.62890625f, // top left
    l1[6], l1[7], l1[8], 1.0f, 0.21484375f, 0.62890625f, // top right
    l3[6], l3[7], l3[8], 1.0f, 0.21484375f, 0.62890625f, // bottom right

    l3[6], l3[7], l3[8], 1.0f, 0.21484375f, 0.62890625f, // bottom right
    l2[6], l2[7], l2[8], 1.0f, 0.21484375f, 0.62890625f,  // bottom left
    l1[0], l1[1], l1[2], 1.0f, 0.21484375f, 0.62890625f, // top left
    };

    return points;
}

// Function to get the intersection point
glm::vec2 GetIntersectionPoint(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d) {
    float t = ((c.x - a.x) * (d.y - c.y) - (c.y - a.y) * (d.x - c.x)) /
        ((b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x));

    glm::vec2 intersection;
    intersection.x = a.x + t * (b.x - a.x);
    intersection.y = a.y + t * (b.y - a.y);

    return intersection;
}