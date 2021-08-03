

#include "VulkanInstance.h"

VkResult VulkanInstance::createInstance(std::vector<const char *>& layers, std::vector<const char *>& extensionNames, char const*const appName)
{
	layerExtension.appRequestedExtensionNames	= extensionNames;
	layerExtension.appRequestedLayerNames		= layers;
	
	// 확장판 정보를 설정한다.
	VkApplicationInfo appInfo	= {};
	appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext				= NULL;
	appInfo.pApplicationName	= appName;
	appInfo.applicationVersion	= 1;
	appInfo.pEngineName			= appName;
	appInfo.engineVersion		= 1;
	appInfo.apiVersion			= VK_MAKE_VERSION(1, 0, 0);

	// 벌칸 인스턴스를 생성하기위한 정보 구조체 정의
	VkInstanceCreateInfo instInfo	= {};
	instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext					= VK_NULL_HANDLE;
	instInfo.flags					= 0;
	instInfo.pApplicationInfo		= &appInfo;

	// 활성화할 레이어 이름 목록을 지정한다.
	instInfo.enabledLayerCount		= (uint32_t)layers.size();
	instInfo.ppEnabledLayerNames	= layers.size() ? layers.data() : NULL;

	// 애플리케이션에서 사용할 확장 목록을 지정한다.
	instInfo.enabledExtensionCount	= (uint32_t)extensionNames.size();
	instInfo.ppEnabledExtensionNames = extensionNames.size() ? extensionNames.data() : NULL;

	VkResult result = vkCreateInstance(&instInfo, NULL, &instance);
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(instance, NULL);
}
