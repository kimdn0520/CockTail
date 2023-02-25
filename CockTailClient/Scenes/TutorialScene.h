#pragma once
#include "SceneBase.h"

namespace ClientSpace
{
	class TutorialScene : public GameEngineSpace::SceneBase
	{
	private:
		Vector3 playerEntryPos;
		Vector3 playerEntryRot;

	public:
		TutorialScene(tstring sceneName);
		virtual ~TutorialScene();

	public:
		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();
		void SetScene();
	};
}