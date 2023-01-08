#include "GraphicsPch.h"
#include "ImGUIManager.h"

// IMGUI 관련 헤더
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ImGUIManager> ImGUIManager::instance = nullptr;

	ImGUIManager::ImGUIManager()
		: imGUIEnable(true)
		, isFrame(false)
	{

	}

	ImGUIManager::~ImGUIManager()
	{
	}

	std::shared_ptr<ImGUIManager> ImGUIManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ImGUIManager>();

		return instance;
	}

	void ImGUIManager::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}

	void ImGUIManager::Finalize()
	{
		ImGui::DestroyContext();
	}

	void ImGUIManager::InitImplWin(HWND hwnd)
	{
		ImGui_ImplWin32_Init(hwnd);
	}

	LRESULT ImGUIManager::SetWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
	}

	void ImGUIManager::ShutDownImplWin()
	{
		ImGui_ImplWin32_Shutdown();
	}

	void ImGUIManager::InitImplDX11(ID3D11Device* device, ID3D11DeviceContext* dc)
	{
		ImGui_ImplDX11_Init(device, dc);
	}

	void ImGUIManager::ShutDownImplDX11()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void ImGUIManager::Frame()
	{

		if (imGUIEnable == true)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}
	}

	void ImGUIManager::Render()
	{
		if (imGUIEnable == true)
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void ImGUIManager::EndFrame()
	{
		if (imGUIEnable == true)
		{
			ImGui::EndFrame();
		}
	}

	bool ImGUIManager::Begin(std::string guiTitle)
	{
		//Frame();
		return ImGui::Begin(guiTitle.c_str());
	}

	bool ImGUIManager::SliderFloat(std::string label, float* value, float minValue, float maxValue)
	{
		return ImGui::SliderFloat(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat2(std::string label, float value[2], float minValue, float maxValue)
	{
		return ImGui::SliderFloat2(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat3(std::string label, float value[3], float minValue, float maxValue)
	{
		return ImGui::SliderFloat3(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::SliderFloat4(std::string label, float value[4], float minValue, float maxValue)
	{
		return ImGui::SliderFloat4(label.c_str(), value, minValue, maxValue);
	}

	bool ImGUIManager::DragFloat(std::string label, float* value, float speed, float min, float max, const char* format)
	{
		return ImGui::DragFloat(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragFloat2(std::string label, float value[2], float speed, float min, float max,
		const char* format)
	{
		return ImGui::DragFloat2(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragFloat3(std::string label, float value[3], float speed, float min, float max,
		const char* format)
	{
		return ImGui::DragFloat3(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragFloat4(std::string label, float value[4], float speed, float min, float max,
		const char* format)
	{
		return ImGui::DragFloat4(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::InputFloat(std::string label, float* value, float step, float stepfast, const char* format)
	{
		return ImGui::InputFloat(label.c_str(), value, step, stepfast, format);
	}

	bool ImGUIManager::InputFloat2(std::string label, float value[2], const char* format)
	{
		return ImGui::InputFloat2(label.c_str(), value, format);
	}

	bool ImGUIManager::InputFloat3(std::string label, float value[3], const char* format)
	{
		return ImGui::InputFloat3(label.c_str(), value, format);
	}

	bool ImGUIManager::InputFloat4(std::string label, float value[4], const char* format)
	{
		return ImGui::InputFloat4(label.c_str(), value, format);
	}

	bool ImGUIManager::DragInt(std::string label, int* value, float speed, int min, int max, const char* format)
	{
		return ImGui::DragInt(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragInt2(std::string label, int value[2], float speed, int min, int max, const char* format)
	{
		return ImGui::DragInt2(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragInt3(std::string label, int value[3], float speed, int min, int max, const char* format)
	{
		return ImGui::DragInt3(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::DragInt4(std::string label, int value[4], float speed, int min, int max, const char* format)
	{
		return ImGui::DragInt4(label.c_str(), value, speed, min, max, format);
	}

	bool ImGUIManager::InputInt(std::string label, int* value, int step, int stepfast)
	{
		return ImGui::InputInt(label.c_str(), value, step, stepfast);
	}

	bool ImGUIManager::InputInt2(std::string label, int value[2])
	{
		return ImGui::InputInt2(label.c_str(), value);
	}

	bool ImGUIManager::InputInt3(std::string label, int value[3])
	{
		return ImGui::InputInt3(label.c_str(), value);
	}

	bool ImGUIManager::InputInt4(std::string label, int value[4])
	{
		return ImGui::InputInt4(label.c_str(), value);
	}

	void GraphicsEngineSpace::ImGUIManager::Text(std::string text)
	{
		ImGui::Text(text.c_str());
	}

	void ImGUIManager::Text(const char* text, ...)
	{
		// 사실상 Imgui::Text의 내부 구현을 따라한 수준
		va_list args;
		va_start(args, text);
		ImGui::TextV(text, args);
		va_end(args);
	}

	void ImGUIManager::End()
	{
		ImGui::End();
	}
}
