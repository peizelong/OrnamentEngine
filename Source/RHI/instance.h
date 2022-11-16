#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"vulkan/vulkan.h"
namespace OE {
	class VulkanInstance
	{
	public:
		VulkanInstance()=default ;
		VulkanInstance(bool enableDebug):enableDebug(enableDebug) {}
		~VulkanInstance();
		void init( std::vector<const char*> ExtensionNames, std::vector<const char*>LayerNames, const std::string& AppName="OrnamentEngine");
	private:
		bool enableDebug = true;
		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debugMessenger=VK_NULL_HANDLE;


	};
}

