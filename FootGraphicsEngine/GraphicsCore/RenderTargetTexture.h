#pragma once

namespace GraphicsEngineSpace
{
	class DX11GraphicsCore;

	/**
	 * \brief Deferred Rendering, 각종 디버그 정보를 확인하기 위한 렌더 타겟 텍스쳐 클래스이다.
	 *
	 * 2D Texture를 하나 생성해서 렌더 타겟화 시키고, 해당 텍스쳐를 Shader Resource View로 생성해서
	 * 화면에 텍스쳐로서 그려질 수 있도록 하는 목적을 가진다.
	 *
	 * 2022.10.24 Dev.BlackFoot
	 */
	class RenderTargetTexture
	{
		// 필요한 멤버 변수
	private:
		ComPtr<ID3D11Texture2D> renderTexture;
		ComPtr<ID3D11RenderTargetView> renderTarget;
		ComPtr<ID3D11ShaderResourceView> shaderResource;

	public:
		RenderTargetTexture();
		~RenderTargetTexture();

		// init. 텍스쳐의 크기를 받아서 생성해준다.
		bool Init(ComPtr<ID3D11Device> device, int width, int height, UINT MSAAQuality);
		
		// COM 변수들 해제 (리사이즈 할 때 불러준다)
		void Finalize();

		// 렌더 타겟 뷰를 던져 줍니다.
			// GraphicsCore의 ResetView에 던져주기 위해서 필요합니다.
		ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return renderTarget; }

		// 셰이더 리소스 뷰를 던져줍니다.
			// 이를 셰이더에 보냄으로써 다른 셰이더 들이 해당 텍스쳐 데이터에 접근 가능해집니다.
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResource; }

		friend DX11GraphicsCore;
	};

}