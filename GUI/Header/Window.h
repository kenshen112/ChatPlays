#pragma once

#include <iostream>

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"


typedef void (*func_ptr)(void);

class Window
{
private:
    GLFWwindow* window;

    bool isActive = false;
    
    bool settingsScreen;
    bool twitchSettings;
    bool controllerSettings;
    bool mainWindow;
    bool tasWindow;
    bool manualControlWindow;

public:
    Window() = default;
    bool CreateWindowGlContext(std::string name, int sizeX, int sizeY);
    void Update();

    void Button(std::string name, func_ptr func);
    void Button(std::string name, bool& toActivate);
    void Slider(std::string name, int var, int min, int max);
    void Slider(std::string name, float var, float min, float max);

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