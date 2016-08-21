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

		int i = 0;

		for (const auto& family : queueFamilies)
		{
			//If we can have graphics command queues
			if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = i;
			}

			i++;
		}

		return indices;
	}
	VKQueues::VKQueues(){
	}


	VKQueues::~VKQueues(){
	}
}
