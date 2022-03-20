#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include "camera.hpp"
#include "scene.hpp"
#include <vector>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int prevX = WINDOW_WIDTH/2;
int prevY = WINDOW_HEIGHT/2;

Camera* camera;

void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    static std::map<GLenum, const GLchar*> sources =
    {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
        {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
        {GL_DEBUG_SOURCE_OTHER, "OTHER"}
    };

    static std::map<GLenum, const GLchar*> severities =
    {
        {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW, "LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
    };

    static std::map<GLenum, const GLchar*> types =
    {
        {GL_DEBUG_TYPE_ERROR, "ERROR"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
        {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
        {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
        {GL_DEBUG_TYPE_MARKER, "MARKER"},
        {GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
        {GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
        {GL_DEBUG_TYPE_OTHER, "OTHER"}
    };
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION && source != GL_DEBUG_SOURCE_SHADER_COMPILER)
    {
        std::cout
        << "# opengl debug :\n"
        << " - code : " << glGetError() << "\n"  
        << " - type : " << types[type] << "\n"
        << " - severity : " << severities[severity] << "\n"
        << " - source : " << sources[source] << "\n" 
        << " - id : " << id << "\n"
        << " - message : " << message 
        << std::endl;
    }
}

/* resizes the viewport whenever the window is resized */
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* processes inputs from the window state */
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        (*camera).processKeyboard(Camera::Direction::FORWARD);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        (*camera).processKeyboard(Camera::Direction::BACKWARD);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        (*camera).processKeyboard(Camera::Direction::LEFT);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        (*camera).processKeyboard(Camera::Direction::RIGHT);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        (*camera).processKeyboard(Camera::Direction::UP);
    }
}

void processMouse(GLFWwindow* window, double x, double y)
{
    float xOffset = (prevX - x)/(WINDOW_WIDTH/2);
    float yOffset = (prevY - y)/(WINDOW_HEIGHT/2);
    prevX = x;
    prevY = y;
    (*camera).processMouseMouvements(xOffset, yOffset);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cerr << "# glfw initialization failed" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DRIFT", NULL, NULL);
    if (!window)
    {
        std::cerr << "# window opening failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* make the window's context the main context of the current thread */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);

    if(!gladLoadGL())
    {
        std::cerr << "# glad initialization failed" << std::endl;
        return -1;
    }

    std::cout << "# opengl version : " << glGetString(GL_VERSION) << std::endl;

    int number;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &number);
    std::cout << "# maximum vertex attributes available : " << number << std::endl;

    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &number);
    std::cout << "# maximum texture units available : " << number << std::endl;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);

    /* shaders */
    Shader lightingShader = Shader("../shader/lighting_vertex.vs", "../shader/lighting_fragment.fs");
    Shader lightShader = Shader("../shader/light_vertex.vs", "../shader/light_fragment.fs");
    
    if(!lightingShader.id || !lightShader.id)
    {
        return -1;
    }

    Scene scene = Scene(lightingShader);
    camera = &scene.camera;    

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
    /* loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* input */
        processInput(window);

        /* render */
        glClearColor(0.7f, 0.8f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        scene.update();

        /* swap front and back buffers */
        glfwSwapBuffers(window);

        /* poll for and process events */
        glfwPollEvents();
    }
    lightingShader.deleteProgram();
    glfwTerminate();
    return 0;
}