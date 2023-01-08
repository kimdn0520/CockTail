#include "GamePch.h"
#include "UIManager.h"

#include "GraphicsManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include "Timer.h"

namespace GameEngineSpace
{
	std::shared_ptr<UIManager> UIManager::instance = nullptr;

	std::shared_ptr<UIManager> UIManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<UIManager>();

		return instance;
	}

	void UIManager::Init()
	{
		graphicsInstance = GraphicsManager::GetInstance();
		inputInstance = InputManager::GetInstance();

		auto renderer = graphicsInstance->GetRenderer();
		auto width = WindowManager::GetInstance()->GetClientWidth();
		auto height = WindowManager::GetInstance()->GetClientHeight();

		nowWidth = width;
		nowHeight = height;

		mainCanvas = renderer->CreateCanvas("MainCanvas", nowWidth, nowHeight);

		// 각종 캔버스들을 이 아래에서 미리 생성해둔다.
		mainCanvas->CreateCanvasUI("TestCanvas", width, height);
		mainCanvas->CreateCanvasUI("PopUpCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });

		// ImGUI포인터를 미리 보관해둔다.
		imgui = ImGUIManager::GetInstance();
	}

	// WIC 텍스쳐 로드 가능한 시점에 호출
	void UIManager::CreateCanvasAll()
	{
		CreateTestCanvas();
		CreatePopUpCanvase();
	}

	void UIManager::Render(float tick)
	{
		GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugBeginEvent("Canvas");
		mainCanvas->Render(tick);
		GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugEndEvent();
	}

	void UIManager::Update()
	{
		auto mousePos = inputInstance->GetMousePos();

		if (mainCanvas->GetSelectMode() == true)
			mainCanvas->CheckPickedUI(mousePos.x, mousePos.y, inputInstance->GetInputState(VK_LBUTTON, KeyState::UP));
		else
			mainCanvas->CheckCollidedButton(mousePos.x, mousePos.y, inputInstance->GetInputState(VK_LBUTTON, KeyState::UP));
	}

	void UIManager::LateUpdate()
	{
	}

	void UIManager::Release()
	{
		mainCanvas->Release();
	}

	std::shared_ptr<Canvas> UIManager::CreateCanvas(const std::string& UIIDstr)
	{
		std::shared_ptr<Canvas> newCanvas = mainCanvas->CreateCanvasUI(UIIDstr, nowWidth, nowHeight);
		newCanvas->SetScale({ mainCanvas->GetWidth() / static_cast<float>(nowWidth), mainCanvas->GetHeight() / static_cast<float>(nowHeight) });

		return newCanvas;
	}

	std::shared_ptr<Canvas> UIManager::GetCanvas(const std::string& canvasIDStr)
	{
		return mainCanvas->GetCanvasUI(canvasIDStr);
	}

	void UIManager::OnResize(uint32 width, uint32 height)
	{
		mainCanvas->SetWidth(width);
		mainCanvas->SetHeight(height);

		// 비율 계산 후 모든 캔버스 Scale 변경
		float xScale = static_cast<float>(width) / nowWidth;
		float yScale = static_cast<float>(height) / nowHeight;

		mainCanvas->SetScaleAllCanvas({ xScale, yScale });
	}

	void UIManager::ClearUI()
	{
	}

	void UIManager::SelectGUIRender()
	{
		auto pickedUI = mainCanvas->GetSelectedUI();

		if (imgui->Begin("SelectedUI"))
		{
			if (pickedUI == nullptr)
				imgui->Text("None");
			else
			{
				// picked UI가 있는 경우.	
				imgui->Text(pickedUI->GetName());

				auto pickedPos = pickedUI->GetPosition();
				float UIPos[2] = { pickedPos.x, pickedPos.y };
				float UIPosZ = pickedPos.z;
				float UIWidth = pickedUI->GetWidth();
				float UIHeight = pickedUI->GetHeight();

				imgui->DragFloat2("UI Position", UIPos);
				imgui->DragFloat("UI Float Z", &UIPosZ, 0.05, 0.0f, 1.0f);
				imgui->DragFloat("UI Width", &UIWidth);
				imgui->DragFloat("UI Height", &UIHeight);
				
				pickedUI->SetPosition({ UIPos[0], UIPos[1], UIPosZ });
				pickedUI->SetWidth(UIWidth);
				pickedUI->SetHeight(UIHeight);
			}
		}
		imgui->End();
	}

	void UIManager::CreateTestCanvas()
	{
		auto canvas = mainCanvas->GetCanvasUI("TestCanvas");

		auto sampleButton = canvas->CreateButtonUI("sampleButton");
		sampleButton->SetPosition(Vector3{ 200.f, 100.f, 0.5f });

		auto texture = GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/DummyButton.png");
		sampleButton->SetDefaultTexture(texture);
		sampleButton->SetWidth(100.f);
		sampleButton->SetHeight(20.f);

		sampleButton->SetClickEvent([&]
			{
				mainCanvas->GetCanvasUI("PopUpCanvas")->SetEnable(true);
			});

		auto sampleButton2 = canvas->CreateButtonUI("sampleButton2");
		sampleButton2->SetPosition(Vector3{ 200.f, 130.f, 0.5f });
		sampleButton2->SetDefaultTexture(texture);
		sampleButton2->SetWidth(100.f);
		sampleButton2->SetHeight(20.f);

		sampleButton2->SetClickEvent([&]
			{
				mainCanvas->GetCanvasUI("PopUpCanvas")->SetEnable(false);
				mainCanvas->GetCanvasUI("TestCanvas")->GetButtonUI("sampleButton")->SetDefaultMaskColor({0.5f, 0.5f, 0.5f, 0.8f});
			});
	}

	void UIManager::CreatePopUpCanvase()
	{
		auto canvas = mainCanvas->GetCanvasUI("PopUpCanvas");

		auto sprite = canvas->CreateSpriteUI("PopupTest");

		auto texture = GraphicsManager::GetInstance()->LoadTexture(L"Resources/UI/DummyPopup.png");

		sprite->SetPosition(Vector3{ 0.f, 0.f, 0.2f });
		sprite->SetTexture(texture);
		sprite->SetWidth(400.f);
		sprite->SetHeight(600.f);

		auto text = canvas->CreateTextUI("PopupText");

		text->SetPosition(Vector3{ 0.f, -100.f, 0.1f });
		std::wstring test = L"Test용 string입니다.";
		text->SetText(test);
		text->SetWidth(100.f);
		text->SetHeight(40.f);

		auto button = canvas->CreateButtonUI("PopupButton");
		texture = ResourceManager::GetInstance()->LoadTexture(L"Resources/UI/DummyButton.png");
		button->SetDefaultTexture(texture);
		button->SetWidth(100.f);
		button->SetHeight(20.f);

		button->SetClickEvent([&]
		{
			mainCanvas->GetCanvasUI("PopUpCanvas")->GetSpriteUI("PopupTest")->SetMaskAlpha(0.5);
		});

		canvas->SetEnable(false);

	}
}
