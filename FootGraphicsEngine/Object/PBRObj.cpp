#include "GraphicsPch.h"
#include "PBRObj.h"

#include "Manager/BufferManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SamplerManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"
#include "Resources/Texture.h"
#include "Resources/RenderingMaterial.h"
#include "Util/Animator.h"
#include "Util/AnimatorTransition.h"
#include "Util/AnimatorState.h"

#include "GraphicsEngine/Renderer.h"


namespace GraphicsEngineSpace
{
	PBRObj::PBRObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, boneResources(nullptr)
		, objAnimator(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	PBRObj::~PBRObj()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes);
		}

		SafeReset(boneResources);

		/*if (animResources != nullptr)
		{
			for (auto animClip : animResources->animationClipList)
			{
				SafeReset(animClip.second);
			}
		}

		SafeReset(animResources);*/
	}

	bool PBRObj::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		// mesh, material 캐싱
		for (auto& objRes : meshResources)
		{
			objRes->mesh = resourceManager->LoadMesh(objRes->ObjName);

			objRes->nodeTM = objRes->mesh->nodeTM;

			// material이 없는 경우
			if (objRes->mesh->materials.empty())
			{
				std::shared_ptr<RenderingMaterial> material = std::make_shared<RenderingMaterial>();

				objRes->material.emplace_back(material);
			}
			// material이 있는 경우
			else
			{
				for (int matCnt = 0; matCnt < objRes->mesh->materials.size(); matCnt++)
				{
					std::shared_ptr<RenderingMaterial> material = std::make_shared<RenderingMaterial>();

					// 외부 머터리얼파일을 불러온다.
					std::ifstream ifs("BinaryFile/Material/" + objRes->mesh->materials[matCnt] + ".mat", std::ios_base::binary);
					boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
					buffer.push(boost::iostreams::zlib_decompressor());
					buffer.push(ifs);
					boost::archive::binary_iarchive ia(buffer);

					FBXBinaryData::MaterialData materialData;

					ia >> materialData;

					std::wstring convertTexName;
					if (materialData.albedoMap != "")
					{
						convertTexName.assign(materialData.albedoMap.begin(), materialData.albedoMap.end());
						material->albedoTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					if (materialData.normalMap != "")
					{
						convertTexName.assign(materialData.normalMap.begin(), materialData.normalMap.end());
						material->normalTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					if (materialData.metallicMap != "")
					{
						convertTexName.assign(materialData.metallicMap.begin(), materialData.metallicMap.end());
						material->metallicTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					if (materialData.roughnessMap != "")
					{
						convertTexName.assign(materialData.roughnessMap.begin(), materialData.roughnessMap.end());
						material->roughnessTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					if (materialData.AOMap != "")
					{
						convertTexName.assign(materialData.AOMap.begin(), materialData.AOMap.end());
						material->aoTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					if (materialData.emissiveMap != "")
					{
						convertTexName.assign(materialData.emissiveMap.begin(), materialData.emissiveMap.end());
						material->emissiveTex = resourceManager->LoadTexture(L"Resources/Texture/" + convertTexName);
					}

					material->material_Ambient.x = materialData.material_Ambient.x;
					material->material_Ambient.y = materialData.material_Ambient.y;
					material->material_Ambient.z = materialData.material_Ambient.z;
					material->material_Ambient.w = materialData.material_Ambient.w;

					material->material_Diffuse.x = materialData.material_Diffuse.x;
					material->material_Diffuse.y = materialData.material_Diffuse.y;
					material->material_Diffuse.z = materialData.material_Diffuse.z;
					material->material_Diffuse.w = materialData.material_Diffuse.w;

					material->material_Specular.x = materialData.material_Specular.x;
					material->material_Specular.y = materialData.material_Specular.y;
					material->material_Specular.z = materialData.material_Specular.z;
					material->material_Specular.w = materialData.material_Specular.w;

					material->material_Emissive.x = materialData.material_Emissive.x;
					material->material_Emissive.y = materialData.material_Emissive.y;
					material->material_Emissive.z = materialData.material_Emissive.z;
					material->material_Emissive.w = materialData.material_Emissive.w;

					material->material_Reflectivity = materialData.material_Reflectivity;
					material->material_Transparency = materialData.material_Transparency;

					material->metallic = materialData.metallic;
					material->roughness = materialData.roughness;

					objRes->material.emplace_back(material);
				}
			}
		}

		// 본 개수 만큼 생성.
		//if (boneResources != nullptr /* && objAnimator->animResources != nullptr*/)
		//{
		//	objAnimator = std::make_unique<Animator>(boneResources->bones.size());
		//	nowBoneTM.assign(boneResources->bones.size(), SimpleMath::Matrix::Identity);
		//}

		return true;
	}

	void PBRObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}

	void PBRObj::PreRender(float tick)
	{
		// 본트랜스폼을 바꿔준다.
	
		if (boneResources != nullptr)
		{
			std::vector<SimpleMath::Matrix> animVector;

			if (objAnimator != nullptr)
			{
				animVector = objAnimator->UpdateBoneTM(tick);
				boneResources->setNode = false;
			}

			// 해당 AnimVector를 업데이트.
			for (size_t i = 0; i < boneResources->bones.size(); i++)
			{
				Bone tmpBone = boneResources->bones[i];
				//Matrix modifiedAnimTM = animVector[i];

				// 부모 TM과의 관계로 새로운 AnimVector를 만들어준다.
				if (tmpBone.parentIdx != -1)
					animVector[i] *= animVector[tmpBone.parentIdx];

				/*while(tmpBone.parentIdx != -1)
				{
					modifiedAnimTM *= animVector[tmpBone.parentIdx];
					tmpBone = boneResources->bones[tmpBone.parentIdx];
				}

				tmpBone = boneResources->bones[i];*/

				if (boneResources->setNode != true)
					nowBoneTM[i] = tmpBone.offsetTransform * animVector[i];
				// nowBoneTM[i] = tmpBone.offsetTransform * modifiedAnimTM;
			}

			boneResources->setNode = true;

		}
		/*
		*/
	}

	void PBRObj::Render()
	{
		assert(D3DDeviceContext);

		for (auto& objRes : meshResources)
		{
			// Mesh의 종류에 따라 다른 Render 함수를 부른다.
			if (objRes->isSkinnedMesh != true)
				StaticRender(objRes);
			else
				SkinnedRender(objRes);
		}
	}

	void PBRObj::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	void PBRObj::SetBoneResource(std::shared_ptr<BoneResources> boneResources)
	{
		this->boneResources = boneResources;

		if (boneResources != nullptr)
		{
			objAnimator = std::make_unique<Animator>(boneResources->bones.size());
			nowBoneTM.assign(boneResources->bones.size(), SimpleMath::Matrix::Identity);
		}
	}

	void PBRObj::SetAnimationResource(std::shared_ptr<AnimationResources> animResources)
	{
		objAnimator->animResources = animResources;
	}

	std::string PBRObj::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> PBRObj::GetMeshResources()
	{
		return meshResources;
	}

	void PBRObj::PlayAnim(std::string animCilpName, bool isLoop)
	{
		if (objAnimator->animResources != nullptr)
		{
			auto findIt = objAnimator->animResources->animationClipList.find(animCilpName);

			if (findIt != objAnimator->animResources->animationClipList.end())
				objAnimator->SetAnimationClip(findIt->second, isLoop);
		}
	}

	void PBRObj::AddAnimationState(const std::string& stateName, const std::string& animClipName, float speed, bool loop)
	{
		if (objAnimator->animResources != nullptr)
		{
			std::shared_ptr<AnimatorState> animatorState = std::make_shared<AnimatorState>();

			animatorState->SetStateName(stateName);

			animatorState->SetMotion(animClipName);

			animatorState->SetSpeed(speed);

			animatorState->SetLoop(loop);

			objAnimator->SetAnimatorState(stateName, animatorState);
		}
	}

	void PBRObj::SettingAnimationStateTransition(const std::string& stateName, const std::string& stateStart, const std::string& stateEnd, bool hasExitTime, float exitTime, float transitionDuration)
	{
		if (objAnimator->animResources != nullptr)
		{
			// Animator에 있는 states 들에서 해당 state name이 있는지 찾는다.
			auto tmp = objAnimator->GetAnimatorStates();

			auto findIt = tmp.find(stateName);
			
			// 해당 stateName을 들고있다면
			if (findIt != tmp.end())
			{
				std::shared_ptr<AnimatorTransition> transition = std::make_shared<AnimatorTransition>();

				transition->SetTransitionState(stateStart, stateEnd);

				transition->SetAnimationStateStart(stateStart);

				transition->SetAnimationStateEnd(stateEnd);

				transition->SetHasExitTime(hasExitTime);

				transition->SetExitTime(exitTime);

				transition->SetTransitionDuration(transitionDuration);

				findIt->second->AddTransition(transition);
			}
		}
	}

	void PBRObj::EntryPlayAnimationState(const std::string& stateName)
	{
		objAnimator->EntryAnimatorState(stateName);
	}

	void PBRObj::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val)
	{
		objAnimator->AddCondition(transitionName, parameterName, conditionName, val);
	}

	void PBRObj::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val)
	{
		objAnimator->AddCondition(transitionName, parameterName, conditionName, val);
	}

	void PBRObj::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val)
	{
		objAnimator->AddCondition(transitionName, parameterName, conditionName, val);
	}

	void PBRObj::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName)
	{
		objAnimator->AddCondition(transitionName, parameterName, conditionName);
	}

	void PBRObj::AddFloat(const std::string& name, float val)
	{
		objAnimator->AddFloat(name, val);
	}

	void PBRObj::AddInteger(const std::string& name, int val)
	{
		objAnimator->AddInteger(name, val);
	}

	void PBRObj::AddBool(const std::string& name, bool val)
	{
		objAnimator->AddBool(name, val);
	}

	void PBRObj::AddTrigger(const std::string& name)
	{
		objAnimator->AddTrigger(name);
	}

	void PBRObj::SetFloat(const std::string& name, float val)
	{
		objAnimator->SetFloat(name, val);
	}

	void PBRObj::SetInteger(const std::string& name, int val)
	{
		objAnimator->SetInteger(name, val);
	}

	void PBRObj::SetBool(const std::string& name, bool val)
	{
		objAnimator->SetBool(name, val);
	}

	void PBRObj::SetTrigger(const std::string& name)
	{
		objAnimator->SetTrigger(name);
	}

	int PBRObj::GetAnimCurrentFrame()
	{
		return objAnimator->GetNowFrame();
	}

	int PBRObj::GetAnimCurrentTotalFrame()
	{
		return objAnimator->GetTotalFrame();
	}

	void PBRObj::StaticRender(const std::shared_ptr<MeshResources>& objRes)
	{
		auto mesh = objRes->mesh;
		auto material = objRes->material;

		for (int matCnt = 0; matCnt < material.size(); matCnt++)
		{
			if(mesh->indexBuffersSize.empty())
				break;
			
			auto& albedo = material[matCnt]->albedoTex;
			auto& normal = material[matCnt]->normalTex;
			auto& metallic = material[matCnt]->metallicTex;
			auto& roughness = material[matCnt]->roughnessTex;
			auto& ao = material[matCnt]->aoTex;
			auto& emissive = material[matCnt]->emissiveTex;

			// 쉐이더 세팅
			ShaderManager::GetInstance()->GetShader("PBRStaticVS")->SetUpShader();

			if (albedo == nullptr)
			{
				// 해당 조건을 만족하는 쉐이더는 하나 밖에 없다.
				ShaderManager::GetInstance()->GetShader("PBRPS")->SetUpShader();
			}
			else
			{
				// 나머지는 모두 알베도가 있음을 가정한다.
				if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr &&
					ao != nullptr &&
					emissive != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRFullPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr &&
					ao != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMRAOPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					ao != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalAOPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					metallic != nullptr &&
					roughness != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					normal != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalPS")->SetUpShader();
				}
				else
				{
					ShaderManager::GetInstance()->GetShader("PBRAlbedoPS")->SetUpShader();
				}
			}

			D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

			UINT stride = sizeof(VertexStruct::PBRStaticVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
			D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			// 상수 버퍼 세팅
			cbMatrix cbPerObj;
			Matrix newWorld = objRes->nodeTM * world;
			cbPerObj.worldViewProj = newWorld * view * proj;
			cbPerObj.worldTransform = newWorld;
			cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(newWorld);
			BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

			// PBR Material
			cbMaterial cbPBRMat;
			cbPBRMat.metallic = material[matCnt]->metallic;
			cbPBRMat.roughness = material[matCnt]->roughness;

			cbPBRMat.AddColor.x = material[matCnt]->material_Diffuse.x;
			cbPBRMat.AddColor.y = material[matCnt]->material_Diffuse.y;
			cbPBRMat.AddColor.z = material[matCnt]->material_Diffuse.z;

			cbPBRMat.emissiveColor.x = material[matCnt]->material_Emissive.x;
			cbPBRMat.emissiveColor.y = material[matCnt]->material_Emissive.y;
			cbPBRMat.emissiveColor.z = material[matCnt]->material_Emissive.z;

			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(1, &cbPBRMat, ShaderType::VERTEX);
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(0, &cbPBRMat, ShaderType::PIXEL);

			// 텍스쳐 세팅
			if (albedo != nullptr)
				D3DDeviceContext->PSSetShaderResources(0, 1, albedo->GetMapSRV().GetAddressOf());

			if (normal != nullptr)
				D3DDeviceContext->PSSetShaderResources(1, 1, normal->GetMapSRV().GetAddressOf());

			if (metallic != nullptr)
				D3DDeviceContext->PSSetShaderResources(2, 1, metallic->GetMapSRV().GetAddressOf());

			if (roughness != nullptr)
				D3DDeviceContext->PSSetShaderResources(3, 1, roughness->GetMapSRV().GetAddressOf());

			if (ao != nullptr)
				D3DDeviceContext->PSSetShaderResources(4, 1, ao->GetMapSRV().GetAddressOf());

			if (emissive != nullptr)
				D3DDeviceContext->PSSetShaderResources(5, 1, emissive->GetMapSRV().GetAddressOf());


			D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

			// 렌더 스테이트 설정.
			D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

			// 해당 내용 그리기
			D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[matCnt], 0, 0);

			// 셰이더 초기화
			ID3D11ShaderResourceView* nullSRV[6] = { nullptr };

			D3DDeviceContext->PSSetShaderResources(0, 6, nullSRV);
		}
	}

	void PBRObj::SkinnedRender(const std::shared_ptr<MeshResources>& objRes)
	{
		auto mesh = objRes->mesh;
		auto material = objRes->material;

		for (int matCnt = 0; matCnt < material.size(); matCnt++)
		{
			if (mesh->indexBuffersSize.empty())
				break;

			auto& albedo = material[matCnt]->albedoTex;
			auto& normal = material[matCnt]->normalTex;
			auto& metallic = material[matCnt]->metallicTex;
			auto& roughness = material[matCnt]->roughnessTex;
			auto& ao = material[matCnt]->aoTex;
			auto& emissive = material[matCnt]->emissiveTex;

			// 쉐이더 세팅
			ShaderManager::GetInstance()->GetShader("PBRSkinnedVS")->SetUpShader();

			if (albedo == nullptr)
			{
				// 해당 조건을 만족하는 쉐이더는 하나 밖에 없다.
				ShaderManager::GetInstance()->GetShader("PBRPS")->SetUpShader();
			}
			else
			{
				// 나머지는 모두 알베도가 있음을 가정한다.
				if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr &&
					ao != nullptr &&
					emissive != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRFullPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr &&
					ao != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMRAOPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					ao != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalAOPS")->SetUpShader();
				}
				else if (
					normal != nullptr &&
					metallic != nullptr &&
					roughness != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					metallic != nullptr &&
					roughness != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					normal != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalPS")->SetUpShader();
				}
				else
				{
					ShaderManager::GetInstance()->GetShader("PBRAlbedoPS")->SetUpShader();
				}
			}

			D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

			UINT stride = sizeof(VertexStruct::PBRSkinnedVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
			D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			// 상수 버퍼 세팅
			cbMatrix cbPerObj;
			cbPerObj.worldViewProj = world * view * proj;
			cbPerObj.worldTransform = world;
			cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(world);
			BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


			// PBR Material
			cbMaterial cbPBRMat;
			cbPBRMat.metallic = material[matCnt]->metallic;
			cbPBRMat.roughness = material[matCnt]->roughness;

			cbPBRMat.AddColor.x = material[matCnt]->material_Diffuse.x;
			cbPBRMat.AddColor.y = material[matCnt]->material_Diffuse.y;
			cbPBRMat.AddColor.z = material[matCnt]->material_Diffuse.z;

			cbPBRMat.emissiveColor.x = material[matCnt]->material_Emissive.x;
			cbPBRMat.emissiveColor.y = material[matCnt]->material_Emissive.y;
			cbPBRMat.emissiveColor.z = material[matCnt]->material_Emissive.z;
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(1, &cbPBRMat, ShaderType::VERTEX);
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(0, &cbPBRMat, ShaderType::PIXEL);

			// Bone Matrix
				// => boneResources의 boneTransform이 애니메이션에 의해 어디선가에서 변경된다고 가정.
				// PreRender부분이 될 수도 있고, 해당 object의 Update일수도 있음.
			cbBoneMatrix cbBone;
			

			for (int i = 0; i < nowBoneTM.size(); i++)
			{
				cbBone.boneMatrix[i] = nowBoneTM[i];
			}

			BufferManager::GetInstance()->GetBuffer("BoneMatrixCB")->SetUpBuffer(2, &cbBone, ShaderType::VERTEX);

			// 텍스쳐 세팅
			if (albedo != nullptr)
				D3DDeviceContext->PSSetShaderResources(0, 1, albedo->GetMapSRV().GetAddressOf());

			if (normal != nullptr)
				D3DDeviceContext->PSSetShaderResources(1, 1, normal->GetMapSRV().GetAddressOf());

			if (metallic != nullptr)
				D3DDeviceContext->PSSetShaderResources(2, 1, metallic->GetMapSRV().GetAddressOf());

			if (roughness != nullptr)
				D3DDeviceContext->PSSetShaderResources(3, 1, roughness->GetMapSRV().GetAddressOf());

			if (ao != nullptr)
				D3DDeviceContext->PSSetShaderResources(4, 1, ao->GetMapSRV().GetAddressOf());

			if (emissive != nullptr)
				D3DDeviceContext->PSSetShaderResources(5, 1, emissive->GetMapSRV().GetAddressOf());


			D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

			// 렌더 스테이트 설정.
			D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

			// 해당 내용 그리기
			D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[matCnt], 0, 0);

			// BoneRender.



			// 셰이더 초기화
			ID3D11ShaderResourceView* nullSRV[6] = { nullptr };

			D3DDeviceContext->PSSetShaderResources(0, 6, nullSRV);
		}
	}
}
