
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

	//디버깅 레이어 활성화하기.
	VkBool32 areLayersSupported(std::vector<const char *> &layerNames);
	VkResult createDebugReportCallback();
	void	destroyDebugReportCallback();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction( VkFlags msgFlags, 
														 VkDebugReportObjectTypeEXT objType,
														 uint64_t srcObject, 
														 size_t location, 
														 int32_t msgCode,
														 const char *layerPrefix,
														 const char *msg, 
														 void *userData);

private:
	// 콜백함수
	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	VkDebugReportCallbackEXT debugReportCallback;
public:
	VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
};