#pragma once
#include "SceneBase.h"

namespace ClientSpace
{
	class BarOperationScene : public GameEngineSpace::SceneBase
	{
	public:
		BarOperationScene(tstring sceneName);
		virtual ~BarOperationScene();

	public:
		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();


	};
}

