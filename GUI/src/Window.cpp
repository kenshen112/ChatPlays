#include "Window.h"


bool Window::CreateWindowGlContext(std::string name, int sizeX, int sizeY)
{
    isActive = glfwInit();

    if (!isActive)
    {
        throw "Glfw Init error";
        return isActive = false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(sizeX, sizeY, name.c_str(), NULL, NULL);

    if (window == nullptr)
    {
        throw "Window error";
        return isActive = false;
    }

    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();

    return isActive;
}

void Window::Open(std::string title)
{
    if (ImGui::Begin(title.data()))
    {
        ImGui::End();
    }
}

void Window::Update()
{

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }

}