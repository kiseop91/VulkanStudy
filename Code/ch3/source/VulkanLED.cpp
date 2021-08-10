#include "VulkanLED.h"
#include "VulkanApplication.h"

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	uint32_t	instanceLayerCount;		// 인스턴스 레이어 수
	std::vector<VkLayerProperties>	layerProperties;		// 레이어 속성을 저장하기위한 컨테이너
	VkResult						result;					// Vulkan APi 명령 결과 리턴값.

	// 모든 레이어를 쿼리한다.
	do {
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);

		if (result)
			return result;

		if (instanceLayerCount == 0)
			return VK_INCOMPLETE; 

		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	// 모든 레이어를 쿼리 후 저장.
	std::cout << "\nInstanced Layers" << std::endl;
	std::cout << "===================" << std::endl;
	for (auto globalLayerProp: layerProperties) {
		std::cout <<"\n"<< globalLayerProp.description <<"\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.layerName <<"\n";

		LayerProperties layerProps;
		layerProps.properties = globalLayerProp;

		// layer properties에 해당하는 인스턴스 레벨 확장을 가져온다.
		result = getExtensionProperties(layerProps);

		if (result){
			continue;
		}

		layerPropertyList.push_back(layerProps);
		// 각 인스턴스 레이어에대한 확장판이름을 출력한다.
		for (auto j : layerProps.extensions) {
			std::cout << "\t\t|\n\t\t|---[Layer Extension]--> " << j.extensionName << "\n";
		}
	}
	return result;
}

/*
* 장치확장 만들기.
*/
 
VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
	VkResult result;

	// getInstanceLayerProperties와 거의 동일..
	std::cout << "Device extensions" << std::endl;
	std::cout << "===================" << std::endl;
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	std::vector<LayerProperties>* instanceLayerProp = &appObj->GetInstance()->instanceObj.layerExtension.layerPropertyList;
	for (auto globalLayerProp : *instanceLayerProp) {
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp.properties;

		if (result = getExtensionProperties(layerProps, gpu))
			continue;

		std::cout << "\n" << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
		layerPropertyList.push_back(layerProps);

		if (layerProps.extensions.size()) {
			for (auto j : layerProps.extensions) {
				std::cout << "\t\t|\n\t\t|---[Device Extesion]--> " << j.extensionName << "\n";
			}
		}
		else {
			std::cout << "\t\t|\n\t\t|---[Device Extesion]--> No extension found \n";
		}
	}
	return result;
}

// 확장판과 확장판의 속성을 인스턴스와 장치 레벨에서 가져온다.
// gpu를 넘겨서 장치 레벨의 확장을 가져오거나, NULL 파라미터를 넘겨 인스턴스 레벨의 확장판은 정하아여 가져온다.
VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu)
{
	uint32_t	extensionCount;								 // 레이어당 확장판 수
	VkResult	result;										 
	char*		layerName = layerProps.properties.layerName; // 레이어명 

	do {
		//  모든 레이어.
		if(gpu)
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, NULL);
		else
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, NULL);

		if (result || extensionCount == 0)
			continue;

		layerProps.extensions.resize(extensionCount);

		// 모든 확장.
		if (gpu)
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
		else
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
	} while (result == VK_INCOMPLETE);

	return result;
}


void VulkanLayerAndExtension::destroyDebugReportCallback()
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	VkInstance& instance	= appObj->instanceObj.instance;
	dbgDestroyDebugReportCallback(instance, debugReportCallback, NULL);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanLayerAndExtension::debugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject, size_t location, int32_t msgCode,
	const char *layerPrefix, const char *msg, void *userData) {

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] ERROR: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] WARNING: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] INFORMATION: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] PERFORMANCE: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] DEBUG: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else {
		return VK_FALSE;
	}

	fflush(stdout);
	return VK_TRUE;
}

/*
Inspects the incoming layer names against system supported layers, theses layers are not supported
then this function removed it from layerNames allow
*/
VkBool32 VulkanLayerAndExtension::areLayersSupported(std::vector<const char *> &layerNames)
{
	uint32_t checkCount = (uint32_t)layerNames.size();
	uint32_t layerCount = (uint32_t)layerPropertyList.size();
	std::vector<const char*> unsupportLayerNames;
	for (uint32_t i = 0; i < checkCount; i++) {
		VkBool32 isSupported = 0;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (!strcmp(layerNames[i], layerPropertyList[j].properties.layerName)) {
				isSupported = 1;
			}
		}

		if (!isSupported) {
			std::cout << "No Layer support found, removed from layer: " << layerNames[i] << std::endl;
			unsupportLayerNames.push_back(layerNames[i]);
		}
		else {
			std::cout << "Layer supported: " << layerNames[i] << std::endl;
		}
	}

	for (auto i : unsupportLayerNames) {
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end()) layerNames.erase(it);
	}

	return true;
}


VkResult VulkanLayerAndExtension::createDebugReportCallback()
{
	VkResult result;

	VulkanApplication* appObj	= VulkanApplication::GetInstance();
	VkInstance* instance		= &appObj->instanceObj.instance;

	// vkCreateDebugReportCallbackEXT API 가져오기 . 콜백함수로 바인딩한다.
	dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugReportCallbackEXT");
	if (!dbgCreateDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgCreateDebugReportCallback function\n";

	// vkDestroyDebugReportCallbackEXT API 가져오기.
	dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugReportCallbackEXT");
	if (!dbgDestroyDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function\n";

	dbgReportCreateInfo.sType		= VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	dbgReportCreateInfo.pfnCallback = debugFunction;
	dbgReportCreateInfo.pUserData	= NULL;
	dbgReportCreateInfo.pNext		= NULL;
	dbgReportCreateInfo.flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
									  VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
									  VK_DEBUG_REPORT_ERROR_BIT_EXT |
									  VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	result = dbgCreateDebugReportCallback(*instance, &dbgReportCreateInfo, NULL, &debugReportCallback);
	if (result == VK_SUCCESS) {
		std::cout << "Debug report callback object created successfully\n";
	}
	return result;
}
