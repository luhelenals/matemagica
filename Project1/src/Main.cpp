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
#include <limits>
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
<<<<<<< HEAD
    PENTAGON,
=======
    PENTAGON, 
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    MOVINGTRIANGLES,
    MAKINGTRIRECTANGLES,
    TRIRECTANGLES,
    FINAL
};

enum Color {
    YELLOW,
    LIGHTPINK,
    PINK,
    LIGHTORANGE,
    ORANGE,
    DARKORANGE,
    LIGHTBLUE,
    BLUE,
<<<<<<< HEAD
    DARKBLUE,
    RED
=======
    DARKBLUE
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
};

void Bounce(std::vector<float>& points, float elapsedTime, bool& bouncing, bool& spinning, float& startTime);
void Spin(std::vector<float>& points, float elapsedTime, float direction, glm::vec3 center);
<<<<<<< HEAD
void CreateDetachingLines(float x1, float x2, float y1, float y2);
=======
void CreateDetachingLines(std::vector<float>& points, int point1, int point2, float y1, float y2);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> GetPoints(std::vector<float>& points);
void MoveObjectAwayFromCenter(std::vector<float>& points, float elapsedTime);
std::vector<float> GenerateFilledPentagon(std::vector<float> points);
<<<<<<< HEAD

float CalculateRadius(const std::vector<float>& points);
float CalculateDistance(float x1, float y1, float x2, float y2);

State currentState = BOUNCING; // Start with bouncing
=======
float CalculateRadius(const std::vector<float>& points);
float CalculateDistance(float x1, float y1, float x2, float y2);

State currentState = PENTAGON; // Start with bouncing
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
<<<<<<< HEAD
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
=======
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56

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
    bool hasSeparated = false;
    bool hasMiniRectangled = false;
    float step = 0.0015f;
<<<<<<< HEAD
    int qtdLines = 0;
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    int i = 1;

    float endTime = 0.0f;
    std::vector<float> actualPoints;
    std::vector<float> frozenPoints;
    std::vector<float> rectangle;
    std::vector<float> line;
    std::vector<float> l1, l2, l3, line1, line2, line3, l01, l02, l03, l11, l12, l13, l21, l22, l23, l31, l32, l33, l41, l42, l43;
    std::vector<float> triangleDC1, triangleAB2, triangleAE3;
    std::vector<float> pentagon, pentagon2;
    std::vector<float> tri0, tri1, tri2, tri3, tri4;
    tri0.resize(18);
    tri1.resize(18);
    tri2.resize(18);
    tri3.resize(18);
    tri4.resize(18);

    std::vector<float> miniT0 = tri0, miniT1 = tri1, miniT2 = tri2, miniT3 = tri3, miniT4 = tri4;

<<<<<<< HEAD
    l01.resize(12);
=======
    l01.resize(12); 
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    l02.resize(12);
    l03.resize(12);
    l11.resize(12);
    l12.resize(12);
    l13.resize(12);
    l21.resize(12);
    l22.resize(12);
    l23.resize(12);
    l31.resize(12);
    l32.resize(12);
    l33.resize(12);
<<<<<<< HEAD
    l41.resize(12);
=======
    l41.resize(12); 
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    l42.resize(12);
    l43.resize(12);

    pentagon.resize(30);
    pentagon2.resize(30);
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
<<<<<<< HEAD

=======
        
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
        std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(points);
        std::vector<float> t1 = triangleDC1, t2 = triangleDC1, t3 = triangleDC1;
        std::vector<float> miniR0, miniR1, miniR2, miniR3, miniR4;
        miniR0 = GenerateRectangle(l02, l01, l03);
        miniR1 = GenerateRectangle(l13, l12, l11);
        miniR2 = GenerateRectangle(l21, l23, l22);
        miniR3 = GenerateRectangle(l32, l31, l33);
        miniR4 = GenerateRectangle(l41, l42, l43);
        std::vector<float> miniR01 = miniR0, miniR11 = miniR1, miniR21 = miniR2, miniR31 = miniR3, miniR41 = miniR4;

<<<<<<< HEAD
=======
        /*std::cout << "Ls:" << std::endl;
        std::cout << l01[0] << ", " << l01[1] << ", " << l01[6] << ", " << l01[7] << std::endl;
        std::cout << l11[0] << ", " << l11[1] << ", " << l11[6] << ", " << l11[7] << std::endl;
        std::cout << l21[0] << ", " << l21[1] << ", " << l21[6] << ", " << l21[7] << std::endl;
        std::cout << l31[0] << ", " << l31[1] << ", " << l31[6] << ", " << l31[7] << std::endl;
        std::cout << l41[0] << ", " << l41[1] << ", " << l41[6] << ", " << l41[7] << std::endl;

        std::cout << " =============================================================================== " << std::endl;

        std::cout << l02[0] << ", " << l02[1] << ", " << l02[6] << ", " << l02[7] << std::endl;
        std::cout << l12[0] << ", " << l12[1] << ", " << l12[6] << ", " << l12[7] << std::endl;
        std::cout << l22[0] << ", " << l22[1] << ", " << l22[6] << ", " << l22[7] << std::endl;
        std::cout << l32[0] << ", " << l32[1] << ", " << l32[6] << ", " << l32[7] << std::endl;
        std::cout << l42[0] << ", " << l42[1] << ", " << l42[6] << ", " << l42[7] << std::endl;

        std::cout << " =============================================================================== " << std::endl;

        std::cout << l03[0] << ", " << l03[1] << ", " << l03[6] << ", " << l03[7] << std::endl;
        std::cout << l13[0] << ", " << l13[1] << ", " << l13[6] << ", " << l13[7] << std::endl;
        std::cout << l23[0] << ", " << l23[1] << ", " << l23[6] << ", " << l23[7] << std::endl;
        std::cout << l33[0] << ", " << l33[1] << ", " << l33[6] << ", " << l33[7] << std::endl;
        std::cout << l43[0] << ", " << l43[1] << ", " << l43[6] << ", " << l43[7] << std::endl;*/
     
        
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
        if (hasSeparated && !hasMiniRectangled) {
            std::tie(l01, l02, l03) = GetTriangleLines(tri0);
            std::tie(l11, l12, l13) = GetTriangleLines(tri1);
            std::tie(l21, l22, l23) = GetTriangleLines(tri2);
            std::tie(l31, l32, l33) = GetTriangleLines(tri3);
            std::tie(l41, l42, l43) = GetTriangleLines(tri4);
        }
        else std::tie(tri0, tri1, tri2, tri3, tri4) = GetPoints(points);

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
            if (elapsedTime >= 4.7f) // Bounce for 5 seconds
            {
<<<<<<< HEAD
                //std::cout << "State: BOUNCING" << std::endl;
=======
                std::cout << "State: BOUNCING" << std::endl;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
                triangleAB2 = ChangeColor(triangleAB2, DARKBLUE);
                triangleDC1 = ChangeColor(triangleDC1, DARKBLUE);
                triangleAE3 = ChangeColor(triangleAE3, DARKBLUE);

                DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);
            }
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
            break;

        case SPINNING:
<<<<<<< HEAD
            step = 1;
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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

<<<<<<< HEAD
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
                    detachedLinesPoints = ChangeColor(detachedLinesPoints, YELLOW);
                }
                else {
                    if (qtdLines < 1 && abs(points[0] - points[6]) >= 0.001 && points[0] < 0.4)
                    {
                        MoveLineToPosition(detachedLinesPoints, 0, points[12], points[18], points[13], points[19], step);
                    }
                    else if (qtdLines < 1)
                    {
                        detachedLinesPoints[3] = 1.0f;
                        detachedLinesPoints[4] = 0.0f;
                        detachedLinesPoints[5] = 0.0f;
                        detachedLinesPoints[9] = 1.0f;
                        detachedLinesPoints[10] = 0.0f;
                        detachedLinesPoints[11] = 0.0f;

                        qtdLines++;
                    }

                    if (qtdLines < 2 && abs(points[24] - points[0]) >= 0.001) {
                        MoveLineToPosition(detachedLinesPoints, 12, points[24], intersection4[0], points[25], intersection4[1], step);
                    }
                    else if (qtdLines < 2) 
                    {
                        detachedLinesPoints[15] = 1.0f;
                        detachedLinesPoints[16] = 0.0f;
                        detachedLinesPoints[17] = 0.0f;
                        detachedLinesPoints[21] = 1.0f;
                        detachedLinesPoints[22] = 0.0f;
                        detachedLinesPoints[23] = 0.0f;

                        qtdLines++;
                    }

                    if (qtdLines < 3 && abs(points[6] - points[12]) >= 0.005) {
                        MoveLineToPosition(detachedLinesPoints, 24, points[6], intersection3[0], points[7], intersection3[1], step);
                    }
                    else if (qtdLines < 3 && qtdLines > 1)
                    {
                        detachedLinesPoints[27] = 1.0f;
                        detachedLinesPoints[28] = 0.0f;
                        detachedLinesPoints[29] = 0.0f;
                        detachedLinesPoints[33] = 1.0f;
                        detachedLinesPoints[34] = 0.0f;
                        detachedLinesPoints[35] = 0.0f;

                        qtdLines++;
                    }

                    if (qtdLines < 4 && abs(points[24] - points[18]) >= 0.005) {
                        MoveLineToPosition(detachedLinesPoints, 36, intersection3[0], intersection2[0], intersection3[1], intersection2[1], step);
                    }
                    else if (qtdLines < 4 && qtdLines > 2)
                    {
                        detachedLinesPoints[39] = 1.0f;
                        detachedLinesPoints[40] = 0.0f;
                        detachedLinesPoints[41] = 0.0f;
                        detachedLinesPoints[45] = 1.0f;
                        detachedLinesPoints[46] = 0.0f;
                        detachedLinesPoints[47] = 0.0f;

                        qtdLines++;
                    }
                    MoveLineToPosition(detachedLinesPoints, 48, points[0], points[6], points[1], points[7], step);
                    MoveLineToPosition(detachedLinesPoints, 60, points[0], intersection4[0], points[1], intersection4[1], step);
                    MoveLineToPosition(detachedLinesPoints, 72, points[12], intersection3[0], points[13], intersection3[1], step);
                    MoveLineToPosition(detachedLinesPoints, 84, points[18], intersection2[0], points[19], intersection2[1], step);
                    MoveLineToPosition(detachedLinesPoints, 96, points[24], intersection3[0], points[25], intersection3[1], step);
                }
            }
            DrawDetachedLines(linesVBO, linesVAO, shader, detachedLinesPoints);

=======
            if (abs(points[0] - points[6]) <= 0.001 && points[0] > 0.4 && !spinningForward && detachedLinesPoints.size() < 12)
            {
                CreateDetachingLines(points, 0, 6, points[1], points[7]);
            }

            if (abs(points[12] - points[18]) <= 0.001 && points[0] > 0.0 && points[0] < 0.5 && !spinningForward && detachedLinesPoints.size() < 24)
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
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            break;

        case IDLE:
            if (elapsedTime >= 1.0f)// 2.0f) // Stay put seconds
            {
                std::cout << "State: IDLE" << std::endl;
                currentState = SPINNING;
                stateStartTime = currentTime; // Reset start time

                std::cout << "Points:" << std::endl;
                std::cout << points[0] << ", " << points[1] << std::endl;
                std::cout << points[6] << ", " << points[7] << std::endl;
                std::cout << points[12] << ", " << points[13] << std::endl;
                std::cout << points[18] << ", " << points[19] << std::endl;
                std::cout << points[24] << ", " << points[20] << "\n" << std::endl;
            }
            if (hasRectangled)
            {
                points = ChangeColor(points, LIGHTBLUE);
                triangleAB2 = ChangeColor(triangleAB2, DARKBLUE);
                triangleDC1 = ChangeColor(triangleDC1, DARKBLUE);
                triangleAE3 = ChangeColor(triangleAE3, DARKBLUE);

                DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);

                currentState = BOUNCING;
            }
            DrawShape(vbo, vao, shader, actualPoints, GL_LINE_LOOP, 6);
            break;

        case LINES:
            step = 0.0017;
<<<<<<< HEAD
            detachedLinesPoints = ChangeColor(detachedLinesPoints, RED);
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
=======
            if (elapsedTime < 5.0f) {
                MoveLineToPosition(detachedLinesPoints, 36, detachedLinesPoints[36], detachedLinesPoints[36], detachedLinesPoints[1], detachedLinesPoints[42] / 2 + 0.02, step);
            }
            if (abs(detachedLinesPoints[37] - detachedLinesPoints[1]) <= 0.001 && detachedLinesPoints.size() < 60) {
                CreateDetachingLines(detachedLinesPoints, 36, 42, detachedLinesPoints[37], detachedLinesPoints[43]);
            }
            if (elapsedTime > 3.0f && elapsedTime < 5.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[24], detachedLinesPoints[24], detachedLinesPoints[25], detachedLinesPoints[13], step);
                frozenPoints = detachedLinesPoints;
            }
            if (elapsedTime > 5.0f && elapsedTime < 7.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[12] - 0.02, detachedLinesPoints[12] - 0.02, detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 24, detachedLinesPoints[12] - 0.02, detachedLinesPoints[12] - 0.02, detachedLinesPoints[25], detachedLinesPoints[31], step);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            }
            if (elapsedTime > 7.0f && elapsedTime < 9.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[48], frozenPoints[48], detachedLinesPoints[49], detachedLinesPoints[55], step);
                MoveLineToPosition(detachedLinesPoints, 24, frozenPoints[24], frozenPoints[24], detachedLinesPoints[25], detachedLinesPoints[31], step);
            }
            if (elapsedTime > 9.0f && elapsedTime < 10.0f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[24], frozenPoints[24], detachedLinesPoints[25], detachedLinesPoints[31], 1);
            }
            if (elapsedTime > 10.0f && elapsedTime < 12.0f) {
<<<<<<< HEAD
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[12], detachedLinesPoints[12], detachedLinesPoints[19], detachedLinesPoints[7], step);
=======
                MoveLineToPosition(detachedLinesPoints, 48, detachedLinesPoints[12], detachedLinesPoints[12], detachedLinesPoints[7], detachedLinesPoints[13], step);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
<<<<<<< HEAD
            if (elapsedTime > 16.0f && elapsedTime < 16.5f) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[12], frozenPoints[12], detachedLinesPoints[13], detachedLinesPoints[19], 1);
                MoveLineToPosition(detachedLinesPoints, 60, detachedLinesPoints[0], detachedLinesPoints[6], detachedLinesPoints[1], detachedLinesPoints[7], 1);
            }
            if (elapsedTime > 16.5f && elapsedTime < 17.0f) {
                MoveLineToPosition(detachedLinesPoints, 72, detachedLinesPoints[24], detachedLinesPoints[30], detachedLinesPoints[25], detachedLinesPoints[31], 1);
                MoveLineToPosition(detachedLinesPoints, 84, detachedLinesPoints[12], detachedLinesPoints[18], detachedLinesPoints[13], detachedLinesPoints[19], 1);
                MoveLineToPosition(detachedLinesPoints, 96, detachedLinesPoints[24], detachedLinesPoints[30], detachedLinesPoints[25], detachedLinesPoints[31], 1);
=======
            if (elapsedTime > 16.0f && detachedLinesPoints.size() < 72) {
                MoveLineToPosition(detachedLinesPoints, 48, frozenPoints[12], frozenPoints[12], detachedLinesPoints[13], detachedLinesPoints[19], 1);
                CreateDetachingLines(detachedLinesPoints, 0, 6, detachedLinesPoints[1], detachedLinesPoints[7]);
            }
            if (elapsedTime > 16.5f && detachedLinesPoints.size() < 108) {
                CreateDetachingLines(detachedLinesPoints, 24, 30, detachedLinesPoints[25], detachedLinesPoints[31]);
                CreateDetachingLines(detachedLinesPoints, 12, 18, detachedLinesPoints[13], detachedLinesPoints[19]);
                CreateDetachingLines(detachedLinesPoints, 24, 30, detachedLinesPoints[25], detachedLinesPoints[31]);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
<<<<<<< HEAD
                MoveLineToPosition(detachedLinesPoints, 12, intersection4[0], points[0], intersection4[1], points[1], step);
                MoveLineToPosition(detachedLinesPoints, 24, triangleAE3[12], points[18], triangleAE3[13], points[19], step);
                MoveLineToPosition(detachedLinesPoints, 36, intersection5[0], triangleAE3[12], intersection5[1], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 48, points[24], triangleAE3[12], points[25], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 60, points[18], points[12], points[19], points[13], step);
                MoveLineToPosition(detachedLinesPoints, 72, points[6], triangleAE3[12], points[7], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 84, points[24], intersection4[0], points[25], intersection4[1], step);
                MoveLineToPosition(detachedLinesPoints, 96, intersection5[0], points[0], intersection5[1], points[1], step);
=======
                MoveLineToPosition(detachedLinesPoints, 12, points[0], intersection4[0], points[1], intersection4[1], step);
                MoveLineToPosition(detachedLinesPoints, 24, points[18], triangleAE3[12], points[19], triangleAE3[13], step);
                MoveLineToPosition(detachedLinesPoints, 36, triangleAE3[12], intersection5[0], triangleAE3[13], intersection5[1], step);
                MoveLineToPosition(detachedLinesPoints, 48, triangleAE3[12], points[24], triangleAE3[13], points[25], step);
                MoveLineToPosition(detachedLinesPoints, 60, points[18], points[12], points[19], points[13], step);
                MoveLineToPosition(detachedLinesPoints, 72, triangleAE3[12], points[6], triangleAE3[13], points[7], step);
                MoveLineToPosition(detachedLinesPoints, 84, intersection4[0], points[24], intersection4[1], points[25], step);
                MoveLineToPosition(detachedLinesPoints, 96, points[0], intersection5[0], points[1], intersection5[1], step);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            }
            if (elapsedTime < 22.0f) {
                DrawDetachedLines(linesVBO, linesVAO, shader, detachedLinesPoints);
            }
            else if (elapsedTime < 26.7f) {
                Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
                DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);

            }
            else {
<<<<<<< HEAD
                frozenPoints = points;
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
                triangle2 = ChangeColor(triangle2, DARKBLUE);
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
                    triangle2 = ChangeColor(triangle2, DARKBLUE);
                    triangleDC1 = ChangeColor(triangleDC1, LIGHTBLUE);
                }
                DrawShape(vbo, vao, shader, triangle2, GL_TRIANGLES, 3);

                if (elapsedTime >= 2.0f) {
                    std::cout << "State: TRIANGLE" << std::endl;
                    currentState = hasRectangled ? ROTATING : MAKINGRECTANGLE;
                    stateStartTime = currentTime;
<<<<<<< HEAD
=======

                    std::cout << "t2: " << std::endl;
                    std::cout << t2[0] << ", " << t2[1] << std::endl;
                    std::cout << t2[6] << ", " << t2[7] << std::endl;
                    std::cout << t2[12] << ", " << t2[13] << std::endl;

                    std::cout << "t3: " << std::endl;
                    std::cout << t3[0] << ", " << t3[1] << std::endl;
                    std::cout << t3[6] << ", " << t3[7] << std::endl;
                    std::cout << t3[12] << ", " << t3[13] << std::endl;

                    std::cout << "\n" << std::endl;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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

<<<<<<< HEAD
                float frequency = 10.0f; // Frequência da piscada (alterar conforme desejado)
=======
                float frequency = 2.0f; // Frequência da piscada (alterar conforme desejado)
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
                float intensity = (sin(elapsedTime * frequency) + 1.0f) / 2.0f;

                if (intensity > 0.5f)
                    rectangle2 = ChangeColor(rectangle2, DARKORANGE);
                else
                    rectangle2 = ChangeColor(rectangle2, ORANGE);

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
            points = ChangeColor(points, LIGHTBLUE);
            ScaleObject(points, elapsedTime);
            Bounce(points, elapsedTime, bouncing, spinning, stateStartTime);
<<<<<<< HEAD

=======
            
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(points);
            pentagon = GeneratePentagon(points);
            triangleAB2 = ChangeColor(triangleAB2, DARKBLUE);
            triangleDC1 = ChangeColor(triangleDC1, DARKBLUE);
            triangleAE3 = ChangeColor(triangleAE3, DARKBLUE);

            DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);

            if (elapsedTime > 2.0f && CalculateCenter(points).y < 0.1f) {

                frozenPoints = points;
                stateStartTime = currentTime;
                currentState = STOPPED;
            }
<<<<<<< HEAD

=======
            
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            break;

        case MOVINGTRIANGLES:
            std::tie(tri0, tri1, tri2, tri3, tri4) = GetPoints(frozenPoints);
            std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(frozenPoints);

            triangleAB2 = ChangeColor(triangleAB2, DARKBLUE);
            triangleDC1 = ChangeColor(triangleDC1, DARKBLUE);
            triangleAE3 = ChangeColor(triangleAE3, DARKBLUE);

            DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);

            miniT0 = ChangeColor(tri0, BLUE);
            miniT1 = ChangeColor(tri1, BLUE);
            miniT2 = ChangeColor(tri2, BLUE);
            miniT3 = ChangeColor(tri3, BLUE);
            miniT4 = ChangeColor(tri4, BLUE);
            DrawShape(vbo, vao, shader, miniT0, GL_TRIANGLES, 3);
            DrawShape(vbo, vao, shader, miniT1, GL_TRIANGLES, 3);
            DrawShape(vbo, vao, shader, miniT2, GL_TRIANGLES, 3);
            DrawShape(vbo, vao, shader, miniT3, GL_TRIANGLES, 3);
            DrawShape(vbo, vao, shader, miniT4, GL_TRIANGLES, 3);

            if (elapsedTime < 0.5f) {
                MoveObjectAwayFromCenter(tri0, elapsedTime);
                MoveObjectAwayFromCenter(tri1, elapsedTime);
                MoveObjectAwayFromCenter(tri2, elapsedTime);
                MoveObjectAwayFromCenter(tri3, elapsedTime);
                MoveObjectAwayFromCenter(tri4, elapsedTime);
                MoveObjectAwayFromCenter(triangleAB2, elapsedTime);
                MoveObjectAwayFromCenter(triangleDC1, elapsedTime);
                MoveObjectAwayFromCenter(triangleAE3, elapsedTime);
                endTime = elapsedTime;
            }
            else {
                MoveObjectAwayFromCenter(tri0, endTime);
                MoveObjectAwayFromCenter(tri1, endTime);
                MoveObjectAwayFromCenter(tri2, endTime);
                MoveObjectAwayFromCenter(tri3, endTime);
                MoveObjectAwayFromCenter(tri4, endTime);
                MoveObjectAwayFromCenter(triangleAB2, endTime);
                MoveObjectAwayFromCenter(triangleDC1, endTime);
                MoveObjectAwayFromCenter(triangleAE3, endTime);
            }

            tri1 = ChangeColor(tri1, LIGHTBLUE);
            tri2 = ChangeColor(tri2, LIGHTBLUE);
            tri3 = ChangeColor(tri3, LIGHTBLUE);
            tri4 = ChangeColor(tri4, LIGHTBLUE);
            tri0 = ChangeColor(tri0, LIGHTBLUE);
<<<<<<< HEAD
            pentagon = GeneratePentagon(frozenPoints);
=======

>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            DrawShape(vbo, vao, shader, pentagon, GL_LINE_LOOP, 5);
            DrawShape(vbo, vao, shader, tri0, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri1, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri2, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri3, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri4, GL_LINE_LOOP, 3);

            if (elapsedTime > 2.5f) {
                stateStartTime = currentTime;
                currentState = MAKINGTRIRECTANGLES;
                hasSeparated = true;
            }

            break;

        case MAKINGTRIRECTANGLES:
<<<<<<< HEAD

=======
                            
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            Rotate(l02, l01, elapsedTime, glm::vec3(l02[6], l02[7], l02[8]), glm::vec3(l01[6], l01[7], l01[8]));
            Rotate(l11, l12, elapsedTime, glm::vec3(l11[6], l11[7], l11[8]), glm::vec3(l12[6], l12[7], l12[8]));
            Rotate(l21, l22, elapsedTime, glm::vec3(l21[6], l21[7], l21[8]), glm::vec3(l22[6], l22[7], l22[8]));
            Rotate(l31, l32, elapsedTime, glm::vec3(l31[6], l31[7], l31[8]), glm::vec3(l32[6], l32[7], l32[8]));
            Rotate(l41, l42, elapsedTime, glm::vec3(l41[6], l41[7], l41[8]), glm::vec3(l42[6], l42[7], l42[8]));
<<<<<<< HEAD

=======
            
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            if (abs(l01[0] - l01[6]) > 0.001f)
            {
                miniT0 = ChangeColor(tri0, BLUE);
                miniT1 = ChangeColor(tri1, BLUE);
                miniT2 = ChangeColor(tri2, BLUE);
                miniT3 = ChangeColor(tri3, BLUE);
                miniT4 = ChangeColor(tri4, BLUE);

                DrawShape(vbo, vao, shader, miniT0, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT1, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT2, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT3, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT4, GL_TRIANGLES, 3);

                pentagon2 = GenerateFilledPentagon(frozenPoints);
<<<<<<< HEAD
                pentagon = GeneratePentagon(frozenPoints);
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
                DrawShape(vbo, vao, shader, pentagon2, GL_TRIANGLES, pentagon2.size() / 6);
                DrawShape(vbo, vao, shader, pentagon, GL_LINE_LOOP, 5);

                DrawShape(vbo, vao, shader, l41, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l42, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l43, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l31, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l32, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l33, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l01, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l02, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l03, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l21, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l22, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l23, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l11, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l12, GL_LINES, 2);
                DrawShape(vbo, vao, shader, l13, GL_LINES, 2);
            }
            else {
                currentState = TRIRECTANGLES;
                stateStartTime = currentTime;
                hasMiniRectangled = true;
            }
            break;

        case TRIRECTANGLES:
            if (elapsedTime < 0.5f)
            {
                DrawShape(vbo, vao, shader, miniT0, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT1, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT2, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT3, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT4, GL_TRIANGLES, 3);
            }

            pentagon2 = GenerateFilledPentagon(frozenPoints);
<<<<<<< HEAD
            pentagon = GeneratePentagon(frozenPoints); // verificar!!
            DrawShape(vbo, vao, shader, pentagon2, GL_TRIANGLES, pentagon2.size() / 6);
            DrawShape(vbo, vao, shader, pentagon, GL_LINE_LOOP, 5);
=======
            DrawShape(vbo, vao, shader, pentagon2, GL_TRIANGLES, pentagon2.size() / 6);
            DrawShape(vbo, vao, shader, pentagon, GL_LINE_LOOP, 5);                
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56

            miniR0 = ChangeColor(std::vector<float>({
                l02[0], l02[1], l02[2], 0.0f, 0.0f, 0.0f,
                l01[0], l01[1], l01[2], 0.0f, 0.0f, 0.0f,
                l01[6], l01[7], l01[7], 0.0f, 0.0f, 0.0f,

                l01[6], l01[7], l01[7], 0.0f, 0.0f, 0.0f,
                l02[6], l02[7], l02[8], 0.0f, 0.0f, 0.0f,
                l02[0], l02[1], l02[2], 0.0f, 0.0f, 0.0f
                }), LIGHTBLUE);

            miniR1 = ChangeColor(std::vector<float>({
                l12[0], l12[1], l12[2], 0.0f, 0.0f, 0.0f,
                l11[0], l11[1], l11[2], 0.0f, 0.0f, 0.0f,
                l11[6], l11[7], l11[7], 0.0f, 0.0f, 0.0f,

                l11[6], l11[7], l11[7], 0.0f, 0.0f, 0.0f,
                l12[6], l12[7], l12[8], 0.0f, 0.0f, 0.0f,
                l12[0], l12[1], l12[2], 0.0f, 0.0f, 0.0f
                }), LIGHTBLUE);

            miniR2 = ChangeColor(std::vector<float>({
                l22[0], l22[1], l22[2], 0.0f, 0.0f, 0.0f,
                l21[0], l21[1], l21[2], 0.0f, 0.0f, 0.0f,
                l21[6], l21[7], l21[7], 0.0f, 0.0f, 0.0f,

                l21[6], l21[7], l21[7], 0.0f, 0.0f, 0.0f,
                l22[6], l22[7], l22[8], 0.0f, 0.0f, 0.0f,
                l22[0], l22[1], l22[2], 0.0f, 0.0f, 0.0f
                }), LIGHTBLUE);

            miniR3 = ChangeColor(std::vector<float>({
                l32[0], l32[1], l32[2], 0.0f, 0.0f, 0.0f,
                l31[0], l31[1], l31[2], 0.0f, 0.0f, 0.0f,
                l31[6], l31[7], l31[7], 0.0f, 0.0f, 0.0f,

                l31[6], l31[7], l31[7], 0.0f, 0.0f, 0.0f,
                l32[6], l32[7], l32[8], 0.0f, 0.0f, 0.0f,
                l32[0], l32[1], l32[2], 0.0f, 0.0f, 0.0f
                }), LIGHTBLUE);

            miniR4 = ChangeColor(std::vector<float>({
                l42[0], l42[1], l42[2], 0.0f, 0.0f, 0.0f,
                l41[0], l41[1], l41[2], 0.0f, 0.0f, 0.0f,
                l41[6], l41[7], l41[7], 0.0f, 0.0f, 0.0f,

                l41[6], l41[7], l41[7], 0.0f, 0.0f, 0.0f,
                l42[6], l42[7], l42[8], 0.0f, 0.0f, 0.0f,
                l42[0], l42[1], l42[2], 0.0f, 0.0f, 0.0f
                }), LIGHTBLUE);

            if (elapsedTime > 0.5f && elapsedTime < 2.2f) {
                miniR01 = ChangeColor(miniR0, DARKORANGE);
                miniR11 = ChangeColor(miniR1, DARKORANGE);
                miniR21 = ChangeColor(miniR2, DARKORANGE);
                miniR31 = ChangeColor(miniR3, DARKORANGE);
                miniR41 = ChangeColor(miniR4, DARKORANGE);

                DrawShape(vbo, vao, shader, miniR41, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, miniR21, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, miniR31, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, miniR11, GL_TRIANGLES, 6);
                DrawShape(vbo, vao, shader, miniR01, GL_TRIANGLES, 6);

                miniR0 = ChangeColor(miniR0, LIGHTORANGE);
                miniR1 = ChangeColor(miniR1, LIGHTORANGE);
                miniR2 = ChangeColor(miniR2, LIGHTORANGE);
                miniR3 = ChangeColor(miniR3, LIGHTORANGE);
                miniR4 = ChangeColor(miniR4, LIGHTORANGE);
<<<<<<< HEAD

=======
                    
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            }
            if (elapsedTime < 2.2f) {
                DrawShape(vbo, vao, shader, miniR4, GL_LINE_LOOP, 6);
                DrawShape(vbo, vao, shader, miniR2, GL_LINE_LOOP, 6);
                DrawShape(vbo, vao, shader, miniR3, GL_LINE_LOOP, 6);
                DrawShape(vbo, vao, shader, miniR1, GL_LINE_LOOP, 6);
                DrawShape(vbo, vao, shader, miniR0, GL_LINE_LOOP, 6);
            }

            if (elapsedTime > 2.2f) {
                stateStartTime = currentTime;
                currentState = FINAL;
            }

            break;

        case STOPPED:
            std::tie(triangleDC1, triangleAB2, triangleAE3) = GetTriangles(frozenPoints);
            std::tie(tri0, tri1, tri2, tri3, tri4) = GetPoints(frozenPoints);

            triangleAB2 = ChangeColor(triangleAB2, DARKBLUE);
            triangleDC1 = ChangeColor(triangleDC1, DARKBLUE);
            triangleAE3 = ChangeColor(triangleAE3, DARKBLUE);

            DrawShape(vbo, vao, shader, triangleAB2, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleDC1, GL_TRIANGLES, 6);
            DrawShape(vbo, vao, shader, triangleAE3, GL_TRIANGLES, 6);

            if (elapsedTime > 0.5f) {
                miniT0 = ChangeColor(tri0, BLUE);
                miniT1 = ChangeColor(tri1, BLUE);
                miniT2 = ChangeColor(tri2, BLUE);
                miniT3 = ChangeColor(tri3, BLUE);
                miniT4 = ChangeColor(tri4, BLUE);

                DrawShape(vbo, vao, shader, miniT0, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT1, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT2, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT3, GL_TRIANGLES, 3);
                DrawShape(vbo, vao, shader, miniT4, GL_TRIANGLES, 3);

                if (elapsedTime > 1.2f) {
                    currentState = MOVINGTRIANGLES;
                    stateStartTime = currentTime;
                }
            }

            tri1 = ChangeColor(tri1, LIGHTBLUE);
            tri2 = ChangeColor(tri2, LIGHTBLUE);
            tri3 = ChangeColor(tri3, LIGHTBLUE);
            tri4 = ChangeColor(tri4, LIGHTBLUE);
            tri0 = ChangeColor(tri0, LIGHTBLUE);

            DrawShape(vbo, vao, shader, tri0, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri1, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri2, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri3, GL_LINE_LOOP, 3);
            DrawShape(vbo, vao, shader, tri4, GL_LINE_LOOP, 3);
            break;

        case FINAL:
            if (elapsedTime < 3.5f)
            {
                pentagon2 = GenerateFilledPentagon(frozenPoints);
                ScaleObject(pentagon2, -elapsedTime * 5);
                DrawShape(vbo, vao, shader, pentagon2, GL_TRIANGLES, pentagon2.size() / 6);
            }
            else {
<<<<<<< HEAD
                float radius = CalculateRadius(pentagon2) / 3.2;
=======
                float radius = CalculateRadius(pentagon2)/3.2;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
                frozenPoints = generatePentagram(-radius, CalculateCenter(pentagon2));
                DrawShape(vbo, vao, shader, pentagon2, GL_TRIANGLES, pentagon2.size() / 6);
                DrawShape(vbo, vao, shader, frozenPoints, GL_LINE_LOOP, 6);
            }

<<<<<<< HEAD

=======
            
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
            //DrawShape(vbo, vao, shader, points, GL_LINE_LOOP, 6);
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

// Função para calcular a distância entre dois pontos 2D
float CalculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Função para calcular o raio da figura
float CalculateRadius(const std::vector<float>& points) {
    float centerX = 0.0f;
    float centerY = 0.0f;
    int numVertices = points.size() / 7; // Cada vértice tem 7 valores: x, y, z, r, g, b, a

    // Calcular o centro da figura
    for (int i = 0; i < numVertices; ++i) {
        centerX += points[i * 7];
        centerY += points[i * 7 + 1];
    }
    centerX /= numVertices;
    centerY /= numVertices;

    // Calcular o raio (maior distância do centro a qualquer vértice)
    float maxDistance = 0.0f;
    for (int i = 0; i < numVertices; ++i) {
        float x = points[i * 7];
        float y = points[i * 7 + 1];
        float distance = CalculateDistance(centerX, centerY, x, y);
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }

    return maxDistance;
}

void MoveObjectAwayFromCenter(std::vector<float>& points, float elapsedTime) {
    // Primeiro ponto do triângulo (tri0)
    glm::vec2 tri0(points[0], points[1]);

    // Centro da tela
    glm::vec2 center = CalculateCenter(points);

    // Calcula a direção do movimento (tri0 -> centro)
    glm::vec2 direction = glm::normalize(center - tri0);

    float factor = elapsedTime * 0.2;

    // Desloca cada vértice do triângulo na direção calculada
    for (size_t i = 0; i < points.size(); i += 3) {
        glm::vec2 point(points[i], points[i + 1]);

        // Aplica um pequeno deslocamento na direção do vetor, proporcional à distância
        point -= (direction * factor);

        // Atualiza o vetor de pontos
        points[i] = point.x;
        points[i + 1] = point.y;
    }
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

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> GetPoints(std::vector<float>& points) {
    glm::vec2 a = glm::vec2(points[0], points[1]);
    glm::vec2 b = glm::vec2(points[6], points[7]);
    glm::vec2 c = glm::vec2(points[12], points[13]);
    glm::vec2 d = glm::vec2(points[18], points[19]);
    glm::vec2 e = glm::vec2(points[24], points[25]);

    glm::vec2 p1 = GetIntersectionPoint(d, c, a, e);
    glm::vec2 p2 = GetIntersectionPoint(a, e, c, b);
    glm::vec2 p3 = GetIntersectionPoint(c, b, e, d);
    glm::vec2 p4 = GetIntersectionPoint(e, d, b, a);
    glm::vec2 p5 = GetIntersectionPoint(b, a, d, c);

    std::vector<float> tri0 = {
        points[0], points[1], 0.0f, 0.0f, 0.0f, 0.0f,
        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> tri1 = {
        points[6], points[7], 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p4.x, p4.y, 0.0f, 0.0f, 0.0f, 0.0f
    };
<<<<<<< HEAD

=======
    
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    std::vector<float> tri2 = {
        points[12], points[13], 0.0f, 0.0f, 0.0f, 0.0f,
        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> tri3 = {
        points[18], points[19], 0.0f, 0.0f, 0.0f, 0.0f,
        p4.x, p4.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> tri4 = {
        points[24], points[25], 0.0f, 0.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f
    };

    tri1 = ChangeColor(tri1, LIGHTBLUE);
    tri2 = ChangeColor(tri2, LIGHTBLUE);
    tri3 = ChangeColor(tri3, LIGHTBLUE);
    tri4 = ChangeColor(tri4, LIGHTBLUE);
    tri0 = ChangeColor(tri0, LIGHTBLUE);

    return std::make_tuple(tri0, tri1, tri2, tri3, tri4);
}

std::vector<float> GeneratePentagon(std::vector<float> points) {
    glm::vec2 a = glm::vec2(points[0], points[1]);
    glm::vec2 b = glm::vec2(points[6], points[7]);
    glm::vec2 c = glm::vec2(points[12], points[13]);
    glm::vec2 d = glm::vec2(points[18], points[19]);
    glm::vec2 e = glm::vec2(points[24], points[25]);

    glm::vec2 p1 = GetIntersectionPoint(d, c, a, e);
    glm::vec2 p2 = GetIntersectionPoint(a, e, c, b);
    glm::vec2 p3 = GetIntersectionPoint(c, b, e, d);
    glm::vec2 p4 = GetIntersectionPoint(e, d, b, a);
    glm::vec2 p5 = GetIntersectionPoint(b, a, d, c);

    std::vector<float> pentagon = ChangeColor(std::vector<float>({
        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p4.x, p4.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f,
        }), LIGHTBLUE);

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
<<<<<<< HEAD
        angle -= 89;
=======
        angle += 190;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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

<<<<<<< HEAD
void CreateDetachingLines(float x1, float x2, float y1, float y2)
=======
void CreateDetachingLines(std::vector<float>& points, int point1, int point2, float y1, float y2)
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
{
    // Only create a line after enough time has passed.
    if (detachedLinesPoints.size() < 30 * 6) // Assume 5 lines with 6 vertices each
    {
        // Create a vertical line segment at the star's current position.
        detachedLinesPoints.insert(detachedLinesPoints.end(),
            {
<<<<<<< HEAD
                x1, y1, 0.0f, 1.0f, 1.0f, 1.0f, // First vertex of the line
                x2, y2, 0.0f, 1.0f, 1.0f, 1.0f  // Second vertex of the line
            });
=======
                points[point1], y1, 0.0f, 1.0f, 1.0f, 1.0f, // First vertex of the line
                points[point2], y2, 0.0f, 1.0f, 1.0f, 1.0f  // Second vertex of the line
            });
        detachedLinesPoints = ChangeColor(detachedLinesPoints, DARKORANGE);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
<<<<<<< HEAD
    step += 0.008;
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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

std::vector<float> GenerateFilledPentagon(std::vector<float> points) {
    glm::vec2 a = glm::vec2(points[0], points[1]);
    glm::vec2 b = glm::vec2(points[6], points[7]);
    glm::vec2 c = glm::vec2(points[12], points[13]);
    glm::vec2 d = glm::vec2(points[18], points[19]);
    glm::vec2 e = glm::vec2(points[24], points[25]);

    glm::vec2 p1 = GetIntersectionPoint(d, c, a, e);
    glm::vec2 p2 = GetIntersectionPoint(a, e, c, b);
    glm::vec2 p3 = GetIntersectionPoint(c, b, e, d);
    glm::vec2 p4 = GetIntersectionPoint(e, d, b, a);
    glm::vec2 p5 = GetIntersectionPoint(b, a, d, c);

    std::vector<float> pentagon = ChangeColor(std::vector<float>({
        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p2.x, p2.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,

        p1.x, p1.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f,

        p3.x, p3.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p4.x, p4.y, 0.0f, 0.0f, 0.0f, 0.0f,
        p5.x, p5.y, 0.0f, 0.0f, 0.0f, 0.0f,
        }), BLUE);

    return pentagon;
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
<<<<<<< HEAD
        r = 1.0f;
        g = 1.0f;
        b = 0.0f;
=======
        r = 0.0f;
        g = 1.0f;
        b = 1.0f;
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
        r = 0.2f;
        g = 0.32941176f;
        b = 0.921568f;
        break;
<<<<<<< HEAD

=======
    
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
    case DARKBLUE:
        r = 0.0274509f;
        g = 0.22745f;
        b = 0.7803921f;
        break;
<<<<<<< HEAD

    case RED:
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
        break;
=======
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
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
        center.x += vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) * 6 + 0];
        center.y += vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) * 6 + 1];
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

<<<<<<< HEAD
    std::vector<float> points = {
    l1[0], l1[1], l1[2], 1.0f, 0.21484375f, 0.62890625f, // top left
    l1[6], l1[7], l1[8], 1.0f, 0.21484375f, 0.62890625f, // top right
    l3[6], l3[7], l3[8], 1.0f, 0.21484375f, 0.62890625f, // bottom right

    l3[6], l3[7], l3[8], 1.0f, 0.21484375f, 0.62890625f, // bottom right
    l2[6], l2[7], l2[8], 1.0f, 0.21484375f, 0.62890625f,  // bottom left
    l1[0], l1[1], l1[2], 1.0f, 0.21484375f, 0.62890625f, // top left
    };
=======
    std::vector<float> points = ChangeColor(std::vector<float>({
        l1[0], l1[1], l1[2], 0.0f, 0.0f, 0.0f, // top left
        l1[6], l1[7], l1[8], 0.0f, 0.0f, 0.0f, // top right
        l3[6], l3[7], l3[8], 0.0f, 0.0f, 0.0f, // bottom right

        l3[6], l3[7], l3[8], 0.0f, 0.0f, 0.0f, // bottom right
        l2[6], l2[7], l2[8], 0.0f, 0.0f, 0.0f,  // bottom left
        l1[0], l1[1], l1[2], 0.0f, 0.0f, 0.0f, // top left
        }), LIGHTPINK);
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56

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