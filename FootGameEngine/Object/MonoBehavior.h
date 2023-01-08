#pragma once

#include "ComponentBase.h"
#include "ITriggerable.h"

namespace GameEngineSpace
{
	/**
	 * \brief 컴포넌트 화 될 수 있으면서, 트리거 이벤트를 받아서 트리거 이벤트를 실행 시킬 수 있는
	 * MonoBehavior 클래스
	 *
	 * 현재는 트리거 이벤트 밖에 없지만, 이후에 다른 이벤트들을 추가 가능하다.
	 *
	 * 2022.08.03(수) Dev.BlackFoot
	 */
	class MonoBehaviour : public ComponentBase, public ITriggerable
	{
	public:
		MonoBehaviour(std::weak_ptr<GameObject> gameObj)
			: ComponentBase(gameObj)
		{			
		}

		virtual ~MonoBehaviour()
		{			
		}
	};

}