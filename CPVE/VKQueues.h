#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace CPVE {

	//Struc will be used to represent which queue families/types we have access to 
	struct QueueFamilyIndices {
		int GraphicsFamily = -1;

		bool IsComplete()
		{
			return GraphicsFamily >= 0;
		}
	};

	class VKQueues {
	public:

		static QueueFamilyIndices FindAvailableQueueFamilies(const VkPhysicalDevice& gpu);

		VKQueues();
		~VKQueues();
	};
}

