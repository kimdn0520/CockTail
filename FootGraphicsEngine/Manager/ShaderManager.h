#pragma once
#include <map>
#include <string>

namespace GraphicsEngineSpace
{
	class ShaderBase;

	// 구 Effect 클래스 처럼 모든 쉐이더를 컴파일 한 뒤, 가지고 있는 쉐이더 매니저.
		// 사용하려면 쉐이더를 래핑한 클래스가 있어야할 것 같다.
	class ShaderManager
	{
	private:
		static std::shared_ptr<ShaderManager> instance;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3DBlob> shaderBlob;

		// 쉐이더 파일을 저장해둘 맵.
		std::map<std::string, std::shared_ptr<ShaderBase>> shaderMap;

	public:
		ShaderManager()
			: device(nullptr)
			, deviceContext(nullptr)
			, shaderBlob(nullptr)
		{}
		~ShaderManager();

		// 인스턴스 얻기
		static std::shared_ptr<ShaderManager> GetInstance();

		// 내부 초기화
		void Init(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext);

		// 모든 쉐이더 컴파일
		bool CompileAllShader();

		// 원하는 쉐이더 얻기
		std::shared_ptr<ShaderBase> GetShader(const std::string& shaderName);

		// 매니저 삭제
		void Finalize();

	private:
		// 쉐이더 컴파일 코드
		bool CompileShaderFromFile(const TCHAR* path, const char* entryName, const char* shaderModel, ID3DBlob** blobOut, const D3D_SHADER_MACRO* macro);

		bool CreateVertexShader(const std::string& path, const std::string& entryName, const std::string& shaderModel, const std::string& shaderName, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize,
			const D3D_SHADER_MACRO* macro);
		bool CreatePixelShader(const std::string& path, const std::string& entryName, const std::string& shaderModel, const std::string& shaderName,
			const D3D_SHADER_MACRO* macro);
		bool CreateGeometryShader(const std::string& path, const std::string& entryName, const std::string& shaderModel, const std::string& shaderName,
			const D3D_SHADER_MACRO* macro);
	};

}