#pragma once

namespace GraphicsEngineSpace
{
	/**
	 * \brief 화면에 그림을 그리기 위한 RenderTargetView, DepthStencilView 등을 모아둔 클래스.
	 *
	 * 이후에 멀티 렌더 타겟 등을 하면서 기능이 추가 되겠지만,
	 * 현재(2022.07.01)는 RenderTargetView와 DepthStencilView 등을 잃어버리지 않게 잘 보관하는 역할만 한다.
	 *
	 * (2022.10.24 추가)
	 * 해당 클래스는 화면에 주요하게 표시 되는 렌더 타겟을 의미하며, 다른 렌더타겟과 다르게 유일하게
	 * Depth StencilView를 가진다.
	 *
	 * (2023. 01. 12 구조 변경)
	 * 해당 클래스는 shadow Map 등 새로운 깊이 연산이 필요할 때 사용할 depthStencil View만을 가진다.
	 * 쉐이더 리소스로 추가할 SRV도 추가로 가지게 한다.
	 *
	 * 2022.07.01 Dev.BlackFoot
	 */
	class DepthStencilResource
	{
	private:
		// 깊이 스텐실 버퍼(Resource로 사용할 Texture)
		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		// 깊이 스텐실 뷰
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		std::vector<ComPtr<ID3D11DepthStencilView>> depthStencilArray;

		// 깊이 스텐실 스테이트
		ComPtr<ID3D11DepthStencilState> depthStencilState;

		// shader에 넣어줄 리소스
		ComPtr<ID3D11ShaderResourceView> shaderResource;

		// Screen ViewPort
		// 스크린에서 사용하는 뷰포트의 DESC를 정해둔 screenViewport
			// CubeMap을 그리기 위한 Viewport를 따로 설정해줘야하기 때문에..
		std::shared_ptr<D3D11_VIEWPORT> screenViewport;


	public:
		DepthStencilResource();
		~DepthStencilResource();

		void Finalize();

		// 각종 게터..
			// 외부에서 수정 가능해야 하기 때문에.. const로 보내지 않는다..
			// 이게 안되면 고쳐야 함

			// 만약 안되면.. 포인터를 레퍼런스로 보내보자..
			// 아니면 디바이스를 받아서 여기서 설정하는 것으로 해보자..(이게 더 나을 수도 있다.)
			// r-value로 취급 된다..
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return depthStencilView; }
		ComPtr<ID3D11Texture2D> GetDepthStencilBuffer() { return depthStencilBuffer; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return depthStencilState; }
		std::vector<ComPtr<ID3D11DepthStencilView>> GetDepthStencilArray() { return depthStencilArray; }
		ComPtr<ID3D11ShaderResourceView> GetShaderResource() { return shaderResource; }
		std::shared_ptr<D3D11_VIEWPORT> GetViewport() { return screenViewport; }

		// 디바이스를 받아서 렌더 타겟을 새로 만들어 주는 과정 (사이즈에 맞게..)
			// 사이즈도 받아와주자.
		void CreateDepthStencil(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight);

		// Point Light를 위해 Cube Map Resource를 만들어줍니다..
		void CreateDepthStencilCube(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight, int cubeMapCnt);

		// Spot Light를 위한 Texture2D Array Resource 시도
		void CreateDepthStencilArray(
			ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC,
			int _clientWidth, int _clientHeight, int arrayCnt);


		// OnResize 처음의 각종 타겟들을 지워주는 함수.
		void DeleteImmediateDepthStencil();
	};
}
