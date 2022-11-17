#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"../tools/tools.h"
#include"vulkan/vulkan.h"
namespace OE {
	class VulkanInstance
	{
	public:
		VulkanInstance()=default ;
		~VulkanInstance();

		operator VkInstance() {	return instance;}

		VkInstance get() { return instance; }
		void init( std::vector<const char*> ExtensionNames, std::vector<const char*>LayerNames, const std::string& AppName="OrnamentEngine");
	private:
		VkInstance instance = VK_NULL_HANDLE;

#ifdef OE_VALIDATION_DEBUG
		VkDebugUtilsMessengerEXT debugMessenger=VK_NULL_HANDLE;
#endif // OE_VALIDATION_DEBUG



	};
}

