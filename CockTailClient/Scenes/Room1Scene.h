#pragma once
#include "SceneBase.h"

namespace ClientSpace
{

	class Room1Scene : public GameEngineSpace::SceneBase
	{
	private:
		Vector3 playerEntryPos;
		Vector3 playerEntryRot;
		std::shared_ptr<GameObject> enemySpawn;

	public:
		Room1Scene(tstring sceneName);
		virtual ~Room1Scene();

	public:
		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();
		void SetScene();
	};
}