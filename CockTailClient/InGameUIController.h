#pragma once

namespace ClientSpace
{

	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	// 대화 상태에 대해 나타내는 enumClass
	enum class eDialogState
	{
		FIRST_CUT_SCENE,	// 첫 컷 씬

		STAGESTART,			// 스테이지 시작

		Bar,				// bar 

		TUTORIAL,			// 튜토리얼 상태.

		NONE				// 대화창 안뜨는 상태 = default
	};

	enum class eBarEvent
	{
		FIRST_CUT_SCENE,
		Bar,
		NONE
	};


	struct Dialog
	{
		// 각 스테이지 대사 중복 가능한 type
		uint32 typeID;
		std::wstring contents;

		Dialog(uint32 id, const std::wstring& content)
		{
			typeID = id;
			contents = content;
		}
	};


	class InGameUIController : public ComponentBase
	{

	public:
		InGameUIController(std::weak_ptr<GameObject> obj);
		~InGameUIController();

	private:
		std::shared_ptr<Canvas> mainCanvas;

	private:
		float tickCount;
		float moveSpeed;

		int currentHP;
		int maxHP;

		int currentBossHP;
		int bossMaxHP;

	public:
		// 페이드 인 , 아웃 true false
		// false일때 실행되는건데, 초기값을 false로 한 이유는 게임 시작할때 페이드인으로 들어가려고...그게 싫으면 true로 하면됨.
		bool stopIn = false;
		bool stopOut = true;


		// 난수 생성기..
		std::mt19937 engine;
		uniform_int_distribution<int> distribution;

	public:
		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		void SceneCheck();

		void SetHP(int HP);
		void SetMaxHP(int maxHP);

		void SetBossHP(int HP);
		void SetBossMaxHP(int maxHP);

		void SetCustomer();

		void BarEvent(int tickCount);

		void FadeIn();
		void FadeOut();

		void StartNowDialog();
		void startBarDialog();

		void CombineItem();

	};

}

