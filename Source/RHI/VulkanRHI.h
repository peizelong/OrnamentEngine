#include <vector>
#include "glfw/glfw3.h"
#include "vulkan/vulkan.h"
#include "RHI.h"
#include "RHIStruct.h"
#include "../tools/tools.h"

namespace OE{
	class VulkanRHI :RHI
	{
	public:
		VulkanRHI();
		~VulkanRHI();

		void init();
		void destroy();
	public:
		VkInstance instance{ VK_NULL_HANDLE };
		VkDebugUtilsMessengerEXT debugMessenger{ VK_NULL_HANDLE };
		VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
		VkDevice device{ VK_NULL_HANDLE };
		VkSurfaceKHR surface{ VK_NULL_HANDLE };

		VkQueue graphicsQueue{ VK_NULL_HANDLE };
	private:
		PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;

	private:
		void createInstance();
		void createPhysicalDevice();
		void createLogicalDevice();
		void createSurface();
	private:
		bool EnableValidationLayers{ true };


		std::vector<const char*> ExtensionNames;
		std::vector<const char*> LayerNames;
	};


}