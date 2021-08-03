#pragma once
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanLED.h"

class VulkanApplication
{
private:
	VulkanApplication();

public:
	~VulkanApplication();

private:
    static std::unique_ptr<VulkanApplication> instance;
    static std::once_flag onlyOnce;

public:
    static VulkanApplication* GetInstance();

	void initialize();				// 초기화 및 리소스 할당
	void prepare();					// 리소스 준비
	void update();					// 데이터 업데이트
	bool render();					// 프리미티브 랜더
	void deInitialize();			// 리소스 해제

private:
	VkResult createVulkanInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions,  const char* applicationName);
	VkResult handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions);
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpus);

public:
    VulkanInstance  instanceObj;

	VulkanDevice*      deviceObj;
};