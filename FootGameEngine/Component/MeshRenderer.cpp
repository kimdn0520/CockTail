#include "GamePch.h"
#include "MeshRenderer.h"

#include "../../FootGraphicsEngine/Interface/IDXObject.h"
#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Component/Camera.h"
#include "Object/GameObject.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"

using namespace GraphicsEngineSpace;

namespace GameEngineSpace
{
	MeshRenderer::MeshRenderer(std::weak_ptr<GameObject> object)
		: RendererBase(object)
	{
	}

	void MeshRenderer::Init(std::string objName)
	{
		renderObj = Factory::GetInstance()->CreateDXObject<PBRObj>(
		BuilderManger::GetInstance()->GetBuilder("PBRBuilder"), objName);

		/*auto legacyBuilder = BuilderManger::GetInstance()->GetBuilder("LegacyStaticBuilder");
		legacyBuilder->AddTexture(renderObj, diffuseID, "albedo", diffuseMap, RenderingData::TextureMapType::ALBEDO);
		legacyBuilder->AddTexture(renderObj, normalID, "normal", normalMap, RenderingData::TextureMapType::NORMAL);*/

		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void MeshRenderer::Render()
	{
		//renderObj->Render();
	}

	void MeshRenderer::PrepareRender(float tick)
	{
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());

		renderObj->PreRender(tick);

		auto cameraWorldPos = mainCam->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

		GraphicsManager::GetInstance()->GetRenderer()->SetCameraPos(cameraWorldPos.x, cameraWorldPos.y, cameraWorldPos.z);
	}

	void MeshRenderer::PlayAnim(std::string animClipName, bool isLoop)
	{
		renderObj->PlayAnim(animClipName, isLoop);
	}
}
