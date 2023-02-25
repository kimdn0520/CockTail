#include "GamePch.h"
#include "UIManager.h"

#include "GraphicsManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include "Timer.h"
#include "UICreator.h"

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
		mainCanvas->CreateCanvasUI("TitleCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("CombinationCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("DialogCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("SettingCanvase", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("InGameCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("InGameMenuCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });
		mainCanvas->CreateCanvasUI("SceneTestCanvas", width / 2, height / 2)->SetPosition({ static_cast<float>(width / 2), static_cast<float>(height / 2), 0.1f });

		// ImGUI포인터를 미리 보관해둔다.
		imgui = ImGUIManager::GetInstance();

		creator = std::make_shared<UICreator>();

		printf("UIManager Init Clear\n");
	}

	// WIC 텍스쳐 로드 가능한 시점에 호출
	void UIManager::CreateCanvasAll()
	{
		creator->CreateCanvasAll();
	}

	void UIManager::Render(float tick)
	{
		GraphicsManager::GetInstance()->GetRenderer()->GraphicsDebugBeginEvent("Canvas");
		mainCanvas->Render(GraphicsManager::GetInstance()->GetRenderer(), tick);
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

		if (imgui->UISelectBegin())
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
				imgui->DragFloat("UI Float Z", &UIPosZ, 0.05f, 0.0f, 1.0f);
				imgui->DragFloat("UI Width", &UIWidth);
				imgui->DragFloat("UI Height", &UIHeight);

				pickedUI->SetPosition({ UIPos[0], UIPos[1], UIPosZ });
				pickedUI->SetWidth(UIWidth);
				pickedUI->SetHeight(UIHeight);

				if (std::dynamic_pointer_cast<ProgressBarUI>(pickedUI) != nullptr)
				{
					float fill = std::dynamic_pointer_cast<ProgressBarUI>(pickedUI)->GetFillPercent();

					imgui->DragFloat("Fill Percent", &fill, 0.01f, 0.0f, 1.0f);

					std::dynamic_pointer_cast<ProgressBarUI>(pickedUI)->SetFillPercent(fill);
				}

				if (std::dynamic_pointer_cast<TextUI>(pickedUI))
				{
					float fontSize = std::dynamic_pointer_cast<TextUI>(pickedUI)->GetFontSize();

					imgui->DragFloat("Font Size", &fontSize);

					std::dynamic_pointer_cast<TextUI>(pickedUI)->SetFontSize(fontSize);
				}

				if (std::dynamic_pointer_cast<SpriteUI>(pickedUI))
				{
					auto maskColor = std::dynamic_pointer_cast<SpriteUI>(pickedUI)->GetMaskColor();
					float color[4] = { maskColor.x, maskColor.y, maskColor.z, maskColor.w };

					imgui->ColorPicker4("Sprite Mask Color", color);

					std::dynamic_pointer_cast<SpriteUI>(pickedUI)->SetMaskColor(color[0], color[1], color[2], color[3]);
				}
			}
		}
		imgui->End();
	}

}
