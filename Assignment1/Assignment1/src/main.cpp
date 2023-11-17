#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Headers/Renderer.h"
#include "Headers/UI.h"
#include "Headers/ppmLoader.h"
#include "Headers/Image.h"
#include "Headers/Screen.h"

int currentW = WIDTH;
int currentH = HEIGHT;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Image Processing: Nattamon", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glewInit() != GLEW_OK)
        std::cout << "Error: Unable to init GLEW" << std::endl;

    //cout version
    std::cout << glGetString(GL_VERSION) << std::endl;
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //Init IMGUI
    UI::GetInstance()->Init(window);
    
    Screen::GetInstance()->Init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwMakeContextCurrent(window);
        glfwGetWindowSize(window, &currentW, &currentH);
        glViewport(0, 0, currentW, currentH);
        Screen::GetInstance()->UpdateWindowDimensions(currentW, currentH);

        /* Render here */
        Renderer::GetInstance()->Clear();

        Screen::GetInstance()->Draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); //have before any imgui actions

        UI::GetInstance()->Display();
        Screen::GetInstance()->DrawUI();
        UI::GetInstance()->Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Screen::GetInstance()->CleanUp();
    UI::GetInstance()->CleanUp();

    glfwTerminate();
    return 0;
}