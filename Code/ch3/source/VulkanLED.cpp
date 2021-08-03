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

