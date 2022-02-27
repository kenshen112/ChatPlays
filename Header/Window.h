#pragma once

#include <iostream>

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

#include "twitch.h"
#ifdef __linux__
    #include "Linux/EvDev.h"
#elif _WIN32
    #include "Windows/Xinput.h"
#endif

typedef void (*func_ptr)(void);

class Window
{
private:
    GLFWwindow* window;
    bool isActive = false;
    bool showSettingsScreen;

public:
    Window() = default;
    bool CreateWindowGlContext(std::string name, int sizeX, int sizeY);
    void Update();

    void DrawMainGUI();
    void DrawSettings();

    func_ptr StartFunc;
    func_ptr ManualCtrl;
    bool(*QuitFunc)(bool);

    inline ~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};