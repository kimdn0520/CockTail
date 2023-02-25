#pragma once

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief ImGUI를 관리하기 위한 매니저 클래스.
	 */
	 // 게임 엔진에서 사용 가능하도록 DLL Export를 해준다.
	class GRAPHICSENGINE_DECLSPEC ImGUIManager
	{
		static std::shared_ptr<ImGUIManager> instance;

		bool imGUIEnable;
		bool isFrame;

	public:
		ImGUIManager();
		~ImGUIManager();

		static std::shared_ptr<ImGUIManager> GetInstance();

		void Init();
		void Finalize();

		// Window 관련
		void InitImplWin(HWND hwnd);
		LRESULT SetWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void ShutDownImplWin();

		// DX11 관련
		void InitImplDX11(ID3D11Device* device, ID3D11DeviceContext* dc);
		void ShutDownImplDX11();

		// 렌더 관련..
		void Frame();
		void Render();
		void EndFrame();
		//void UpdateCamFrame();
		void OnOffImGui() { imGUIEnable = !imGUIEnable; }
		bool isImguiEnable() { return imGUIEnable; }

		// 실제 imGui를 생성할 함수.
			// imGUI의 함수를 래핑함
		bool Begin(std::string guiTitle);
		bool UISelectBegin();
		bool CollapsingHeader(std::string label);
		bool TreeNode(std::string label);
		bool TreeNode(std::string parentLabel, std::string label);

		// 화면에 무언가를 그리고싶을때 쓰자
		// gui를 fullscreen + nomove + layer back + 투명 시켜줌.
		bool DrawBegin(std::string guiTitle);

		bool SliderFloat(std::string label, float* value, float minValue, float maxValue);
		bool SliderFloat2(std::string label, float value[2], float minValue, float maxValue);
		bool SliderFloat3(std::string label, float value[3], float minValue, float maxValue);
		bool SliderFloat4(std::string label, float value[4], float minValue, float maxValue);

		bool DragFloat(std::string label, float* value, float speed = 1.0f, float min = 0.f, float max = 0.f, const char* format = "%.3f");
		bool DragFloat2(std::string label, float value[2], float speed = 1.0f, float min = 0.f, float max = 0.f, const char* format = "%.3f");
		bool DragFloat3(std::string label, float value[3], float speed = 1.0f, float min = 0.f, float max = 0.f, const char* format = "%.3f");
		bool DragFloat4(std::string label, float value[4], float speed = 1.0f, float min = 0.f, float max = 0.f, const char* format = "%.3f");

		bool InputFloat(std::string label, float* value, float step = 0.0f, float stepfast = 0.f, const char* format = "%.3f");
		bool InputFloat2(std::string label, float value[2], const char* format = "%.3f");
		bool InputFloat3(std::string label, float value[3], const char* format = "%.3f");
		bool InputFloat4(std::string label, float value[4], const char* format = "%.3f");

		bool DragInt(std::string label, int* value, float speed = 1.0f, int min = 0, int max = 0, const char* format = "%d");
		bool DragInt2(std::string label, int value[2], float speed = 1.0f, int min = 0, int max = 0, const char* format = "%d");
		bool DragInt3(std::string label, int value[3], float speed = 1.0f, int min = 0, int max = 0, const char* format = "%d");
		bool DragInt4(std::string label, int value[4], float speed = 1.0f, int min = 0, int max = 0, const char* format = "%d");

		bool InputInt(std::string label, int* value, int step = 1, int stepfast = 100);
		bool InputInt2(std::string label, int value[2]);
		bool InputInt3(std::string label, int value[3]);
		bool InputInt4(std::string label, int value[4]);

		bool ColorPicker4(std::string label, float col[4]);

		bool CheckBox(std::string label, bool* check);

		// 포맷으로 써도 괜찮지만.. string을 이용해서 +로 사용해도 괜찮을 것 같다.
		void Text(std::string text);
		// Printf()와 비슷한 포맷 버전
		void Text(const char* text, ...);

		void DrawLine(SimpleMath::Vector3 pos1, SimpleMath::Vector3 pos2);

		void DrawSpot(SimpleMath::Vector3 pos);

		void DrawTriangle(SimpleMath::Vector3 pos1, SimpleMath::Vector3 pos2, SimpleMath::Vector3 pos3);

		void Spacing();

		void End();

		// Tree Node 끝내기..
		void TreePop();
		void Separator();
	};
}