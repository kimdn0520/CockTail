#pragma once
#include "SceneBase.h"

namespace ClientSpace
{
	class LobbyScene : public GameEngineSpace::SceneBase
	{
	private:
		Vector3 playerEntryPos;
		Vector3 playerEntryRot;

	public:
		LobbyScene(tstring sceneName);
		virtual ~LobbyScene();

	public:
		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();
		void SetScene();
	};
}