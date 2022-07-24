#include "MainWindow.h"


void MainWindow::Open()
{
    window = new Window();
    if (window->CreateWindowGlContext("ChatPlays", 1920, 1080))
    {

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();

        ImGui::NewFrame();

        if (window->CreateWindow("Main Window", &isOpen))
        {
            //window->Button("Twitch", tWindow->Open());

        }

        ImGui::EndFrame();
    }
}

void MainWindow::Close()
{

}

void MainWindow::Update()
{
    
}