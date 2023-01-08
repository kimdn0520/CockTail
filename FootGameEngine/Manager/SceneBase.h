#pragma once
#include "Define/FootEngineTypedef.h"
//#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GameObject;

	class Camera;

	class RendererBase;

	class ColliderBase;

	class Animator;

	/**
	 * \brief 내부적으로 게임 오브젝트 등을 가지고 있는 ScenceBase
	 *
	 * 기본적으로 씬에 배치된 게임 오브젝트들만을 update, Render 할 수 있다.
	 *
	 * 클라이언트 영역에서 해당 베이스를 상속받은 씬들을 만들어서 게임 로직을 작성할 수 있다.
	 *
	 * 2022.07.25(월) Dev.BlackFoot
	 */
	class SceneBase : public std::enable_shared_from_this<SceneBase>
	{
		tstring sceneName;

		std::shared_ptr<Camera> mainCamera;

		std::vector<std::shared_ptr<GameObject>> gameObjectInScene;

		// 렌더링 가능 오브젝트 모아둔 벡터
		std::vector<std::shared_ptr<RendererBase>> renderObjInScene;

		// 충돌 가능 오브젝트 모아둔 벡터
		std::vector<std::shared_ptr<ColliderBase>> colliderObjInScene;

		// 포인트 라이트
		//std::vector<GraphicsEngineSpace::PointLight*> pointLights;

		// 스카이 박스를 달아줍시다.
			// 그래픽스 엔진의 리소스 매니저에서 관리해주기 때문에 포인터를 사용해준다.
		//GraphicsEngineSpace::SkyBoxBase* skyBox;
		// IBL을 위한 변수
		//GraphicsEngineSpace::IBLTexture* ibl;

		// 디버그 렌더러 용 함수 => 스태틱으로 만들어서 모든 씬에서 가능하게.
		static bool debugRender;

	public:
		SceneBase(tstring sceneName = TEXT("SampleScene"));

		virtual ~SceneBase();
		// 각종 이벤트 함수..
			// 대부분의 경우 내부의 오브젝트의 함수들을 불러준다.
		void Awake();
		void Start();
		void PreUpdate(float tick = 0.0f);
		void Update(float tick = 0.0f);
		void LateUpdate(float tick = 0.0f);

		// 모든 렌더러 컴포넌트를 렌더러(그래픽 엔진)에 넘겨준다.
		void Render(float tick);

		void OnEnable();

		// 큐브맵 설정
		/*void SetSkyBox(const tstring& skyBoxName);
		void SetIBL(const tstring& iblName, const tstring& radiancePath, const tstring& irradiancePath);*/


		// 카메라 설정
		void SetMainCamera(std::shared_ptr<Camera> _camera) { mainCamera = _camera; }
		std::shared_ptr<Camera> GetMainCamera() { return mainCamera; }

		std::vector<std::shared_ptr<GameObject>> GetGameObjectInScene() { return gameObjectInScene; }

		//
		//GraphicsEngineSpace::PointLight* CreatePointLight();

		virtual void BuildScene() abstract;

		void ClearScene();

	private:
		void AddGameObject(std::shared_ptr<GameObject> _newGameObj);

	public:
		// 빈 게임 오브젝트를 생성한다.
		std::shared_ptr<GameObject> CreateEmpty();

		// 이후 특정 게임 오브젝트에 렌더러나 콜라이더를 추가하는 함수를 넣어준다
		// 해당 컴포넌트를 가지고 있는 게임오브젝트의 Awake시 불려진다.
		void AddRenderer(std::shared_ptr<RendererBase> renderObj);
		void AddCollider(std::shared_ptr<ColliderBase> colliderObj);

		// 씬 이름 획득
		tstring GetSceneName() { return sceneName; }

		// SceneManager에 쉽게 접근하기 위해 friend 클래스 선언
		friend class SceneManager;
	};
}

