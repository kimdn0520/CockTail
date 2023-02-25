#pragma once
#include "PassBase.h"
#include "Util/LightHelper.h"

namespace GraphicsEngineSpace
{
	// 패스에 필요한 각종 클래스 전방선언
	class VertexShader;
	class GeometryShader;
	class RenderTargetTexture;
	class DepthStencilResource;
	class IDXObject;

	// Shadow Map Texture를 만들기 위한 패스.
		// 실제로는 RenderTarget을 변경해주는 역할
		// 지니고 있는 DepthStencilShader에 Shader Resource Texture를 만들어주는 역할을 한다.
	class ShadowPass : public PassBase
	{
		// 렌더타겟은 따로 설정하지 않는다. => 원상 복귀 시켜줄 mainDSV
	private:
		std::shared_ptr<DepthStencilResource> mainDSV;
		// Directional Light에 사용할 DSV
		std::shared_ptr<DepthStencilResource> dirShadowDSV;
		// 큐브맵 형식으로 만들어줄 쉐도우 큐브맵.
		std::shared_ptr<DepthStencilResource> pointShadowDSV;
		// SpotLight에 사용할 DSV
		std::shared_ptr<DepthStencilResource> spotShadowDSV;

		std::shared_ptr<VertexShader> shadowStaticVS;
		std::shared_ptr<VertexShader> shadowSkinnedVS;
		std::shared_ptr<VertexShader> geoShadowStaticVS;
		std::shared_ptr<VertexShader> geoShadowSkinnedVS;
		std::shared_ptr<GeometryShader> shadowGS;

		// 픽셀쉐이더는 따로 지정하지 않는다..
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		SimpleMath::Matrix lightViewProj;

		__declspec(align(16)) struct cbLightMatrix
		{
			SimpleMath::Matrix lightViewProj[6];
		};

		cbLightMatrix lightMatrix;

		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};

		// cubeMap 세팅을 했는가에 대한 정보.
		bool isSetPointLightCnt = false;
		// spot Light 세팅 여부
		bool isSetSpotLightCnt = false;

	public:
		virtual void Start(const std::shared_ptr<RenderTargetTexture>& mainRTV, const std::shared_ptr<DepthStencilResource>& mainDSV) override;
		virtual void Release() override;

		virtual void OnResize(int width, int height) override;

		// ShadowCubeMap을 light 개수만큼 생성해줍니다.
		void SetPointLightArray(int pointLightCnt);
		// Spot Light 개수 만큼 쉐도우 맵을 생성합니다.
		void SetSpotLightArray(int spotLightCnt);

		// Shadow Render Target을 세팅해둔 상태로 -> 그림자를 드리울 모든 오브젝트를 그려줘야한다.
			// 현 시점에서 그림자를 드리울 오브젝트는 PBR Obj이며.. 따라서 해당 오브젝트 들을 따로 관리 해주거나 해야할 수도 있다.
			// PBR Obj 안에 Shadow Render 함수를 넣는 것도 방법일 수 있다..
		// RenderTarget 등을 세팅하는 힘수
		void RenderStart();
		// 사실상 PBR Obj 안에서 돌아야 할 것 같다..
			// 아니면 obj Queue를 받거나..
		void Render(std::shared_ptr<IDXObject> object);
		// 사용한 리소스 초기화
		void EndRender();

		// Point Light용 함수를 따로 빼보자
		void PointRenderStart(int idx);
		void PointRender(std::shared_ptr<IDXObject> object);

		// Spot Light 용 렌더 함수
		void SpotRenderStart(int idx);

		void SetLightViewProj(SimpleMath::Matrix lightViewProj);
		void SetLightMatrix(std::vector<SimpleMath::Matrix> lightMatrix);
		void SetLightMatrix(PBR::PointLight pointLight);

		std::shared_ptr<DepthStencilResource> GetShadowDSV() { return dirShadowDSV; }
		std::shared_ptr<DepthStencilResource> GetPointShadowDSV() { return pointShadowDSV; }
		std::shared_ptr<DepthStencilResource> GetSpotShadowDSV() { return spotShadowDSV; }


		bool GetIsSetPointLightCnt() { return isSetPointLightCnt; }
		void ResetIsSetPointLightCnt();
		
		bool GetIsSetSpotLightCnt() { return isSetSpotLightCnt; }
		void ResetIsSetSpotLightCnt();
	};
}

