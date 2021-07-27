

/**************** 1. 인스턴스 레이어 속성 지정하기. ****************/
// 인스턴스 레이어의 수
uint32_t instanceLayerCount;
	
// 레이어의 수를 반환하는 함수, 두번째 파라미터는 특정 VkLayerProperties* , 사용가능한 속성레이어를 가져오기위해서는 null지정
vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL); 

VkLayerProperties *layerProperty = NULL;
vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperty);

// 각 인스턴스 레이어에서 사용 가능한 확장판을 가져온다. 
foreach  layerProperty{
	VkExtensionProperties *instanceExtensions;
	res = vkEnumerateInstanceExtensionProperties(layer_name, 
			&instanceExtensionCount, instanceExtensions);
}

/**************** 2. 인스턴스 생성하기 ****************/
VkInstance instance;	// 불칸 인스턴스 객체
VkInstanceCreateInfo instanceInfo = {};

// 인스턴스에서 활성화할 레이어 이름을 지정한다.
instanceInfo.ppEnabledLayerNames = {"VK_LAYER_LUNARG_standard_validation"};

// 인스턴스에서 활성화할 확장판 이름을 지정한다. 
instanceInfo.ppEnabledExtensionNames = {VK_KHR_SURFACE_EXTENSION_NAME, 
										VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

// 인스턴스 객체 생성하기.
vkCreateInstance(&instanceInfo, NULL, &instance);

/**************** 3. 물리 장치 나열하기.  ****************/
VkPhysicalDevice				gpu;		// 물리 장치
uint32_t						gpuCount;	// 수
std::vector<VkPhysicalDevice>	gpuList;	// 목록

// gpu숫자 가져오기.
vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);

// gpu 정보 가져오기.
vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList);

/**************** 4. 장치 생성하기. ****************/
// 큐, 큐타입 가져오기.
vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueCount, queueProperties);

// GPU 또는 물리적장치로 부터 메모리 속성 가져오기.
vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

// 물리적 장치 또는 GPU 속성 가져오기.
vkGetPhysicalDeviceProperties(gpu, &gpuProps);

// 물리적 장치로 논리적 장치 만들기.
VkDevice			device;	// 논리 장치.
VkDeviceCreateInfo	deviceInfo = {};
vkCreateDevice(gpuList[0], &deviceInfo, NULL, &device);

/**************** 5. 프레젠테이션 초기화  ****************/
// 프레젠테이션의 역할은 랜더링된 컨텐츠를 윈도(창)에 표시하는 역할
// 빈 윈도우 만들기. (플랫폼에 따라 다르다.)
CreateWindowEx(...);		/*Windows*/
xcb_create_window(...); 	/*Linux*/

// // WSI 익스텐션 쿼리를 날려서, 함수포인터로저장한다.
// vkCreateSwapchainKHR, vkCreateSwapchainKHR .....

// 추상 화면 객체 생성.
VkWin32SurfaceCreateInfoKHR createInfo = {};
vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, &surface);

// 모든 큐 중에서 프레젠테이션을 지원하는 큐를 선택한다.
foreach Queue in All Queues{
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, queueIndex, surface, &isPresentaionSupported);
	// 큐의 인덱스를 저장한다.
	if (isPresentaionSupported) {
		graphicsQueueFamilyIndex = Queue.index;
		break;
	}
}

// 그래픽스 큐이며, 프레젠테이션을 지원하는 큐를 가져온다. 
vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);

// 전체 화면 포맷 수에 해당하는 메모리를 할당.
uint32_t formatCount;
vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, NULL);

VkSurfaceFormatKHR *surfaceFormats = allocate memory(formatCount * VkSurfaceFormatKHR);

// 화면 형식을 VkSurfaceFormatKHR 객체로 가져온다.
vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, surfaceFormats);

/**************** 6. 스왑체인 만들기. ****************/

// 커맨드 버퍼에 커맨드 레코딩 시작
vkBeginCommandBuffer(cmd, &cmdBufInfo);

// 화면의 용도를 가져온다. (화면크기, 가능한 최소/촤대크기. 변환 기능등..)
vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surfCapabilities);
		
// 화면 프레젠테이션 모드를 가져온다. (드로잉 화면이 업데이트 되는 방법 , 수직동기화 같은..)
vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, NULL);
VkPresentModeKHR presentModes[presentModeCount];
vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes);
		
// 스왑체인을 만든다.  이중버퍼링을 지원하면 드로잉을 위해 값 2와 함께 2개의 이미지를 반환함.
VkSwapchainCreateInfoKHR swapChainInfo = {};
vkCreateSwapchainKHR(device, &swapChainInfo, NULL, &swapChain);
		
// 스왑체인으로 가져온 이미지의 이미지 뷰를 생성한다.
vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
VkImage swapchainImages[swapchainImageCount];
vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages);
		
// 스왑체인 이미지 가져오기.
foreach swapchainImages{
	// 각 구현에 호환되는 레이아웃 설정
	SetImageLayout();

	// 파이프라인 메모리장벽 삽입.
	VkImageMemoryBarrier imgMemoryBarrier = { ... };
	vkCmdPipelineBarrier(cmd,srcStages,destStages,0,0,NULL,0,NULL,1,&imgMemoryBarrier);

	// 이미지 오브잭트를 위한 이미지 뷰를 만든다.
	SwapChainBuffer scBuffer = {...};
	VkImageViewCreateInfo colorImageView = {};
	colorImageView.image = sc_buffer.image;
	vkCreateImageView(device, &colorImageView, NULL, &scBuffer.view);

	// 응용프로그램이 상요할 수 있도록 이미지 뷰를 저장한다.
	buffers.push_back(scBuffer);
}

/**************** 7. 깊이 버퍼( 뎁스버퍼 ) 만들기  ****************/

// 물리적 장치에서 지원하는 포맷 기능이 뭔지 쿼리를 날려서 확인한다.
vkGetPhysicalDeviceFormatProperties(gpuList, depthFormat, &properties);

// 이미지 객체 생성
vkCreateImage(device, &imageInfo, NULL, &imageObject);

// 이미지 리소스를 위한 메모리 요구사항 가져오기.
vkGetImageMemoryRequirements(device, imageObject, &memRequirements);

// 메모리 할당.
VkDeviceMemory	mem;
vkAllocateMemory(device, &memAlloc, NULL, &mem);

// 메모리 바인딩.
vkBindImageMemory(device, imageObject, mem, 0);

// 구현에 호환되는 이미지 레이아웃을 설정한다.
SetImageLayout(. . .)

// 지정된 이미지 레이아웃이 사용되기 전에 생성됨을 보장하기위해 파이프라인 장벽 삽입.
vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgPipelineBarrier);

// 생성된 뎁스버퍼 오브젝트로 이미지 뷰를 생성한다.
VkImageViewCreateInfo imgViewInfo = { ... };
imgViewInfo.image = imageObject;
vkCreateImageView(device, &imgViewInfo, NULL, &view);


/**************** 8. 쉐이더 모듈 바인딩 하기. ****************/
// opengl쉐이더 코드랑 똑같은듯..?
// 쉐이더코드는 glslangValidator.exe 라는 프로그램이 컴파일해서 SPIR-V로 변환한다.
#version 450
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;
layout (location = 0) out vec4 outColor;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
	outColor = inColor;
	gl_Position = pos;
	gl_Position.y = -gl_Position.y;
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}

#version 450
layout (location = 0) in vec4 color;
layout (location = 0) out vec4 outColor;
void main() {
	outColor = color;
};

VkPipelineShaderStageCreateInfo vtxShdrStages = {....};
VkShaderModuleCreateInfo moduleCreateInfo = { ... };
moduleCreateInfo.pCode = spvVertexShaderData/*spvVertexShaderData에는 버텍스 쉐이더의 이진 데이터가 들어가있다.*/;
// 장치에 쉐이더 모듈을 생성한다.
vkCreateShaderModule(device, &moduleCreateInfo, NULL, &vtxShdrStages.module);

/**************** 9. 디스크립터 레이아웃과 파이프라인 레이아웃 생성하기 ****************/

// 디스크립터 레이아웃은 쉐이더와 연결된 정보를 정의한다.
VkDescriptorSetLayoutBinding layoutBindings[2];
layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
layoutBindings[0].binding		 = 0;
layoutBindings[0].stageFlags	 = VK_SHADER_STAGE_VERTEX_BIT;
layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
layoutBindings[1].binding		 = 0;
layoutBindings[1].stageFlags     = VK_SHADER_STAGE_FRAGMENT_BIT;

// 레이아웃 바인딩을 사용하여 디스크립터 셋 레이아웃을 생성한다.
VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
descriptorLayout.pBindings					     = layoutBindings;
VkDescriptorSetLayout descLayout[2];
vkCreateDescriptorSetLayout(device, &descriptorLayout, NULL, descLayout.data());

// 디스크립터 레이아웃을 사용하여 파이프라인 레이아웃을 만든다.
VkPipelineLayoutCreateInfo pipelineLayoutCI = { ... };
pipelineLayoutCI.pSetLayouts				= descLayout.data();
vkCreatePipelineLayout(device, &pipelineLayoutCI, NULL, &pipelineLayout);


/**************** 10. 랜더 패스 ****************/

// 색상과 깊이 버퍼를 위한 두개의 Attachment를 정의한다.
VkAttachmentDescription attachments[2];
attachments[0].format = colorImageformat;
attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR 
							: VK_ATTACHMENT_LOAD_OP_DONT_CARE;
attachments[1].format = depthImageformat;
attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

VkAttachmentReference colorReference, depthReference = {...};

// 서브패스의 정의와 색상, 깊이 이미지를 사용한다.
VkSubpassDescription subpass	= {};
subpass.pColorAttachments		= &colorReference;
subpass.pDepthStencilAttachment = &depthReference;

// 랜더패스 제어 구조체 정의
VkRenderPassCreateInfo rpInfo	= { &attachments,&subpass ...};

VkRenderPass renderPass; // 랜더패스 오브젝트 생성
vkCreateRenderPass(device, &rpInfo, NULL, &renderPass);

/**************** 11. 프레임버퍼 만들기. ****************/
VkImageView attachments[2];						// [0] for color, [1] for depth
attachments[1] = Depth.view;

VkFramebufferCreateInfo fbInfo = {};
fbInfo.renderPass				= renderPass;	// 랜더패스 오브젝트
fbInfo.pAttachments				= attachments;	// 이미지 뷰 Attachments
fbInfo.width					= width;		// 프레임 버퍼의 너비
fbInfo.height					= height;		// 프레임버퍼의 너비

//  프레임 버퍼 객체를 위한 메모리 할당
// 스왚 체인의 각 이미지에 대해 하나의 프레임 버퍼가 있따. 
VkFramebuffer framebuffers[number of draw surface image in swap chain];

foreach(drawing buffer in swapchain) {
	attachments[0] = currentSwapChainDrawImage.view;
	vkCreateFramebuffer(device, &fbInfo, NULL, &framebuffers[i]);
}

/****************  12. GPU 메모리에 버텍스 데이터 채우기 ****************/

static const VertexWithColor triangleData[] = {
	/*{  x ,     y,    z,    w,    r,    g,    b,   a },*/
	{ 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0 },
	{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0 },
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0 },
};

VkBuffer				buffer;
VkMemoryRequirements	mem_requirement;
VkDeviceMemory			deviceMemory;

// 버퍼객체를 만들고 필요한 메모리와 할당을 쿼리한다.
VkBufferCreateInfo buffer_info = { ... };
vkCreateBuffer(device, &buffer_info, NULL, &buffer);

vkGetBufferMemoryRequirements(device, buffer, &mem_requirement);

VkMemoryAllocateInfo alloc_info = { ... };

vkAllocateMemory(device, &alloc_info, NULL, &(deviceMemory));

// triangleData를 매핑과 언매핑을 사용하여 GPU에 복사한다.
uint8_t *pData;
vkMapMemory(device, deviceMemory, 0, mem_requirement.size, 0, &pData);
memcpy(pData, triangleData, dataSize); /**** Copying data ****/
vkUnmapMemory(device, deviceMemory);

// 할당된 메모리 바인딩
vkBindBufferMemory(device, buffer, deviceMemory, 0);

/****************  13. 버택스 바인딩  ****************/
VkVertexInputBindingDescription viBinding;
viBinding.binding = 0;
viBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
viBinding.stride = sizeof(triangleData) /*data Stride*/;

VkVertexInputAttributeDescription viAttribs[2];
viAttribs[0].binding = 0;
viAttribs[0].location = 0;
viAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
viAttribs[0].offset = 0;
viAttribs[1].binding = 0;
viAttribs[1].location = 1;
viAttribs[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
viAttribs[1].offset = 16;

/****************  14. 스테이트 정의하기  ****************/
// Vertex Input state
VkPipelineVertexInputStateCreateInfo vertexInputStateInfo	= { ... };
vertexInputStateInfo.vertexBindingDescriptionCount			= 1;
vertexInputStateInfo.pVertexBindingDescriptions				= &viBinding;
vertexInputStateInfo.vertexAttributeDescriptionCount		= 2;
vertexInputStateInfo.pVertexAttributeDescriptions			= viAttribs;

// 동적 스테이트 Dynamic states
VkPipelineDynamicStateCreateInfo dynamicState				= { ... };
// 입력 어셈블리 스테이트 제어 구조체  Input assembly state control structure
VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo	= { ... };
// 레스터화 스테이트 제어 구조체 Rasterization state control structure
VkPipelineRasterizationStateCreateInfo rasterStateInfo		= { ... };
// 색상 블랜딩 첨부 스테이트 제어 구조체 Color blend Attachment state control structure
VkPipelineColorBlendAttachmentState colorBlendSI			= { ... };
// 색상 블랜딩 스테이트 제어 구조체 Color blend state control structure
VkPipelineColorBlendStateCreateInfo colorBlendStateInfo		= { ... };
// 뷰포트 스테이트 제어 구조체 View port state control structure
VkPipelineViewportStateCreateInfo viewportStateInfo			= { ... };
// 깊이 스텐실 스테이트 제어 구조체 Depth stencil state control structure
VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = { ... };
// 멀티 샘플링 스테이트 제어 구조체  Multi sampling state control structure
VkPipelineMultisampleStateCreateInfo   multiSampleStateInfo = { ... };

/****************  15. 그래픽스 파이프라인 만들기  ****************/
VkPipelineCache	pipelineCache; 
VkPipelineCacheCreateInfo pipelineCacheInfo;
vkCreatePipelineCache(device, &pipelineCacheInfo, NULL, &pipelineCache);

// 그래픽스 파이프라인 제어 구조체를 정의한다.
VkGraphicsPipelineCreateInfo pipelineInfo;
pipelineInfo.layout					= pipelineLayout;
pipelineInfo.pVertexInputState		= &vertexInputStateInfo;
pipelineInfo.pInputAssemblyState	= &inputAssemblyInfo;
pipelineInfo.pRasterizationState	= &rasterStateInfo;
pipelineInfo.pColorBlendState		= &colorBlendStateInfo;
pipelineInfo.pMultisampleState		= &multiSampleStateInfo;
pipelineInfo.pDynamicState			= &dynamicState;
pipelineInfo.pViewportState			= &viewportStateInfo;
pipelineInfo.pDepthStencilState		= &depthStencilStateInfo;
pipelineInfo.pStages				= shaderStages;
pipelineInfo.stageCount				= 2;
pipelineInfo.renderPass				= renderPass;

// 그래픽스 파이프라인 생성
vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, NULL, &pipeline);

/****************  16. 드로잉 화면 획득하기  ****************/

// 획등한 드로잉 이미지를 동기화하기 위해 세마포어 정의
// 드로잉이 완료되면 드로잉 이미지를 획득.
VkSemaphore imageAcquiredSemaphore ;
VkSemaphoreCreateInfo imageAcquiredSemaphore CreateInfo = { ... };
imageAcquiredSemaphore CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo,
					NULL, &imageAcquiredSemaphore );

// 다음으로 사용가능한 스왑체인 이미지의 인덱스를 가져온다.
vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, 
						imageAcquiredSemaphore , NULL, &currentSwapChainImageIndex);

/****************  17. 랜더 패스 제어 구조체 준비하기  ****************/

// 지우기 위한 생상, 깊이 , 스텐실 값 정의
const VkClearValue clearValues[2] = {
	[0] = { .color.float32 = { 0.2f, 0.2f, 0.2f, 0.2f } },
	[1] = { .depthStencil = { 1.0f, 0 } },
};

// 프레임 버퍼를 위한 랜더 패스 실행 데이터 구조체
VkRenderPassBeginInfo renderPassBegin;
renderPassBegin.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
renderPassBegin.pNext		= NULL;
renderPassBegin.renderPass	= renderPass;
renderPassBegin.framebuffer = framebuffers[currentSwapchainImageIndex];
renderPassBegin.renderArea.offset.x			= 0;
renderPassBegin.renderArea.offset.y			= 0;
renderPassBegin.renderArea.extent.width		= width;
renderPassBegin.renderArea.extent.height	= height;
renderPassBegin.clearValueCount				= 2;
renderPassBegin.pClearValues				= clearValues;

		
/****************  18. 랜더 패스 실행하기 ****************/
/**** 랜더패스 시작 ****/
vkCmdBeginRenderPass(cmd, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE); 
		
vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline); // 파이프라인 바인딩
const VkDeviceSize offsets[1] = { 0 };
vkCmdBindVertexBuffers(cmd, 0, 1, &buffer, offsets);	 // 삼각형의 버퍼 데이터 바인딩
vkCmdSetViewport(cmd, 0, NUM_VIEWPORTS, &viewport); // viewport = {0, 0, 500, 500, 0 ,1}
vkCmdSetScissor(cmd, 0, NUM_SCISSORS, &scissor);	 // scissor  = {0, 0, 500, 500}
vkCmdDraw(cmd, 3, 1, 0, 0);			 // 3 vertices, 1 instance, 0번째가 첫 번째 버텍스다.

/**** 랜더패스 종료 ****/
vkCmdEndRenderPass(cmd);

// 파이프라인 장벽
setImageLayout()
vkCmdPipelineBarrier(cmd, ....);

/**** 여기서 커맨드 버퍼 레코딩이 종료된다. ****/
vkEndCommandBuffer(cmd);

/****************  19. 큐 제출하기 ****************/

VkFenceCreateInfo fenceInfo = { ... }; VkFence drawFence;
// cmdBuffer 처리의 완료를 보장하기 위한 장벽(팬스)
vkCreateFence(device, &fenceInfo, NULL, &drawFence);

// 커맨드 버퍼 제출 제어 구조체를 채운다.
VkSubmitInfo submitInfo[1]			= { ... };
submitInfo[0].pNext					= NULL;
submitInfo[0].sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
submitInfo[0].pWaitSemaphores		= &imageAcquiredSemaphore ;
submitInfo[0].commandBufferCount	= 1;
submitInfo[0].pCommandBuffers		= &cmd;

// 실행으 ㄹ위해 커맨드 버퍼를 큐에 제출한다.
vkQueueSubmit(queue, 1, submitInfo, NULL);

/****************  20. 윈도우에 드로우 결과 출력하기. ****************/

// 프레젠테이션 제어 구조체 정의
VkPresentInfoKHR present = { ... };
present.sType			 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
present.pNext			 = NULL;
present.swapchainCount	 = 1;
present.pSwapchains		 = &swapChain;
present.pImageIndices	 = &swapChainObjCurrent_buffer;

// 제출된 모든 커맨드 버퍼가 처리됐는지 점검한다.
do {
	res = vkWaitForFences(device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
} while (res == VK_TIMEOUT);

//프레젠테이션을 위해 스왑체인 이미지를 프레젠테이션 큐에 넘긴다.
vkQueuePresentKHR(queue, &present);

// 동기화 객체를 삭제한다.
vkDestroySemaphore(device, imageAcquiredSemaphore , NULL);
vkDestroyFence(device, drawFence, NULL);