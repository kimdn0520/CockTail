#pragma once

namespace ClientSpace
{
	class GameManager
	{
	public:
		GameManager() {};
		~GameManager() {};

		void InitScene();

	private:
		std::shared_ptr<SceneManager> sceneManager;
	};
}

