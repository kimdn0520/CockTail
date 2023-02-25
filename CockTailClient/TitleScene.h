#pragma once
#include "SceneBase.h"

namespace ClientSpace
{
	class TitleScene : public GameEngineSpace::SceneBase
	{
	public:
		TitleScene(tstring sceneName);
		virtual ~TitleScene();

	public:
		virtual void BuildScene() override;


	private:
		std::shared_ptr<GameObject> CreateUI();
	};
}

