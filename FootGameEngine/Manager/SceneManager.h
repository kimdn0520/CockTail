#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class SceneBase;
	class PhysicsManager;
	class GameObject;

	/**
	 * \brief Scene을 관리하는 SceneManager 클래스
	 *
	 * 해당 매니저 클래스를 실제로 게임엔진에서 가지고 있다. 싱글톤으로 구현한다.
	 *
	 * 2022.07.26(화) Dev.BlackFoot
	 */
	class SceneManager final
	{
		// 단 하나 존재하는 SceneManager 객체
		static std::shared_ptr<SceneManager> instance;

		// 물리매니저 캐싱용도
		std::shared_ptr<PhysicsManager> physicsManager;

	private:
		// 현재 씬
		std::shared_ptr<SceneBase> currentScene;
		// 게임 전체의 Scene
		std::map<tstring, std::shared_ptr<SceneBase>> scenes;

		// 다음으로 로드 될 씬 여부 확인용 변수
		bool reservedLoadScene;
		tstring reservedLoadSceneName;

		// Tick 변경용 delta에 곱해줄 float변수.
		float deltaMultiple;
		// 현재 틱에 변경이 있는가 알아보는 불변수
		bool isChangedTick = false;

		bool isChangeScene = false;

	public:
		SceneManager() = default;

		~SceneManager() = default;

		// 이동, 복사 생성자 제거
		SceneManager(const SceneManager & _other) = delete;
		SceneManager& operator= (const SceneManager & _other) = delete;
		SceneManager(SceneManager && _other) = delete;
		SceneManager& operator= (SceneManager && _other) = delete;
		
		static std::shared_ptr<SceneManager> GetInstance();

		void Init();

		void Release();

		void Update();

		void AddGameScene(std::shared_ptr<SceneBase> _scene);

		std::shared_ptr<SceneBase> GetCurrentScene() { return currentScene; }

		void LoadScene(tstring _sceneName);

		// 리사이즈 용..
		void UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight);

		// deltaTime 변경
		void ChangeTick(float multiple);
		// delatTime 원상 복귀
		void SetTickOrigin() { isChangedTick = false; }

		void LoadSceneData(std::string sceneName);

		std::shared_ptr<GameObject> FindObject(std::string uid);
		std::shared_ptr<GameObject> FindObjectName(tstring name);

	private:
		void ChangeScene();
	};
}
