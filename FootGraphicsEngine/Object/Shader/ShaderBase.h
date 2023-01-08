#pragma once

/// 각종 쉐이더들이 가지고 있어야하는 요소들을 wrap해둔 shaderBase 클래스
///	shader manager에서 Map이 가지고 있는 요소

#include <string>

namespace GraphicsEngineSpace
{
	// 쉐이더 종류 enum
	enum class ShaderType
	{
		NONE,
		VERTEX,
		PIXEL
	};

	// 쉐이더 구조체 => 밖으로 보여줄 것.
		// 포인터 주소를 전달하는 의미에 가까움
	struct Shader
	{
		int unused;
	};

	class ShaderBase
	{
	protected:
		ShaderType type;

	public:
		ShaderBase(ShaderType type)
			: type(type) {}
		virtual  ~ShaderBase() = default;

		// SetUp 및 Get
		// SetUp DC에 쉐이더를 설정해주는 단계
		virtual bool SetUpShader() abstract;
		virtual Shader* GetShader() abstract;
	};
}
