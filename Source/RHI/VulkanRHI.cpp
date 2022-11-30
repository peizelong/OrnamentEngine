#include "VulkanRHI.h"
namespace OE {
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
														VkDebugUtilsMessageTypeFlagsEXT messageType,
														const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
														void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	bool isDeviceSuitable(VkPhysicalDevice physicalDevice) {

		QueueFamilyIndices indices = findQueueFamily(physicalDevice);

		//VkPhysicalDeviceProperties deviceProperties;
		//VkPhysicalDeviceFeatures deviceFeatures;
		//vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		//vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		//	deviceFeatures.geometryShader;
		return indices.isComplete();

	}
	QueueFamilyIndices findQueueFamily(VkPhysicalDevice physicalDevice) {
		QueueFamilyIndices familyIndices;
		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				familyIndices.graphicsFamily = i;
			}
			if (familyIndices.isComplete()) {
				break;
			}
			i++;
		}
		return familyIndices;
	}

	VulkanRHI::VulkanRHI()	{}
	VulkanRHI::~VulkanRHI()	{}

	void VulkanRHI::init()
	{
		ExtensionNames.push_back("VK_KHR_win32_surface");

		createInstance();
		createPhysicalDevice();
		createLogicalDevice();
		createSurface();
	
	}

	void VulkanRHI::destroy()
	{

	}



	void VulkanRHI::createInstance()
	{
		/// <summary>
		/// 创建 VkInstance 和 VkDebugUtilsMessengerEXT
		/// </summary>
		/// <returns></returns>
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo instanceInfo{ };
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_3;
		appInfo.pEngineName = "OrnamentEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pApplicationName = "OrnamentEngine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

		//#ifdef OE_VALIDATION_DEBUG
		VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
		if (EnableValidationLayers) {
			ExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			LayerNames.push_back("VK_LAYER_KHRONOS_validation");
			debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugInfo.pfnUserCallback = debugCallback;
			debugInfo.pUserData = nullptr;

			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
		}
		//#endif // OE_VALIDATION_DEBUG

				//查询层是否可用
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		for (const char* layerName : LayerNames) {
			bool layerFound = false;

			for (auto& layerProperties : availableLayers) {
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


		VKRESULT(vkCreateInstance(&instanceInfo, nullptr, &instance))

#ifdef OE_VALIDATION_DEBUG
			if (EnableValidationLayers) {
				CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
				DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

				VKRESULT(CreateDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, &debugMessenger))
			}
#endif // DEBUG
	}
	void VulkanRHI::createPhysicalDevice()
	{

		/// <summary>
		/// 挑选VkPhysicalDevice 和 VkDevice
		/// </summary>
		/// <returns></returns>
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			std::runtime_error("Dont have physical devices");
		}
		std::vector<VkPhysicalDevice> physicalDevices;
		vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());
		for (const auto& device : physicalDevices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}
		if (physicalDevice == VK_NULL_HANDLE) {
			std::runtime_error("failed to find a suitable GPU!");
		}

	}
	void VulkanRHI::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamily(physicalDevice);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;
		createInfo.enabledLayerCount = 0;


		VKRESULT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	}
	void VulkanRHI::createSurface()
	{
	}
}