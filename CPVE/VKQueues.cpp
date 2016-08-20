#include "VKQueues.h"

namespace CPVE {
	QueueFamilyIndices VKQueues::FindAvailableQueueFamilies(const VkPhysicalDevice& gpu)
	{
		QueueFamilyIndices indices;

		//Get the number of supported queue families
		uint32_t familyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, nullptr);

		//Get the queue families data
		std::vector<VkQueueFamilyProperties> queueFamilies(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, queueFamilies.data());

		return indices;
	}
	VKQueues::VKQueues(){
	}


	VKQueues::~VKQueues(){
	}
}
