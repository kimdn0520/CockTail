#pragma once

namespace GraphicsEngineSpace
{
	/// 소연 누나의 InputLayout클래스를 참고해서 제작한 inputlayout 클래스.
	/// 내부적으로 버텍스 구조체가 다른 오브젝트들의 inputlayout을 가지고 있으며,
	///	필요할 때 전달하는 역할을 한다. => Effect 클래스와 비슷한 느낌..
	///
	///	기본적으로 이 때까지 object에 붙어있던 BuildVertexLayout함수 부분을 클래스로 뽑아준것.
	///	2022.07.06(수) Dev.BlackFoot

	/**
	 * \brief 버텍스 구조체를 바탕으로 각종 인풋레이아웃 Desc를 만들어준다.
	 */
	class InputLayoutDesc
	{
	public:
		// 가장 기본적인 InputLayout 위치와 컬러 정보.
		static D3D11_INPUT_ELEMENT_DESC PosColor[2];

		// Effect 표현을 위한 위치와 UV정보
		static D3D11_INPUT_ELEMENT_DESC PosTex[2];
		
		// PBR
		static D3D11_INPUT_ELEMENT_DESC PBRStatic[5];

		static D3D11_INPUT_ELEMENT_DESC PBRSkinned[9];

		// 스카이 박스 DESC
		static D3D11_INPUT_ELEMENT_DESC Pos[1];
	};

}