#pragma once
#include "Resources/ObjectResources.h"

/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IRenderer;

	/**
	 * \brief Factory에서 생성하는 모든 오브젝트의 베이스가 되는 DXObj..
	 *
	 * 작동 방식을 Interface로 뽑아야한다.
	 * 기본적으로 Device, DC. 각종 리소스를 가지고 있어야 한다.(해당 내용은 이 인터페이스를 상속받은 오브젝트들에 넣어준다)
	 * 2022.06.21(화) Dev.BlackFoot
	 */
	class GRAPHICSENGINE_DECLSPEC IDXObject
	{
	public:
		IDXObject() = default;
		virtual ~IDXObject() = default;

		// 초기화. Device와 DC를 받아서? 생각해보면 해당 내용은.. 게임엔진에서는 알 필요가 없긴하지만..
		// 스마트 포인터로 바꿔줘도 무방할 듯. 
		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) abstract;
		// 각종 행렬을 받아서 업데이트
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) abstract;
		
		// tick을 받아서 업데이트 해주는 PreRender
		virtual void PreRender(float tick) abstract;
		// 내부의 오브젝트가 멤버 변수로 각 디바이스의 포인터를 가지고 있는 것이 나을까..
		virtual void Render() abstract;

		// 파서 부분을 가져오는 것..은.. 또 어떻게 하지..
			// 빌더에서 만들어서 오브젝트를 Device에 넣어준다.
		// 오브젝트에 정보 넣어주기
		virtual void SetMeshResources(std::shared_ptr<MeshResources> meshResources) abstract;
		virtual void SetBoneResource(std::shared_ptr<BoneResources> boneResources) {}
		virtual void SetAnimationResource(std::shared_ptr<AnimationResources> animResources) {}

		// 오브젝트를 분류할 수 있는 이름을 넣어주자..
		virtual std::string GetObjName() abstract;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() abstract;
		virtual std::shared_ptr<BoneResources> GetBoneResources() { return nullptr; }
		//virtual std::shared_ptr<AnimationResources> GetAnimationResources() { return nullptr; }

		// 각종 Matrix Getter
		virtual const SimpleMath::Matrix& GetWorld() abstract;
		virtual const SimpleMath::Matrix& GetView() abstract;
		virtual const SimpleMath::Matrix& GetProj() abstract;

		////////////////////////////////////////////////////////////////////////////////////////////////
		//										Animator 관련
		////////////////////////////////////////////////////////////////////////////////////////////////
		 
		// 애니메이션 실행용
		virtual void PlayAnim(std::string animClipName, bool isLoop) {};

		// State 이름, 클립 이름, 재생 속도, 루프 여부
		virtual void AddAnimationState(const std::string& stateName, const std::string& animClipName, float speed, bool loop) {};

		// Transition을 추가할 State이름, 시작 State이름, 끝 State이름, hasExitTime, exitTime, Transition Duration
		virtual void SettingAnimationStateTransition(const std::string& stateName, const std::string& stateStart, const std::string& stateEnd, bool hasExitTime, float exitTime, float transitionDuration) {};
		
		// Entry로 설정할 Aniamtion State
		virtual void EntryPlayAnimationState(const std::string& stateName) {};
		
		// Transition 이름, Parameter 이름, 조건 이름, 값(없는건 트리거)
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val) {};
		virtual void AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName) {};

		virtual void AddFloat(const std::string& name, float val) {};
		virtual void AddInteger(const std::string&, int val) {};
		virtual void AddBool(const std::string&, bool val) {};
		virtual void AddTrigger(const std::string& name) {};

		virtual void SetFloat(const std::string& name, float val) {};
		virtual void SetInteger(const std::string& name, int val) {};
		virtual void SetBool(const std::string& name, bool val) {};
		virtual void SetTrigger(const std::string& name) {};

		virtual int GetAnimCurrentFrame() { return 0; }
		virtual int GetAnimCurrentTotalFrame() { return 0; }
	};
}
