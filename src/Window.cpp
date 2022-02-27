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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    return isActive;
}

void Window::DrawMainGUI()
{
    ImGui::Begin("Main");
    if (ImGui::Button("Settings"))
    {
        showSettingsScreen = true;
    }

    if (ImGui::Button("Control Manual"))
    {
        (*ManualCtrl)();
    }
    if (ImGui::Button("Start Bot"))
    {
        (*StartFunc)();
    }
    if (ImGui::Button("Exit"))
    {
        isActive = false;
        // Use this to close threads
        (*QuitFunc)(true);

        return;
    }
    ImGui::End();
}

void Window::DrawSettings()
{
    ImGui::Begin("Settings");
    // Show options for Twitch Or, Controller implementation
    if (ImGui::Button("Twitch"))
    {

    }

    if (ImGui::Button("Close"))
    {
        showSettingsScreen = false;
    }
    ImGui::End();
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

        DrawMainGUI();

        if (showSettingsScreen)
        {
            DrawSettings();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }

}