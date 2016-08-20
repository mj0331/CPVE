#pragma once

#include "Log.hpp"
#include "VDeleter.hpp"
#include "VKExtensions.h"
#include "VKQueues.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <cstring>

namespace CPVE {
	class App
	{
	private:
		GLFWwindow* Window;
		int Width;
		int Height;
		std::string Title;

		//Use this to specify which validation layers to use
		//N.B.: "VK_LAYER_LUNARG_standard_validation" requires the LunarG Vulkan SDK
		const std::vector<const char*> VKValidationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

#ifdef NDEBUG
		//Disable validation layers in release builds
		const bool UseValidationLayers = false;
#else
		//Enable validation layers in debug builds
		const bool UseValidationLayers = true;
#endif
		//Static debug callbac function(ref: https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers )
		static VkBool32 VKDebugCallback(VkDebugReportFlagsEXT flags,VkDebugReportObjectTypeEXT objType,uint64_t obj,size_t location,int32_t code,const char* layerPrefix,const char* msg,void* userData);
		//Set up the callback and a cleanup function
		VDeleter<VkDebugReportCallbackEXT> Callback{ Instance, VKExtensions::DestroyDebugReportCallbackEXT };
		//Vulkan instance
		VDeleter<VkInstance> Instance{ vkDestroyInstance };
		//Vulkan physical device(implicitly deleted when Instance is destroyed, no wrapper needed)
		VkPhysicalDevice GPU;

		std::vector<VkExtensionProperties> VKExtensionProperites;

		void InitWindow();
		void InitVulkan();
		void CreateVKInstance();
		void GetVKExtensionProperties();

		std::vector<const char*> GetGLFWRequiredExtensions();
		bool CheckRequiredExtensions(const char** glfwExtList, const int& size);
		bool CheckRequiredValidationLayers();
		void SetupDebugCallback();

		void SelectPhysicalDevice();
		//This function is used to pick the "best" GPU based on a score
		VkPhysicalDevice SelectBestDevice(std::vector<VkPhysicalDevice> candidates);
		//This function returns true if a device meets all the requirements(eg. all required extensions are present)
		bool IsDeviceValid(VkPhysicalDevice device);

		void Update();
	public:
		App(int w=960, int h=540, std::string title="Vulkan 1.0.17.0");
		~App();

		void Run();
	};
}

