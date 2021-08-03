
#pragma once
#include "Headers.h"

struct LayerProperties{
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension{
public:
	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();

	/******* LAYER AND EXTENSION MEMBER FUNCTION AND VARAIBLES *******/
	
	// List of layer names requested by the application.
	std::vector<const char *>			appRequestedLayerNames;
	
	// List of extension names requested by the application.
	std::vector<const char *>			appRequestedExtensionNames;
	
	// Layers and corresponding extension list
	std::vector<LayerProperties>		layerPropertyList;

	// Instance/global layer
	VkResult getInstanceLayerProperties();
	
	// Global extensions
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu = NULL);

	// Device based extensions
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);
};