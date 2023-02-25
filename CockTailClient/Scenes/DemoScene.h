#pragma once
#include "SceneBase.h"

namespace ClientSpace
{
	class DemoScene : public GameEngineSpace::SceneBase
	{
	public:
		DemoScene(tstring sceneName);
		virtual ~DemoScene();

	public:
		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();
	};
}