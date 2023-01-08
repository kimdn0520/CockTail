#include "GraphicsPch.h"
#include "ConstantBuffer.h"

namespace GraphicsEngineSpace
{
	ConstantBuffer::ConstantBuffer(ComPtr<ID3D11DeviceContext> deviceContext)
		: BufferBase(BufferType::CONSTANT)
		, deviceContext(deviceContext)
		, buffer(nullptr)
		, bufferSize(0)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
		
	}

	bool ConstantBuffer::SetUpBuffer(unsigned slot, void* data, ShaderType targetType)
	{
		if (buffer == nullptr)
			return false;

		// 각 쉐이더에 맞게 버퍼를 세팅해줍니다.
		// SetupBuffer 전에 버퍼가 만들어져 있어야합니다..!
			// GPU에 어떤 자원을 업데이트 할 때 필요한 함수.
		// 여기데이터에는 쉐이더를 얻어와서 보내주는 방식인가..
		// 쉐이더 포인터를 얻어와서 보이드 포인터로 바꿔서 이 함수로 던져주면 함수 내에서 암시적 캐스팅이 일어나는 식?
		deviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, data, bufferSize, 0);

		switch (targetType)
		{
			case ShaderType::VERTEX:
			{
				// 한 슬롯에 버퍼 하나씩..
					// 한 쉐이더에 constBuffer가 여러개이면 여러개의 버퍼를 만들어서 넣을 수 있다.
					// 그래서 중복되는 버퍼 관리하려고 이렇게 한건가
				deviceContext->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());

				return true;
			}

			case ShaderType::PIXEL:
			{
				deviceContext->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());

				return true;
			}
		}

		return false;
	}

	bool ConstantBuffer::CreateBuffer(ComPtr<ID3D11Device> device, UINT typeSize)
	{
		// 지훈이형 구조에서는 BufferBuilder와 Friend 키워드를 통해 거기서 사용해주는 듯하다.
		D3D11_BUFFER_DESC bufferDescription;
		// 버퍼 공간 만들어주기
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));

		// 버퍼에 정보 넣어주기
			// 사실상 UpdateSubresource를 사용할 것이기 때문에 여기서 정해주자.
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.ByteWidth = typeSize;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = 0;

		bufferSize = typeSize;

		HRESULT hr = device->CreateBuffer(&bufferDescription, nullptr, buffer.GetAddressOf());

		return SUCCEEDED(hr);
	}
}
