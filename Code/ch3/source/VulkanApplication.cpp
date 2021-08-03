#include "VulkanApplication.h"

std::unique_ptr<VulkanApplication> VulkanApplication::instance;
std::once_flag VulkanApplication::onlyOnce;

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;


VulkanApplication::VulkanApplication() 
{
	// 애플리케이션 시작 시 인스턴스 계층 열거
	instanceObj.layerExtension.getInstanceLayerProperties();
	deviceObj = NULL;
}

VulkanApplication::~VulkanApplication()
{

}

// 싱글턴 오브젝트 리턴
VulkanApplication* VulkanApplication::GetInstance(){
    std::call_once(onlyOnce, [](){instance.reset(new VulkanApplication()); });
    return instance.get();
}

VkResult VulkanApplication::createVulkanInstance( std::vector<const char *>& layers, std::vector<const char *>& extensionNames, const char* applicationName)
{
	return instanceObj.createInstance(layers, extensionNames, applicationName);
}

// 이 함수는 논리적 장치를 생성하는 역할을 합니다.
// 논리적 장치 생성 프로세스에는 다음 단계가 필요합니다.
// 1. 물리적 장치 특정 계층 및 해당 확장 가져오기 [선택 사항]
// 2. 사용자 정의 VulkanDevice 객체 생성
// 3. 이 물리적 장치에서 활성화해야 하는 계층 및 확장 목록을 제공합니다.
// 4. 물리적 장치 또는 GPU 속성 가져오기
// 5. 물리적 장치 또는 GPU에서 메모리 속성 가져오기
// 6. 물리적 장치 노출 대기열 및 관련 속성 쿼리
// 7. 그래픽 큐의 핸들 가져오기
// 8. 논리적 장치를 생성하고 그래픽 대기열에 연결합니다.

// 장치 및 대기열 생성
VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions )
{

	// 사용자가 관리할 Vulkan Device 객체를 정의합니다.
	// 물리적 및 논리적 장치와 해당 대기열 및 속성
	deviceObj = new VulkanDevice(gpu);
	if (!deviceObj){
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	// 장치 사용 가능한 레이어와 확장자를 출력합니다.
	deviceObj->layerExtension.getDeviceExtensionProperties(gpu);

	// 물리적 장치 또는 GPU 속성 가져오기
	vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProps);

	// 물리적 장치 또는 GPU에서 메모리 속성을 가져옵니다.
	vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

	// 물리적 장치 및 해당 속성에서 사용 가능한 대기열을 쿼리합니다.
	deviceObj->getPhysicalDeviceQueuesAndProperties();

	// 그래픽 파이프라인을 지원하는 대기열을 검색합니다.
	deviceObj->getGraphicsQueueHandle();

	// 논리적 장치를 생성하고 이 장치가 그래픽 대기열에 연결되어 있는지 확인
	return deviceObj->createDevice(layers, extensions);
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList)
{
	uint32_t gpuDeviceCount;

	VkResult result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, NULL);
	assert(result == VK_SUCCESS);

	assert(gpuDeviceCount);

	gpuList.resize(gpuDeviceCount);

	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanApplication::initialize()
{
	char title[] = "Hello World!!!";
	// 지정된 레이어 및 확장 이름으로 Vulkan 인스턴스를 생성
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	// 시스템의 물리적 장치 목록 가져오기
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	if (gpuList.size() > 0) {
		handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
	}
}

void VulkanApplication::deInitialize()
{
	deviceObj->destroyDevice();
	instanceObj.destroyInstance();
}

void VulkanApplication::prepare()
{
}

void VulkanApplication::update()
{
}

bool VulkanApplication::render() 
{
	return true;
}


