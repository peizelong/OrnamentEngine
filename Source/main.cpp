#include<iostream>
#include"core/RHI/VulkanRHI.h"


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	OE::VulkanRHI rhi;
	rhi.init(window);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	rhi.destroy();
	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}