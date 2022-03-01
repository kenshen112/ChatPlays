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
        settingsScreen = true;
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
    if (ImGui::Button("Controller Settings"))
    {
        controllerSettings = true;
    }

    if (ImGui::Button("Close"))
    {
        settingsScreen = false;
    }
    ImGui::End();
}

void Window::DrawXinputSettings()
{
    if (controller == nullptr)
    {
        controller = new Emit();
    }
    ImGui::Begin("Controller Settings");
    // Int selector for player amount. 
    // Command inputs? Though I feel like that should belong to Twitch
    int amount = 0;
    if (ImGui::InputInt("Amount of players", &amount))
    {
        controller->SetPlayerAmount(amount);
    }
    ImGui::End();
}

void Window::DrawEvdevSettings()
{
    ImGui::Begin("Controller Settings");

    ImGui::End();
}

void Window::DrawControllerTAS()
{
    ImGui::Begin("TAS");

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

        if (settingsScreen)
        {
            DrawSettings();
        }

        if (controllerSettings)
        {
#ifdef _WIN32
            DrawXinputSettings();
#elif __linux__
            DrawEvdevSettings();
#endif
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }

}