#include "instance.h"
namespace OE {
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,	
														VkDebugUtilsMessageTypeFlagsEXT messageType,
														const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,	
														void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VulkanInstance::~VulkanInstance()
	{
		if (enableDebug)
			DestroyDebugUtilsMessengerEXT(instance,  debugMessenger,nullptr );
		if(instance!=VK_NULL_HANDLE)
			vkDestroyInstance(instance, nullptr);
	}

	void VulkanInstance::init(std::vector<const char*> ExtensionNames, std::vector<const char*>LayerNames, const std::string& AppName )
	{
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo instanceInfo{ };
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_3;
		appInfo.pEngineName = "OrnamentEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pApplicationName = AppName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		

		VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
		if (enableDebug) {
			ExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			LayerNames.push_back("VK_LAYER_KHRONOS_validation");
			debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugInfo.pfnUserCallback = debugCallback;
			debugInfo.pUserData = nullptr; 

			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
		}
		//查询层是否可用
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		for (const char* layerName : LayerNames) {
			bool layerFound = false;

			for ( auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				throw std::runtime_error("Layer 验证失败");
			}
		}

		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = ExtensionNames.size();
		instanceInfo.ppEnabledExtensionNames = ExtensionNames.data();
		instanceInfo.enabledLayerCount = LayerNames.size();
		instanceInfo.ppEnabledLayerNames = LayerNames.data();



		if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("instance 创建失败");
		}
		if (CreateDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}
}