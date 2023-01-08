#include "GraphicsPch.h"
#include "SpriteEffectBuilder.h"

#include "Interface/IDXObject.h"
#include "Manager/ResourceManager.h"
#include "Object/SpriteEffect.h"
#include "Resources/RenderingMaterial.h"

using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> SpriteEffectBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		if(std::dynamic_pointer_cast<SpriteEffect>(pDXObj) == nullptr)
			return nullptr;

		BuildGeometry(pDXObj, objectName);

		return pDXObj;
	}

	std::shared_ptr<MeshResources> SpriteEffectBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject,
		std::string objectPath)
	{
		std::shared_ptr<MeshResources> tmpRes = std::make_shared<MeshResources>();

		tmpRes->ObjName = objectPath;

		// 오브젝트 이름을 판별합니다.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".model")
		{
			// 모델을 불러오자 일단 path.. 임시
			std::ifstream ifs("BinaryFile/Model/" + objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			tmpRes.reset();

			BuildBinaryEffect(DXObject, modelData);
		}
		else
		{
			BuildDefaultEffect(DXObject, tmpRes);
			return tmpRes;
		}
	}

	void SpriteEffectBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		resourceManager = ResourceManager::GetInstance();
	}

	void SpriteEffectBuilder::BuildDefaultEffect(std::shared_ptr<IDXObject> DXObject, std::shared_ptr<MeshResources> tmpResource)
	{
		// 일단은 하드코딩으로 테스트용 obj를 하나 만들어봅니다.
			// TODO 이 부분은 이후에 Effect Mesh를 생성하면 변경해주어야 합니다.
		// Plane Effect 생성
		std::vector<VertexStruct::TexVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = Vector3(-3.f, 0.f, -1.f);
		vertices[0].Tex = Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = Vector3(-3.f, 0.f, 1.f);
		vertices[1].Tex = Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = Vector3(3.f, 0.f, 1.f);
		vertices[2].Tex = Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = Vector3(-3.f, 0.f, -1.f);
		vertices[3].Tex = Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = Vector3(3.f, 0.f, 1.f);
		vertices[4].Tex = Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = Vector3(3.f, 0.f, -1.f);
		vertices[5].Tex = Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		auto mesh = resourceManager->CreateMesh(
			vertices,
			indices,
			"EffectMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		tmpResource->mesh = mesh;
		tmpResource->setMesh = true;

		// 텍스쳐 하나 생성.
		auto mat = std::make_shared<RenderingMaterial>();

		auto albedoTex = resourceManager->LoadTexture(L"Resources/Effect/EffectTest.png");
		mat->albedoTex = albedoTex;

		tmpResource->material.emplace_back(mat);

		tmpResource->setMaterial = true;
	}

	void SpriteEffectBuilder::BuildBinaryEffect(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->isSkinnedMesh = true;

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}
	}
}
