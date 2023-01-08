#pragma once

namespace GameEngineSpace
{
	class ColliderBase;

	/**
	 * \brief 트리거 이벤트를 발동시킬 수 있는 컴포넌트들에 붙는 트리거 콜라이더.
	 *
	 * 해당 클래스를 상속받은 후 트리거 이벤트가 발생하면 다형성을 이용해서 다르게 동작하게 만들 수 있다.
	 *
	 * 2022.08.03(수) Dev.BlackFoot
	 */
	class ITriggerable
	{
		// 마찬가지로 구현이 안되어도 괜찮기 때문에 abstract를 붙이지 않는다.
		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) {}
		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) {}
		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) {}

		friend class GameObject;
	};

}
