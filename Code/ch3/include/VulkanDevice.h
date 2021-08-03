
#pragma once

#include "Headers.h"
//#include "VulkanQueue.h"
#include "VulkanLED.h"

class VulkanApplication;


// Vulkan exposes one or more devices, each of which exposes one or more queues which may process 
// work asynchronously to one another.The queues supported by a device are divided into families, 
// each of which supports one or more types of functionality and may contain multiple queues with 
// similar characteristics.Queues within a single family are considered compatible with one another, 
// and work produced for a family of queues can be executed on any queue within that family
class VulkanDevice{
public:
	VulkanDevice(VkPhysicalDevice* gpu);
	~VulkanDevice();

public:
    VkDevice							device;		// Logical device
	VkPhysicalDevice*					gpu;		// Physical device
	VkPhysicalDeviceProperties			gpuProps;	// Physical device attributes
    VkPhysicalDeviceMemoryProperties	memoryProperties;

public:
	// Queue
	VkQueue									queue;							// Vulkan Queues object
	std::vector<VkQueueFamilyProperties>	queueFamilyProps;				// Store all queue families exposed by the physical device. attributes
	uint32_t								graphicsQueueIndex;				// Stores graphics queue index
	uint32_t								graphicsQueueWithPresentIndex;  // Number of queue family exposed by device
	uint32_t								queueFamilyCount;				// Device specificc layer and extensions

	// Layer and extensions
	VulkanLayerAndExtension		layerExtension;

public:
	VkResult createDevice(std::vector<const char *>& layers, std::vector<const char *>& extensions);
	void destroyDevice();

	// Get the avaialbe queues exposed by the physical devices
	void getPhysicalDeviceQueuesAndProperties();

	// Query physical device to retrive queue properties
	uint32_t getGraphicsQueueHandle();
};