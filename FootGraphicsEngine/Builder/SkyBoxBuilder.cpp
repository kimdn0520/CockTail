#include "GraphicsPch.h"
#include "SkyBoxBuilder.h"

#include "Interface/IDXObject.h"
#include "Object/SkyBox.h"
#include "GraphicsCore/RasterizerState.h"
#include "Manager/ResourceManager.h"
#include "Resources/RenderingMaterial.h"

using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> SkyBoxBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// 들어온 오브젝트 확인
		if(std::dynamic_pointer_cast<SkyBox>(pDXObj) == nullptr)
			return nullptr;

		pDXObj->SetMeshResources(BuildGeometry(pDXObj, objectName));

		return pDXObj;
	}

	std::shared_ptr<MeshResources> SkyBoxBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// 뭐가 되었건.. 스카이 박스로 만들어줍니다.
		_tmpObjRes = BuildSkyBoxResources(_tmpObjRes);

		return _tmpObjRes;
	}

	void SkyBoxBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 바인딩
		resourceManager = ResourceManager::GetInstance();
	}

	std::shared_ptr<MeshResources> SkyBoxBuilder::BuildSkyBoxResources(std::shared_ptr<MeshResources> _objRes)
	{
		_objRes->mesh = resourceManager->LoadMesh("SkyBoxMesh");

		_objRes->setMesh = true;

		// 텍스쳐 어떻게 넣음..?
		std::shared_ptr<RenderingMaterial> material = std::make_shared<RenderingMaterial>();
		material->SetName("SkyBoxMaterial");

		material->cubeTex = resourceManager->LoadTexture(L"Resources/Texture/snowcube1024.dds");

		_objRes->material.emplace_back(material);

		_objRes->setMaterial = true;

		return _objRes;
	}
}
