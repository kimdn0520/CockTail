#pragma once
#include <map>
#include <string>

namespace GraphicsEngineSpace
{
	class BufferBase;

	// 쉐이더에 사용되는 Buffer들을 관리하는 매니저 클래스
		// 현재는 사실상 cb만 가지고 있다.
		// 값 세팅은 각 Obj 클래스에서 해준다.
	class BufferManager
	{
	private:
		static std::shared_ptr<BufferManager> instance;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		// 각종 버퍼를 저장해둘 맵
			// 지훈이 형 같은 경우에는 생성을 빌더에서 하고 (빌더는 생성만 하고)
			// 리소스 자체의 관리는 리소스 매니저를 통해 관리해주었다.
		std::map<std::string, std::shared_ptr<BufferBase>> bufferMap;

	public:
		BufferManager()
			: device(nullptr)
			, deviceContext(nullptr)
		{}
		~BufferManager();

		// 인스턴스
		static std::shared_ptr<BufferManager> GetInstance();

		void Init(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext);

		// 사용할 버퍼 만들기
		bool CreateConstantBuffer(std::string cbName, UINT typeSize);

		void Finalize();

		// 필요한 버퍼 불러오기
		std::shared_ptr<BufferBase> GetBuffer(std::string bufferName);
	};
}