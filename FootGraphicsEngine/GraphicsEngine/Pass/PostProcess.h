#pragma once

#include "PassBase.h"

namespace GraphicsEngineSpace
{
	class VertexShader;
	class PixelShader;
	class RenderTargetTexture;
	class DepthStencilResource;

	// 렌더 순서의 경우
		// 그림자 렌더(DSV) => LightPass의 RenderTarget에 Render
		// => 해당 타겟의 Texture를 받아서 PostProcessing => 이 결과를 Back버퍼에 그림.

	/**
	 * \brief PostProcessing을 위한 패스입니다.
	 *
	 * Deferred Render 이후 모든 렌더가 끝난 Render Target을 받아와 픽셀별로 처리해주는 Pass 입니다.
	 */
	class PostProcess : public PassBase
	{
	private:
		// 결과를 출력할 RTV(main)
		std::shared_ptr<RenderTargetTexture> mainRTV;
		// 그릴 때 사용할 DSV (사실상 main)
		std::shared_ptr<DepthStencilResource> mainDSV;

		// 각종 셰이더
		std::shared_ptr<VertexShader> quadVS;
		std::shared_ptr<PixelShader> postProcessPS;

		// Reset을 위한 그래픽스 코어
		std::shared_ptr<DX11GraphicsCore> graphicsCore;

		// 현재는 ToneMapping 하나지만 나중에 포스트 프로세스 종류가 늘어난다면
		// 추가적으로 트리거용 bool 변수나 PixelShader 변수를 추가해야합니다.

	public:
		virtual void Start(const std::shared_ptr<RenderTargetTexture>& mainRTV, const std::shared_ptr<DepthStencilResource>& mainDSV) override;
		virtual void Release() override;

		virtual void OnResize(int width, int height) override;

		void RenderStart();

		void Render(const std::shared_ptr<RenderTargetTexture>& renderTarget);

		void EndRender();

		void ImGuiRender();
	};

}
