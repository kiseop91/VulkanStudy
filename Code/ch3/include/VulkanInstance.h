
#pragma once

#include "VulkanLED.h"

class VulkanInstance{
public:
    VulkanInstance(){}
    ~VulkanInstance(){}

public:
    // 벌칸 객체 
    VkInstance	instance;

	// 벌칸 인스턴스의 지정레이어와 확장판
	VulkanLayerAndExtension	layerExtension;

public:
	VkResult createInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions, const char* applicationName);
	void destroyInstance();
};
