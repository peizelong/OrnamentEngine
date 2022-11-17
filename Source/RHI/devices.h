#pragma once
#include"../tools/tools.h"
#include"vulkan/vulkan.h"
namespace OE {


	class VulkanDevices
	{
	public:
		operator VkDevice() { return device; }
		void init(VkInstance instance);
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
	};
}
