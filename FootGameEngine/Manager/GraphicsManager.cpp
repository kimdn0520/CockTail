#include "GamePch.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "../../FootGraphicsEngine/Manager/ResourceManager.h"

#include "SceneManager.h"
#include "Object/GameObject.h"
#include "Component/BoxCollider.h"
#include "Component/Camera.h"
#include "Component/MeshRenderer.h"
#include "SceneBase.h"
#include "UIManager.h"

namespace GameEngineSpace
{
	// 스태틱변수 선언
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 여기서 Dll을 로드해줍니다. => 암시적 링킹이기 때문에 해당 부분이 필요하지 않습니다.
			// TODO : 내 구조에 맞게 재 설계.

		// 초기 리사이즈를 제대로 하기위한 클라이언트 정보 얻어오기.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// 렌더러를 생성해봅시다.
		renderer.reset(CreateRenderer());


		if (renderer->Initialize(hWnd, _width, _height) != true)
		{
			printf("Create Renderer FAIL!\n");
			system("PAUSE");
			return false;
		}
		
		printf("Create Renderer Clear\n");

		static uint32 nowWidth = _width;
		static uint32 nowHeight = _height;

		// 리사이즈 펑션을 정합니다..
		// 윈도우 매니저 불러오기.
		WindowManager::GetInstance()->SetOnResizeFunc([&](uint32 width, uint32 height)
			{
				renderer->SetClientSize(width, height);
				renderer->OnResize();
				UIManager::GetInstance()->OnResize(width, height);
			}
		);

		WindowManager::GetInstance()->SetOnMouseMoveFunc([&](float x, float y)
			{
				InputManager::GetInstance()->SetMousePos(x, y);
			}
		);

		printf("Graphics Manager Init Clear\n");
		
		return true;
	}

	bool GraphicsManager::LateInit()
	{
		renderer->LateInitialize();

		return true;
	}

	void GraphicsManager::Update()
	{
		/// 화면 갱신에 있어서 참조할 사항을 날려줍니다..
		///
	}

	void GraphicsManager::Release()
	{
		if (renderer != nullptr)
			renderer->Finalize();
		renderer.reset();

		/*if (graphicsDLL != nullptr)
			FreeLibrary(graphicsDLL);*/
	}

	void GraphicsManager::PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassDirectionalLight(color, direction, power, lightViewProj);
	}

	void GraphicsManager::PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, bool isShadow, std::vector<SimpleMath::Matrix> lightViewProj)
	{
		renderer->PassPointLight(color, position, power, range, isShadow, lightViewProj);
	}

	void GraphicsManager::PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, SimpleMath::Vector3 direction, float power, float innerSpotAngle, float outerSpotAngle, float range, bool
	                                    isShadow, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassSpotLight(color, position, direction, power, innerSpotAngle, outerSpotAngle, range, isShadow, lightViewProj);
	}

	void GraphicsManager::PassAmbientSkyColor(SimpleMath::Vector4 color)
	{
		renderer->PassAmbientSkyColor(color);
	}

	void GraphicsManager::CreateUITest(HWND hWnd)
	{
		// 테스트용 스프라이트
				// 해당 부분을 렌더러에 만들어 준다.
		auto resourceManager = ResourceManager::GetInstance();

		resourceManager->LoadCubeMesh();

		/*uint64 id = resourceManager->Load<Texture>("Smol", L"Resources/UI/Smol.png");

		testSprite->SetTexture(resourceManager->Get<Texture>(id));
		testSprite->SetPosition({200.f, 100.f, 0.1f});
		testSprite->SetWidth(200.f);
		testSprite->SetHeight(200.f);*/
	}

	void GraphicsManager::UIRender(float tick)
	{
		//testCanvas->Render(tick);
	}

	std::shared_ptr<Texture> GraphicsManager::LoadTexture(std::wstring path)
	{
		auto resourceManager = ResourceManager::GetInstance();

		return resourceManager->LoadTexture(path);
	}

	void GraphicsManager::MakeMesh(std::string name, std::vector<FBXBinaryData::VertexData> vertex, std::vector<std::vector<unsigned int>> indices)
	{
		ResourceManager::GetInstance()->MakeMesh(name, vertex, indices);
	}

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}
