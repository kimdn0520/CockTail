#pragma once


namespace GameEngineSpace
{
	/**
	 * \brief 매니저들을 이용해서 씬을 업데이트 시키는 게임엔진
	 */
	class GameEngine
	{
	private:
		static std::shared_ptr<GameEngine> gameEngineInstance;

	public:
		void Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight);

		INT Tick();

		void Release();

		static std::shared_ptr<GameEngine> GetEngineInstance() { return gameEngineInstance; }
	};
}
