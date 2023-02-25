#pragma once

namespace ClientSpace
{
	class BarLobby : public GameEngineSpace::SceneBase
	{
		public:
			BarLobby(tstring sceneName);
			virtual ~BarLobby();
	
	public:
		virtual void BuildScene() override;


	private:
		std::shared_ptr<GameObject> CreateUI();


	};
}

