#include "App.h"

namespace CPVE {
	App::App(int w, int h, std::string title)
	{
		Width = w;
		Height = h;
		Title = title;
	}


	App::~App()
	{

	}

	VkBool32 App::VKDebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
	{
		Log::Message("[VKDebugCallback] A validation layer reported an error:\n" + std::string(msg));
		return VK_FALSE;
	}

	void App::InitWindow()
	{
		//Initialize GLFW
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		Window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
	}

	void App::InitVulkan()
	{
		CreateVKInstance();
		SetupDebugCallback();
		SelectPhysicalDevice();
	}

	void App::CreateVKInstance()
	{
		//Check if the validation layers requested are available
		if (UseValidationLayers && !CheckRequiredValidationLayers())
		{
			throw std::runtime_error("[CreateVKInstance] One or more of the requested validation layers are not present!\nEither install the required layers or disable debug mode!");
		}

		//Struct containing info about our program
		VkApplicationInfo app_info = {};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		//Program releated info(title and version)
		app_info.pApplicationName = Title.c_str();
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		//Engine releated info(name and version)
		app_info.pEngineName = "Cross Platform Vulkan Engine";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_0;

		//Struct containing info about the vkInstance we'll create(app info, extensions and layers)
		VkInstanceCreateInfo instance_info = {};
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		//Pass the app info
		instance_info.pApplicationInfo = &app_info;
		//Get extension info from GLFW
		auto extensions = GetGLFWRequiredExtensions();

		instance_info.enabledExtensionCount = extensions.size();
		instance_info.ppEnabledExtensionNames = extensions.data();
		//Validation layers to enable
		if (UseValidationLayers)
		{
			instance_info.enabledLayerCount = VKValidationLayers.size();
			instance_info.ppEnabledLayerNames = VKValidationLayers.data();
		}
		else
		{
			instance_info.enabledLayerCount = 0;
		}
		
		//Finally create the bloody vkInstance
		if (vkCreateInstance(&instance_info, nullptr, &Instance) != VK_SUCCESS) {
			throw std::runtime_error("[CreateVKInstance] Cannot create Vulkan instance!");
		}
		else
		{
			Log::Message("[CreateVKInstance] Created Vulkan instance successfully!");
		}

		//Obtain extension info
		GetVKExtensionProperties();

		//Check if all extensions required by GLFW exist
		if (!CheckRequiredExtensions(extensions.data(), extensions.size()))
		{
			throw std::runtime_error("[CreateVKInstance] Not all extensions required by GLFW are present!");
		}
		else
		{
			Log::Message("[CreateVKInstance] All required extensions are present!");
		}
	}

	void App::GetVKExtensionProperties()
	{
		//Get the number of extensions
		unsigned int extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		
		//Resize the vector accordingly and fill it with data
		VKExtensionProperites.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, VKExtensionProperites.data());
	
		//Print the number of extensions, their names and versions
		Log::Message("[GetVKExtensionProperties] Vulkan extension count: " + extensionCount);
		for (const auto& ext : VKExtensionProperites)
		{
			Log::Message(ext.extensionName);
		}
	}

	std::vector<const char*> App::GetGLFWRequiredExtensions()
	{
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		if (UseValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}

		return extensions;
	}

	bool App::CheckRequiredExtensions(const char** glfwExtList, const int& size)
	{
		for (int i = 0; i < size; i++)
		{
			bool isOK = false;

			for (const auto& vkExt : VKExtensionProperites)
			{
				if (strcmp(glfwExtList[i], vkExt.extensionName) == 0)
				{
					isOK = true;
				}
			}

			if (isOK == false)
			{
				return false;
			}
		}

		return true;
	}

	bool App::CheckRequiredValidationLayers()
	{
		//Get the number of available layers
		unsigned int layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		//Get the available layers info
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		//Check availableLayers agains the list of layers we requested
		for (const char* reqLayer : VKValidationLayers)
		{
			bool isFound = false;

			for (const auto& availableLayer : availableLayers)
			{
				if (strcmp(reqLayer, availableLayer.layerName) == 0)
				{
					isFound = true;
				}
			}

			//A requested layer is not found
			if (!isFound)
			{
				return false;
			}
		}

		return true;
	}

	void App::SetupDebugCallback()
	{
		//Do not use debug callbacks in release versions
		if (!UseValidationLayers)
		{
			return;
		}

		VkDebugReportCallbackCreateInfoEXT debug_callback_info = {};
		debug_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		//Set up the callback for errors and warnings
		debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		//Set up the pointer to our debug function
		debug_callback_info.pfnCallback = (PFN_vkDebugReportCallbackEXT)VKDebugCallback;
	
		//Try to load the the required extension
		if (VKExtensions::CreateDebugReportCallbackEXT(Instance, &debug_callback_info, nullptr, &Callback) != VK_SUCCESS)
		{
			throw std::runtime_error("[SetupDebugCallback] Error getting the CreateDebugReportCallback extension!");
		}
	}

	void App::SelectPhysicalDevice()
	{
		GPU = VK_NULL_HANDLE;

		//Get the number of GPUs that support Vulkan
		uint32_t GPUCount = 0;
		vkEnumeratePhysicalDevices(Instance, &GPUCount, nullptr);

		//If the GPUCount is still 0, that means that the GPU does not support Vulkan
		if (GPUCount <= 0)
		{
			throw std::runtime_error("[SelectPhysicalDevice] Cannot find any GPU that supports Vulkan.");
		}

		std::vector<VkPhysicalDevice> availableGPUs(GPUCount);
		vkEnumeratePhysicalDevices(Instance, &GPUCount, availableGPUs.data());

		//Select the best GPU
		GPU = SelectBestDevice(availableGPUs);
	}

	VkPhysicalDevice App::SelectBestDevice(std::vector<VkPhysicalDevice> candidates)
	{
		if (candidates.empty())
		{
			throw std::runtime_error("[SelectBestDevice] Empty candidate list given");
		}

		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		int score;

		int maxScore = 0;
		VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties bestDeviceProperties;

		Log::Message("[SelectBestDevice] Selecting best GPU...");

		for (const auto& device : candidates)
		{
			//Only bother with the score if the device has all req. extensions
			if (IsDeviceValid(device))
			{
				//Set initial score
				score = 0;
				//Get GPU properties
				//(eg: name, type, supproted Vulkan version)
				vkGetPhysicalDeviceProperties(device, &properties);
				//Get GPU features
				//(eg: texture compression, 64bit floats, multi-viewport rendering)
				vkGetPhysicalDeviceFeatures(device, &features);

				Log::Message("[SelectBestDevice] Testing " + std::string(properties.deviceName) + " ...");

				//Right now we don't need any specific feature
				score += 1;

				if (score > maxScore)
				{
					bestDevice = device;
					bestDeviceProperties = properties;
				}
			}
		}

		//No device is found. This is bad.
		if (bestDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("[SelectBestDevice] No suitable GPU found!");
		}

		Log::Message("[SelectBestDevice] Selected best device: " + std::string(bestDeviceProperties.deviceName));
		
		return bestDevice;
	}

	bool App::IsDeviceValid(VkPhysicalDevice device)
	{
		return true;
	}

	void App::Update()
	{
		//Handle events
		while (!glfwWindowShouldClose(Window)) {
			glfwPollEvents();
		}
	}

	void App::Run()
	{
		InitWindow();
		InitVulkan();
		Update();
	}
}
