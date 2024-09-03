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

enum State {
    SPINNING,
    BOUNCING,
    IDLE,
    LINES,
    ROTATING,
    MAKINGRECTANGLE,
    RECTANGLE,
    CHANGINGCOLOR,
    TRIANGLE,
    MAKINGTRIANGLE,
    STOPPED,
    PENTAGON
};

enum Color {
    YELLOW,
    LIGHTPINK,
    PINK,
    LIGHTORANGE,
    ORANGE,
    DARKORANGE,
    LIGHTBLUE,
    BLUE
};

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime);
void Spin(std::vector<float>& points, float elapsedTime, float direction, glm::vec3(center));
void CreateDetachingLines(float x1, float x2, float y1, float y2);
void DrawDetachedLines(GLuint& linesVBO, GLuint& linesVAO, GLuint shader, const std::vector<float>& detachedLinesPoints);
void MoveLineToPosition(std::vector<float>& detachedLinesPoints, int i, float targetX1, float targetX2, float targetY1, float targetY2, float step);
void Rotate(std::vector<float>& object1, std::vector<float>& object2, float elapsedTime, glm::vec3 center1, glm::vec3 center2);
bool initOpenGL();
std::vector<float> generatePentagram(float radius, glm::vec2 center);
glm::vec2 GetIntersectionPoint(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d);
std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangles(std::vector<float>& points);
void DrawShape(unsigned int vbo, unsigned int vao, unsigned int shader, std::vector<float>& points, int mode, int count);
std::vector<float> ChangeColor(std::vector<float> points, Color color);
std::tuple<std::vector<float>, std::vector<float>, std::vector<float>> GetTriangleLines(std::vector<float>& points);
std::vector<float> GenerateRectangle(std::vector<float> l1, std::vector<float> l2, std::vector<float> l3);
glm::vec3 CalculateCenter(const std::vector<float>& vertices);
void ScaleObject(std::vector<float>& vertices, float elapsedTime);
std::vector<float> GeneratePentagon(std::vector<float> points);

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
    bool hasRectangled = false;
    bool hasStarred = false;
    bool condition = true;
    float step = 0.0015f;
    int qtdLines = 0;
    int i = 1;

    std::vector<float> actualPoints;
    std::vector<float> frozenPoints;
    std::vector<float> rectangle;
    std::vector<float> line;
    std::vector<float> l1, l2, l3, line1, line2, line3;
    std::vector<float> triangleDC1, triangleAB2, triangleAE3;
    std::vector<float> pentagon;
    pentagon.resize(30);
    line1.resize(12);
    line2.resize(12);
    line3.resize(12);
    l1.resize(12);           // Agora l1 tem 12 elementos com valor 0.0f
    l2.resize(12);           // Agora l2 tem 12 elementos com valor 0.0f
    l3.resize(12);           // Agora l3 tem 12 elementos com valor 0.0f
    triangleDC1.resize(18);  // Agora triangleDC1 tem 18 elementos com valor 0.0f
    triangleAB2.resize(18);  // Agora triangleAB2 tem 18 elementos com valor 0.0f
    triangleAE3.resize(18);  // Agora triangleAE3 tem 18 elementos com valor 0.0f*/

    while (!glfwWindowShouldClose(gWindow))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<float> points = generatePentagram(0.3f, glm::vec2(0.0f, 0.0f));
        if (hasRectangled)
            pentagon = GeneratePentagon(points);
        std::vector<float> triangle2 = ChangeColor(triangleDC1, PINK);
        if (!hasRectangled)
            std::tie(l1, l2, l3) = GetTriangleLines(triangleDC1);
        else {
            l1 = line1;
            l2 = line2;
            l3 = line3;
        }

        std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(points);
        std::vector<float> t1 = triangleDC1, t2 = triangleDC1, t3 = triangleDC1;

        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - stateStartTime;

        switch (currentState) {
        case BOUNCING:
            Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
            if (hasRectangled)
            {
                Bounce(triangleAB2, elapsedTime, bouncing, spinning, stateStartTime);
                Bounce(triangleDC1, elapsedTime, bouncing, spinning, stateStartTime);
                Bounce(triangleAE3, elapsedTime, bouncing, spinning, stateStartTime);
            }
            if (elapsedTime >= 0.7f) // Bounce for 5 seconds
            {
                std::cout << "State: BOUNCING" << std::endl;
                if (hasSpinned && !hasRectangled)
                    currentState = ROTATING;

                else if (!hasSpinned)
                    currentState = SPINNING;

                else if (hasSpinned && hasRectangled)
                    currentState = PENTAGON;

                stateStartTime = currentTime; // Reset start time
            }
            if (hasSpinned && !hasRectangled)
                points = ChangeColor(points, LIGHTPINK);
            else if (hasRectangled)
            {
                points = ChangeColor(points, LIGHTBLUE);
                triangleAB2 = ChangeColor(triangleAB2, BLUE);
                triangleDC1 = ChangeColor(triangleDC1, BLUE);
                triangleAE3 = ChangeColor(triangleAE3, BLUE);

                DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);
            }
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            break;

        case SPINNING:
            step = 1;
            glm::vec3 center = CalculateCenter(points);
            Spin(points, elapsedTime, (spinningForward ? 1.0f : -1.0f), (spinningForward ? center : CalculateCenter(actualPoints)));
            if (elapsedTime >= 10.0f && !spinningForward) // Spin for 6 seconds
            {
                currentState = LINES;
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

            glm::vec2 a(points[0], points[1]);
            glm::vec2 b(points[6], points[7]);
            glm::vec2 c(points[12], points[13]);
            glm::vec2 d(points[18], points[19]);
            glm::vec2 e(points[24], points[25]);

            glm::vec2 intersection2 = GetIntersectionPoint(d, e, a, b);
            glm::vec2 intersection3 = GetIntersectionPoint(d, e, c, b);
            glm::vec2 intersection4 = GetIntersectionPoint(d, c, a, e);
            glm::vec2 intersection5 = GetIntersectionPoint(b, a, d, c);

            if (spinningForward) DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            else {
                if (detachedLinesPoints.size() < 12) {
                    CreateDetachingLines(points[12], points[18], points[13], points[19]);
                    CreateDetachingLines(points[24], intersection4[0], points[25], intersection4[1]);
                    CreateDetachingLines(points[6], intersection3[0], points[7], intersection3[1]);
                    CreateDetachingLines(intersection3[0], intersection2[0], intersection3[1], intersection2[1]);
                    CreateDetachingLines(points[0], points[6], points[1], points[7]);
                    CreateDetachingLines(points[0], intersection4[0], points[1], intersection4[1]);
                    CreateDetachingLines(points[12], intersection3[0], points[13], intersection3[1]);
                    CreateDetachingLines(points[18], intersection2[0], points[19], intersection2[1]);
                    CreateDetachingLines(points[24], intersection3[0], points[25], intersection3[1]);
                }
                else {
                    if (qtdLines < 1 && abs(points[0] - points[6]) >= 0.001 && points[0] < 0.4)
                    {
                        MoveLineToPosition(detachedLinesPoints, 0, points[12], points[18], points[13], points[19], step);
                    }
                    else if (qtdLines < 1) qtdLines++;

                    if (qtdLines < 2 && abs(points[24] - points[0]) >= 0.001) {
                        MoveLineToPosition(detachedLinesPoints, 12, points[24], intersection4[0], points[25], intersection4[1], step);
                    } 
                    else if (qtdLines < 2) qtdLines++;

                    if (qtdLines < 3 && abs(points[6] - points[12]) >= 0.001) {
                        MoveLineToPosition(detachedLinesPoints, 24, points[6], intersection3[0], points[7], intersection3[1], step);
                    }
                    else if (qtdLines < 3 && qtdLines > 1) qtdLines++;
                    
                    if (qtdLines < 4 && abs(points[24] - points[18]) >= 0.001) {
                        MoveLineToPosition(detachedLinesPoints, 36, intersection3[0], intersection2[0], intersection3[1], intersection2[1], step);
                    }
                    else if (qtdLines < 4 && qtdLines > 2) qtdLines++;
                    
                    MoveLineToPosition(detachedLinesPoints, 48, points[0], points[6], points[1], points[7], step);
                    MoveLineToPosition(detachedLinesPoints, 60, points[0], intersection4[0], points[1], intersection4[1], step);
                    MoveLineToPosition(detachedLinesPoints, 72, points[12], intersection3[0], points[13], intersection3[1], step);
                    MoveLineToPosition(detachedLinesPoints, 84, points[18], intersection2[0], points[19], intersection2[1], step);
                    MoveLineToPosition(detachedLinesPoints, 96, points[24], intersection3[0], points[25], intersection3[1], step);
                }
            }

            DrawDetachedLines(linesVBO, linesVAO, shader, detachedLinesPoints);
            
            break;

        case IDLE:
            if (elapsedTime >= 1.0f)// 2.0f) // Stay put seconds
            {
                currentState = SPINNING;
                stateStartTime = currentTime; // Reset start time
            }
            if (hasRectangled)
            { 
                points = ChangeColor(points, LIGHTBLUE);
                triangleAB2 = ChangeColor(triangleAB2, BLUE);
                triangleDC1 = ChangeColor(triangleDC1, BLUE);
                triangleAE3 = ChangeColor(triangleAE3, BLUE);

                DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);

                currentState = BOUNCING;
            }
            DrawShape(vbo, vao, shader, actualPoints, GL_LINE_LOOP, 6);
            break;

        case LINES:
            step = 0.0017;
            if (elapsedTime < 3.0f) {
                MoveLineToPosition(detachedLinesPoints, 36, detachedLinesPoints[36], detachedLinesPoints[36], detachedLinesPoints[42] / 2 + 0.015f, detachedLinesPoints[1], step);
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[36], detachedLinesPoints[42], detachedLinesPoints[37], detachedLinesPoints[43], 1);
            }
            if (elapsedTime > 3.0f && elapsedTime < 5.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[24], detachedLinesPoints[24], detachedLinesPoints[19], detachedLinesPoints[31], step);
                frozenPoints = detachedLinesPoints;
            }
            if (elapsedTime > 5.0f && elapsedTime < 7.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[12] - 0.02f, detachedLinesPoints[12] - 0.02f, detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 24, detachedLinesPoints[12] - 0.02f, detachedLinesPoints[12] - 0.02f, detachedLinesPoints[25], detachedLinesPoints[31], step);
            }
            if (elapsedTime > 7.0f && elapsedTime < 9.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[48], frozenPoints[48], detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 24, frozenPoints[24], frozenPoints[24], detachedLinesPoints[25], detachedLinesPoints[31], step);
            }
            if (elapsedTime > 9.0f && elapsedTime < 10.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[24], frozenPoints[24], detachedLinesPoints[25], detachedLinesPoints[31], 1);
            }
            if (elapsedTime > 10.0f && elapsedTime < 12.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[12], detachedLinesPoints[12], detachedLinesPoints[19], detachedLinesPoints[7], step);
                frozenPoints = detachedLinesPoints;
            }
            if (elapsedTime > 12.0f && elapsedTime < 14.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[0] - 0.02, detachedLinesPoints[0] - 0.02, detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 12, detachedLinesPoints[0] - 0.02, detachedLinesPoints[0] - 0.02, detachedLinesPoints[13], detachedLinesPoints[19], step);
            }
            if (elapsedTime > 14.0f && elapsedTime < 16.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[48], frozenPoints[48], detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 12, frozenPoints[12], frozenPoints[12], detachedLinesPoints[13], detachedLinesPoints[19], step);
            }
            if (elapsedTime > 16.0f && elapsedTime < 16.5f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[12], frozenPoints[12], detachedLinesPoints[13], detachedLinesPoints[19], 1);
                MoveLineToPosition(detachedLinesPoints, 60, detachedLinesPoints[0], detachedLinesPoints[6], detachedLinesPoints[1], detachedLinesPoints[7], 1);
            }
            if (elapsedTime > 16.5f && elapsedTime < 17.0f) {
                MoveLineToPosition(detachedLinesPoints, 72, detachedLinesPoints[24], detachedLinesPoints[30], detachedLinesPoints[25], detachedLinesPoints[31], 1);
                MoveLineToPosition(detachedLinesPoints, 84, detachedLinesPoints[12], detachedLinesPoints[18], detachedLinesPoints[13], detachedLinesPoints[19], 1);
                MoveLineToPosition(detachedLinesPoints, 96, detachedLinesPoints[24], detachedLinesPoints[30], detachedLinesPoints[25], detachedLinesPoints[31], 1);
            }
            if (elapsedTime > 17.0f) {
                step = 0.001;

                glm::vec2 a(points[0], points[1]);
                glm::vec2 b(points[6], points[7]);
                glm::vec2 c(points[12], points[13]);
                glm::vec2 d(points[18], points[19]);
                glm::vec2 e(points[24], points[25]);

                glm::vec2 intersection4 = GetIntersectionPoint(d, c, a, e);
                glm::vec2 intersection5 = GetIntersectionPoint(b, a, d, c);

                MoveLineToPosition(detachedLinesPoints, 0, points[6], points[12], points[7], points[13], step);
                MoveLineToPosition(detachedLinesPoints, 12, intersection4[0], points[0], intersection4[1], points[1], step);
                MoveLineToPosition(detachedLinesPoints, 24, triangleAE3[12], points[18], triangleAE3[13], points[19], step);
                MoveLineToPosition(detachedLinesPoints, 36, intersection5[0], triangleAE3[12], intersection5[1], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 48, points[24], triangleAE3[12], points[25], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 60, points[18], points[12], points[19], points[13], step);
                MoveLineToPosition(detachedLinesPoints, 72, points[6], triangleAE3[12], points[7], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 84, points[24], intersection4[0], points[25], intersection4[1], step);
                MoveLineToPosition(detachedLinesPoints, 96, intersection5[0], points[0], intersection5[1], points[1], step);
            }
            if (elapsedTime < 22.0f) {
                DrawDetachedLines(linesVBO, linesVAO, shader, detachedLinesPoints);
            }
            else if (elapsedTime < 26.7f) {
                Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
                DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);

            }
            else {
                currentState = ROTATING;
                stateStartTime = currentTime;
            }
            break;

        case ROTATING:
            if (!hasRectangled) Rotate(triangleAE3, triangleAB2, elapsedTime, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            else {
                t1 = ChangeColor(t1, LIGHTBLUE);
                t2 = ChangeColor(t2, LIGHTBLUE);
                t3 = ChangeColor(t3, LIGHTBLUE);
                Rotate(t3, t2, elapsedTime, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
                triangle2 = ChangeColor(triangle2, BLUE);
                DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);
            }

            condition = hasRectangled
                ? (abs(points[0] - t3[6]) < 0.001f) && (abs(points[1] - t3[7]) < 0.001f)
                : (abs(triangleAE3[12] - triangleAB2[12]) < 0.001f && abs(triangleAE3[13] - triangleAB2[13]) < 0.001f);

            if (condition)
            {
                std::cout << "State: ROTATING" << std::endl;
                currentState = hasRectangled ? IDLE : TRIANGLE;
                if (hasRectangled) hasStarred = true;
                stateStartTime = currentTime; // Reset start time
            }

            if (!hasRectangled) {
                DrawShape(vbo, vao, shader, triangleDC1, GL_LINE_LOOP, 3);
                DrawShape(vbo, vao, shader, triangleAB2, GL_LINE_LOOP, 3);
                DrawShape(vbo, vao, shader, triangleAE3, GL_LINE_LOOP, 3);
            }
            else {
                DrawShape(vbo, vao, shader, t1, GL_LINE_LOOP, 3);
                DrawShape(vbo, vao, shader, t2, GL_LINE_LOOP, 3);
                DrawShape(vbo, vao, shader, t3, GL_LINE_LOOP, 3);
            }
            break;

        case TRIANGLE:
            if (hasRectangled) triangleDC1 = ChangeColor(triangleDC1, LIGHTORANGE);

            if (elapsedTime >= 1.0f) {// Stay put seconds
                if (hasRectangled)
                {
                    triangle2 = ChangeColor(triangle2, BLUE);
                    triangleDC1 = ChangeColor(triangleDC1, LIGHTBLUE);
                }
                DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);

                if (elapsedTime >= 2.0f) {
                    std::cout << "State: TRIANGLE" << std::endl;
                    currentState = hasRectangled ? ROTATING : MAKINGRECTANGLE;
                    stateStartTime = currentTime;

                    std::cout << "t2: " << std::endl;
                    std::cout << t2[0] << ", " << t2[1] << std::endl;
                    std::cout << t2[6] << ", " << t2[7] << std::endl;
                    std::cout << t2[12] << ", " << t2[13] << std::endl;

                    std::cout << "t3: " << std::endl;
                    std::cout << t3[0] << ", " << t3[1] << std::endl;
                    std::cout << t3[6] << ", " << t3[7] << std::endl;
                    std::cout << t3[12] << ", " << t3[13] << std::endl;

                    std::cout << "\n" << std::endl;
                }
            }

            DrawShape(vbo, vao, shader, triangleDC1, GL_LINE_LOOP, 3);

            break;

        case MAKINGRECTANGLE:
            DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);

            Rotate(l3, l2, elapsedTime, glm::vec3(l3[0], l3[1], l3[2]), glm::vec3(l2[0], l2[1], l2[2]));

            if (abs(l1[0] - l2[6]) < 0.001f && abs(l1[6] - l3[6]) < 0.001f) {
                std::cout << "State: MAKINGRECTANGLE" << std::endl;
                currentState = RECTANGLE;
                stateStartTime = currentTime;
                rectangle = GenerateRectangle(l1, l2, l3);
                hasRectangled = true;
                line1 = ChangeColor(l1, LIGHTORANGE);
                line2 = ChangeColor(l2, LIGHTORANGE);
                line3 = ChangeColor(l3, LIGHTORANGE);
            }
            DrawShape(vbo, vao, shader, l1, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l2, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l3, GL_LINES, 2);
            break;

        case RECTANGLE:
            if (elapsedTime > 0.5f) {
                rectangle = ChangeColor(rectangle, LIGHTORANGE);
                std::vector<float> rectangle2 = rectangle;

                if (i % 300)
                    rectangle2 = ChangeColor(rectangle2, DARKORANGE);
                else
                    rectangle2 = ChangeColor(rectangle2, ORANGE);

                i++;

                DrawShape(vbo, vao, shader, rectangle2, GL_TRIANGLES, 6);
                if (elapsedTime > 5.0f) {
                    i = 1;
                    std::cout << "State: RECTANGLE" << std::endl;

                    std::cout << "l2: " << std::endl;

                    std::cout << l2[6] << ", " << l2[7] << std::endl;
                    std::cout << "l3: " << std::endl;

                    std::cout << l3[6] << ", " << l3[7] << "/n" << std::endl;

                    currentState = MAKINGTRIANGLE;
                    stateStartTime = currentTime;

                }
            }
            else DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);

            DrawShape(vbo, vao, shader, rectangle, GL_LINE_LOOP, 6);
            break;

        case MAKINGTRIANGLE:
            Rotate(l2, l3, elapsedTime, glm::vec3(l2[0], l2[1], l2[2]), glm::vec3(l3[0], l3[1], l3[2]));
            if (abs(l3[6] - l2[6]) < 0.01f && abs(l2[7] - l3[7]) < 0.01f && elapsedTime > 0.2f) {
                currentState = TRIANGLE;
                stateStartTime = currentTime;
            }

            DrawShape(vbo, vao, shader, l1, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l2, GL_LINES, 2);
            DrawShape(vbo, vao, shader, l3, GL_LINES, 2);

            break;

        case PENTAGON:
            ScaleObject(points, elapsedTime);
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            if (elapsedTime > 2.0f) {
                DrawShape(vbo, vao, shader, pentagon, GL_POINTS, 5);
            }

            break;

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

void ScaleObject(std::vector<float>& vertices, float elapsedTime) {
    // Calcula o centro do objeto
    glm::vec3 center(0.0f);
    for (size_t i = 0; i < vertices.size(); i += 6) {
        center.x += vertices[i];
        center.y += vertices[i + 1];
        center.z += vertices[i + 2];
    }
    center /= (vertices.size() / 6);

    // Calcula o fator de escala baseado no tempo decorrido
    float scale = 1.0f - (0.1 * elapsedTime);

    // Escala cada ponto em relação ao centro
    for (size_t i = 0; i < vertices.size(); i += 6) {
        vertices[i] = center.x + (vertices[i] - center.x) * scale;
        vertices[i + 1] = center.y + (vertices[i + 1] - center.y) * scale;
        vertices[i + 2] = center.z + (vertices[i + 2] - center.z) * scale;
    }
}

std::vector<float> GeneratePentagon(std::vector<float> points) {
    glm::vec2 a = glm::vec2(points[0], points[1]);
    glm::vec2 b = glm::vec2(points[7], points[8]);
    glm::vec2 c = glm::vec2(points[14], points[15]);
    glm::vec2 d = glm::vec2(points[21], points[22]);
    glm::vec2 e = glm::vec2(points[28], points[29]);

    glm::vec2 p1(GetIntersectionPoint(a, e, c, d));
    glm::vec2 p2(GetIntersectionPoint(a, e, c, b));
    glm::vec2 p3(GetIntersectionPoint(b, c, e, d));
    glm::vec2 p4(GetIntersectionPoint(a, b, e, d));
    glm::vec2 p5(GetIntersectionPoint(a, b, c, d));

    std::vector<float> pentagon = ChangeColor(std::vector<float>({
        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p4.x, p4.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f
        }), PINK);

    std::cout << "pentagon:" << std::endl;
    std::cout << pentagon[0] << ", " << pentagon[1] << std::endl;
    std::cout << pentagon[6] << ", " << pentagon[7] << std::endl;
    std::cout << pentagon[12] << ", " << pentagon[13] << std::endl;
    std::cout << pentagon[18] << ", " << pentagon[19] << std::endl;
    std::cout << pentagon[24] << ", " << pentagon[20] << "\n" << std::endl;

    return pentagon;
}

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime)
{
    float bounce = glm::abs(glm::sin(elapsedTime * 4.0f));

    for (int i = 1; i < points.size(); i += 6)
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
        angle -= 89;
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

void CreateDetachingLines(float x1, float x2, float y1, float y2)
{
    // Only create a line after enough time has passed.
    if (detachedLinesPoints.size() < 30 * 6) // Assume 5 lines with 6 vertices each
    {
        // Create a vertical line segment at the star's current position.
        detachedLinesPoints.insert(detachedLinesPoints.end(),
            {
                x1, y1, 0.0f, 1.0f, 1.0f, 1.0f, // First vertex of the line
                x2, y2, 0.0f, 1.0f, 1.0f, 1.0f  // Second vertex of the line
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

void MoveLineToPosition(std::vector<float>& detachedLinesPoints, int i,
    float targetX1, float targetX2, float targetY1, float targetY2,
    float step)
{
    // Calculate the distance to move on each axis for the first point
    float deltaX1 = (targetX1 - detachedLinesPoints[i]) * step;
    float deltaY1 = (targetY1 - detachedLinesPoints[i + 1]) * step;
    float deltaX2 = (targetX2 - detachedLinesPoints[i + 6]) * step;
    float deltaY2 = (targetY2 - detachedLinesPoints[i + 7]) * step;

    // Update the line's position incrementally
    detachedLinesPoints[i] += deltaX1;     // Update X1
    detachedLinesPoints[i + 1] += deltaY1; // Update Y1
    detachedLinesPoints[i + 6] += deltaX2; // Update X2
    detachedLinesPoints[i + 7] += deltaY2; // Update Y2
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

std::vector<float> ChangeColor(std::vector<float> points, Color color) {
    // Determine o tamanho do bloco (6 ou 7) dependendo se "a" existe
    size_t blockSize = (points.size() % 7 == 0) ? 7 : 6;

    float r, g, b;

    switch (color) {
    case YELLOW:
        r = 0.0f;
        g = 1.0f;
        b = 1.0f;
        break;

    case LIGHTPINK:
        r = 1.0f;
        g = 0.21484375f;
        b = 0.62890625f;
        break;

    case PINK:
        r = 0.7890625f;
        g = 0.01953125f;
        b = 0.453125f;
        break;

    case LIGHTORANGE:
        r = 1.0f;
        g = 0.658823f;
        b = 0.490196f;
        break;

    case ORANGE:
        r = 0.52549f;
        g = 0.0470588f;
        b = 0.0f;
        break;

    case DARKORANGE:
        r = 0.741176f;
        g = 0.1215686f;
        b = 0.0f;
        break;

    case LIGHTBLUE:
        r = 0.635294f;
        g = 0.745098f;
        b = 0.9254902f;
        break;

    case BLUE:
        r = 0.0274509f;
        g = 0.22745f;
        b = 0.7803921f;
        break;
    }

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
        float angle = i * angleIncrement + PI / 2; // Começar no topo (-PI/2 radianos)
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