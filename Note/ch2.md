
- CMake DownLoad : https://cmake.org/download/
- LunarG DownLoad : https://vulkan.lunarg.com/sdk/home#windows

--- 

- 2주차 내용

Vulkan 설치 및 Sudo코드를 작성하며, 전체내용을 훑어보기.  
실제 API를 사용하여 작성하므로.. 해당 함수명을 API문서에서 검색하면서 보면 이해에 도움이된다.  
이 챕터에서 작성하는 코드는 한 주차씩 단계별로 상세히 알아가므로, 완벽하게 이해하는 것보다 전체적인 큰그림을 그려보면 좋을듯함.  
슈도코드를 보면, Vulkan의 모든객체는 생성을 위한 팩토리함수가 따로 있으며, 구조체를 통해 다양한 속성을 지정할 수 있다. ( 생성할 때 지정한 속성을 구조체로 넘기는 방식이다. )

### 삼각형을 그리려면.. 
1. VkInstance 생성
2. 지원되는 그래픽 카드 선택 (VkPhysicalDevice)
3. Draw 및 프레젠테이션을 위해 VkDeviceand와 VkQueue 만들기
4. window, window surface 및 swap chain 만들기
5. swap chain images를 VkImageView로 래핑하기.
6. 랜더타겟 및 사용방법을 지정하는 RenderPass 생성하기
7. RenderPass용 프레임 버퍼 생성
8. 그래픽 파이프라인 설정
9. 가능한 모든 스왑 체인 이미지에 대해 Draw 명령으로 Command Buffer를 할당하고 기록.
10. 획득한 이미지로 프레임을 그리고, CommandBuffer 제출 및 스왑체인으로부터 이미지 받기. 

실습 코드는 [Code](../Code/ch2/ch2.cpp)폴더 참조.