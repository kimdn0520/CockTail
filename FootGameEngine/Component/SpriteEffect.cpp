#include "GamePch.h"
#include "SpriteEffect.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Camera.h"
#include "Object/GameObject.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "GraphicsManager.h"

using namespace GraphicsEngineSpace;

namespace GameEngineSpace
{
	SpriteEffect::SpriteEffect(std::weak_ptr<GameObject> object)
		: RendererBase(object)
	{
	}

	void SpriteEffect::Init(std::string objName)
	{
		renderObj = Factory::GetInstance()->CreateDXObject<GraphicsEngineSpace::SpriteEffect>(
			BuilderManger::GetInstance()->GetBuilder("SpriteEffectBuilder"), objName);

		GraphicsManager::GetInstance()->GetRenderer()->AddRenderObj(renderObj);
	}

	void SpriteEffect::Render()
	{

	}

	void SpriteEffect::PrepareRender(float tick)
	{
		std::shared_ptr<Camera> mainCam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(transform->GetWorldTM(), mainCam->GetViewMatrix(), mainCam->GetProjMatrix());

		/*auto effect = std::dynamic_pointer_cast<GraphicsEngineSpace::SpriteEffect>(renderObj);

		SimpleMath::Vector2 offset;

		if (effect != nullptr)
			offset = effect->GetTexOffset();

		effect->SetTexOffset(offset.x + 5.0f * tick, offset.y);

		if(offset.x > 1.0f)
			effect->SetTexOffset(-1.0f, offset.y);*/
	}

	void SpriteEffect::DebugIMGUIRender(float tick)
	{
		auto imgui = ImGUIManager::GetInstance();

		// offset을 변경해봅니다.
		auto effect = std::dynamic_pointer_cast<GraphicsEngineSpace::SpriteEffect>(renderObj);

		SimpleMath::Vector2 offset;

		if (effect != nullptr)
			offset = effect->GetTexOffset();

		if (imgui->Begin("Tex Offset"))
		{
			float offsetArr[2] = { offset.x, offset.y };
			imgui->DragFloat2("Offset", offsetArr, 0.01f, -1.f, 1.f);

			effect->SetTexOffset(offsetArr[0], offsetArr[1]);
		}
		imgui->End();
	}
}
