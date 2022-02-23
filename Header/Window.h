#include <iostream>

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

class Window
{

private:
	GLFWwindow* window;
	bool isActive = false;

public:
	Window() = default;
	bool CreateWindowGlContext(std::string name, int sizeX, int sizeY);
	void Update();

	inline ~Window()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(window);
		glfwTerminate();
	}
};