#pragma once

/**
 * \brief 혜성이의 오브젝트 구조를 참고하여 만든 오브젝트 클래스.
 */
namespace GameEngineSpace
{
	/**
	 * \brief 32비트형 정수 타입을 static 변수로 선언
	 *
	 * 오브젝트 클래스와 별개로 오브젝트의 ID만 생성하는 ID생성기 클래스입니다.
	 */
	class ObjectIDGenerator
	{
		static uint32 nextID;

	public:
		// 후위 증감 연산자를 통해서 리턴 뒤 ID를 증가시킨다.
		static uint32 IDGenerate() { return nextID++; }
	};

	/**
	 * \brief 게임에 존재하는 모든 오브젝트들의 최상위 부모 클래스 입니다.
	 *
	 * 단일 객체 생성을 방지하기 위해 abstract 키워드를 사용했습니다.
	 */
	class Object abstract
	{
		// 현재 오브젝트가 활성화 되어 있는가.
		bool isEnable;

		// 오브젝트 ID
		uint32 objectID;

	public:
		Object();
		virtual ~Object();

		// 오브젝트를 상속받는 모든 클래스는 릴리즈를 작성해줘야한다.
		virtual void Release() abstract;

	public:
		static void Destroy();

		// 씬이 전환되도 삭제하지 않을 오브젝트를 설정하는 함수
		static void DontDestroyOnLoad(std::shared_ptr<Object> targetObject);

		// Unity의 이벤트 함수의 실행순서를 참고하여 만든 오브젝트 각각의 실행시 수행될 함수.
			// 각 함수들의 경우 구현이 되지 않을 수도 있기 때문에 abstract를 붙이지 않음.
		virtual void Awake();

		virtual void Start();

		virtual void FixedUpdate(float tick);

		virtual void PreUpdate(float tick);

		virtual void Update(float tick);

		virtual void LateUpdate(float tick);

		virtual void DebugIMGUIRender(float tick);

		// 오브젝트가 활성화 될 때 불리는 함수
		virtual void OnEnable();

		// 반대로 비활성화 될 때 불리는 함수
		virtual void OnDisable();

		virtual void OnDestroy();

		uint32 GetObjectID() const { return objectID; }

		bool GetIsEnable() const { return isEnable; }

		void SetIsEnable(bool _isEnable) { isEnable = _isEnable; }
	};
}