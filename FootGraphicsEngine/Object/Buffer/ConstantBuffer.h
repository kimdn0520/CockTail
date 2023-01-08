#pragma once

#include "BufferBase.h"

namespace GraphicsEngineSpace
{
	using GraphicsEngineSpace::BufferBase;

	class BufferManager;

	class ConstantBuffer : public BufferBase
	{
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11Buffer> buffer;
		UINT bufferSize;

	public:
		ConstantBuffer(ComPtr<ID3D11DeviceContext> deviceContext);
		~ConstantBuffer() override;

		bool SetUpBuffer(unsigned slot, void* data, ShaderType targetType) override;

	private:
		bool CreateBuffer(ComPtr<ID3D11Device> device, UINT typeSize);

		friend BufferManager;
	};

}