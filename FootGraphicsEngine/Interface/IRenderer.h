#pragma once
/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;
	class Canvas;

	/**
	* \brief Renderer의 기능만 정리해둔 인터페이스 클래스
	*
	* 2022.06.14(화) Dev.BlackFoot
	*/
	class GRAPHICSENGINE_DECLSPEC IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() {}

		// 반드시 구현해줘야하는 목록
		// 초기화 => Device, DC, SwapChain 생성
		// 초기화 할 때 받아서 Render에게 넘겨준다.
		virtual bool Initialize(HWND _hWnd, int _clientWidth, int _clientHeight) abstract;
		// WIC Factory 이슈로 인한 Late Init
		virtual bool LateInitialize() abstract;

		// 렌더러 자체의 초기화가 끝난 후, 오브젝트 들을 초기화 해주는 함수 추가
		virtual void InitObject(std::shared_ptr<IDXObject> obj) abstract;

		// 엔진 종료
		virtual void Finalize() abstract;
		// 화면 창이 바뀌었을 때 해주는 onResize(렌더 타겟 생성)
		virtual void OnResize() abstract;

		// 그리기 함수
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void DebugRender() abstract;
		virtual void ShadowRenderStart() abstract;
		virtual void ShadowRenderStart(int idx, bool isPointLight) abstract;
		virtual void ShadowRender(std::shared_ptr<IDXObject> obj, bool isPointLight) abstract;
		virtual void ShadowRenderEnd() abstract;
		virtual void EndRender() abstract;

		// 창 사이즈와 관련된 함수 목록 => 외부에서 사용 가능해야하기 때문에 인터페이스에 추가
		virtual bool IsVaildDevice() abstract;
		virtual void SetClientSize(int _width, int _height) abstract;
		virtual void SetWinMinMax(bool _isMinimized, bool _isMaximized) abstract;

		// 화면비 얻어오기
		virtual float GetAspectRatio() const abstract;

		// UI 관련 생성함수
		virtual std::shared_ptr<Canvas> CreateCanvas(const std::string& name, float width, float height) abstract;

		// 그래픽스 디버그 이벤트
		virtual void GraphicsDebugBeginEvent(const std::string& name) abstract;
		virtual void GraphicsDebugEndEvent() abstract;

		// 카메라 위치 설정
		virtual void SetCameraPos(float posX, float posY, float posZ) abstract;

		// 빛 던지기
		virtual void PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj) abstract;
		virtual void PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, bool isShadow, std::vector<SimpleMath::Matrix> lightViewProj) abstract;
		virtual void PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, SimpleMath::Vector3 direction, float power, float innerSpotAngle, float outerSpotAngle, float range, bool
		                           isShadow, SimpleMath::Matrix lightViewProj) abstract;
		virtual void PassAmbientSkyColor(SimpleMath::Vector4 color) abstract;

		// Scene이 바뀔 때 초기화 해주는 부분(특히 shadow Map을 초기화)
		virtual void ResetShadowPass() abstract;
	};

}