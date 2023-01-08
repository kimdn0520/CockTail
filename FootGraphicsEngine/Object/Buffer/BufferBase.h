#pragma once

#include "Object/Shader/ShaderBase.h"

namespace GraphicsEngineSpace
{
	// 버텍스와 인덱스 버퍼도 여기서 관리할 수는 있다..만 일단은 constant만..
	enum class BufferType
	{
		NONE,
		CONSTANT
	};

	class BufferBase
	{
		BufferType type;

	public:
		BufferBase(BufferType type)
			: type()
		{}

		virtual ~BufferBase() = default;

		virtual bool SetUpBuffer(unsigned int slot, void* data, ShaderType targetType) abstract;
	};

}