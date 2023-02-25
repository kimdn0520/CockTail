#pragma once
#include "Object/ComponentBase.h"

//#include "Utils/ColliderUtils/BoundingVolumes.h"

namespace GraphicsEngineSpace
{
	class IDXObject;
}

namespace GameEngineSpace
{
	class Transform;

	// 투영 타입
		// 투영타입에 따라서 View, Proj 등이 달라짐.
	enum class ProjType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	/**
	 * \brief 컴포넌트를 상속받은 네임스페이스 버전 Camera
	 */
	class Camera final : public ComponentBase
	{
		// 위치 정보
			// 사실상 여기에 위에 작성한 위치정보와 look up right 벡터가 모두 들어있다.
		std::shared_ptr<Transform> transform;

		// View, Proj 행렬
		Matrix view;
		Matrix proj;
		Matrix isoProj;

		ProjType projType;

		// 절두체에 관련된 변수들.
		float nearZ;
		float farZ;
		float aspect;
		float fovY;

		// 이후에 ViewFrustum 관련 변수, 함수 추가.
			// ViewFrustum 컬링, 스카이 박스 등의 기능 추가시 필요.
		// Frustum
		//std::shared_ptr<BoundingFrustum> viewFrustum;
		std::shared_ptr<GraphicsEngineSpace::IDXObject> skyBox;

		// Shaking factor
		float shakingIntensity;
		float originalZ;

	public:
		Camera(std::weak_ptr<GameObject> _obj);
		virtual ~Camera();

		virtual void Release() override;

		// 각종 Getter Setter
		const float& GetAspectRatio() { return aspect; }
		// 화면 비가 변경되면 ProjMatrix도 변경되어야함.
		void SetAspectRatio(float _ratio) { aspect = _ratio; UpdateProjMatrix(projType); }
		const float& GetNearZ() { return nearZ; }
		void SetNearZ(float _nearZ) { nearZ = _nearZ; UpdateProjMatrix(projType); }
		const float& GetFarZ() { return farZ; }
		void SetFarZ(float _farZ) { farZ = _farZ; UpdateProjMatrix(projType); }
		const float& GetFov() { return fovY; }
		void SetFov(float _fov) { fovY = _fov; UpdateProjMatrix(projType); }

		// TM 반환.
			// View와 Proj 행렬은 여기 들어갑니다.
			// Proj * View * SRT 에서의 View, Proj
		const Matrix& GetViewMatrix();
		const Matrix& GetProjMatrix();
		const Matrix& GetIsometricProjMatrix();

		virtual void Update(float tick) override;
		void Render();

		// 프러스텀 설정.
		void SetLens(float _fovY, float _aspect, float _nearZ, float _farZ);

		void SetShaking(float intensity);

		void UpdateViewMatrix();

		void UpdateProjMatrix(ProjType _projType);

		std::vector<SimpleMath::Vector3> ScreenToRayOrigin(const Vector2& mousePos);

		// ViewFrustumCulling을 위한 함수 세팅
		//bool ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix);
	
	private:
		void UpdateViewFrustum();
	};
}
