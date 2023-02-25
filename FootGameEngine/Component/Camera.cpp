#include "GamePch.h"
#include "Camera.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Object/GameObject.h"
#include "WindowManager.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	Camera::Camera(std::weak_ptr<GameObject> _obj) :
		ComponentBase(_obj, ComponentType::CAMERA)
		, projType(ProjType::PERSPECTIVE)
		, shakingIntensity(0.0f)
		, originalZ(0.0f)
	{
		// 트랜스폼 캐싱
		transform = GetGameObject()->GetComponent<Transform>();

		// 카메라 갱신시 종횡비가 1.0이었기 때문에 찌그러졌었다.
		float _aspect = static_cast<float>(WindowManager::GetInstance()->GetClientWidth()) / WindowManager::GetInstance()->GetClientHeight();

		// 프러스텀 생성
		//viewFrustum = std::make_shared<BoundingFrustum>();

		// 각각의 정보는 SetLens로..
		SetLens(0.25f * FootMath::Pi, _aspect, 1.0f, 500.0f);

		UpdateViewMatrix();

		// 큐브맵 생성
		skyBox = Factory::GetInstance()->CreateDXObject<SkyBox>(
		BuilderManger::GetInstance()->GetBuilder("SkyBoxBuilder"), "skyBox");

		//auto skyBoxBuilder = BuilderManger::GetInstance()->GetBuilder("SkyBoxBuilder");

		//skyBoxBuilder->AddTexture(skyBox, 1, "skyBoxTex", L"Resources/Texture/snowcube1024.dds", RenderingData::TextureMapType::CUBE);

		GraphicsManager::GetInstance()->GetRenderer()->InitObject(skyBox);
	}

	Camera::~Camera()
	{
	}

	void Camera::Release()
	{
		transform.reset();
		//viewFrustum.reset();

		__super::Release();
	}

	void Camera::UpdateViewMatrix()
	{
		// ViewMatrix는 결국.. 카메라의 월드 TM의 역행렬이다.
			// 카메라의 WorldTM => 카메라를 월드의 위치로 이동시켜주는 행렬(view => World로)
			// 카메라의 WorldTM의 역행렬 => 특정 오브젝트에 곱해주면.. 카메라를 중심으로 한 좌표계로 옮겨줄 수 있다.
		//transform->GetWorldTM().Invert(view);

		//view = ViewMatrix(GetGameObject()->GetComponent<Transform>()->GetWorldPosition(), GetGameObject()->GetComponent<Transform>()->GetWorldRotation());


		// 트랜스폼 look right up
		// SetLook을 하는게 문제긴 했는데.. 사실 이거 안하면 방향이 이상해지긴함.
		// 그래서 look right up을 여기서 세팅 해줘야하는데..

		// 카메라의 look => 카메라가 지금 보고 있는 부분 즉. 카메라의 현재 transform에서 z방향으로 뻗어 나간 부분.
		//transform->SetLook(view * Vector::UnitZ);

		// view 행렬의 Z값을 뽑으려면
			// view * UnitZ가 되어야한다..
			// 여기서는 벡터가 먼저 곱해지므로, 해당 값을 얻기 위해서 전치를 해줘야한다.
			// 해당 값이 카메라의 look을 명확하게 나타내고자 한다면 view 매트릭스의 UnitZ 성분이 현재 카메라의 look을 나타내야함.
		//Vector4 newLook = Vector4::Transform(Vector4::UnitZ, transform->GetWorldTM());
		//auto worldTM = transform->GetWorldTM();


		//transform->SetLook(Vector3{newLook.x, newLook.y, newLook.z});

		Vector3 axisX = transform->GetRight();			// 카메라 x축
		Vector3 axisY = transform->GetUp();					// 카메라 y축
		Vector3 axisZ = transform->GetLook();				// 카메라 z축 (look벡터를 z축 삼는다.) - 카메라가 바라보는 지점
		Vector3 camPos = transform->GetLocalPosition();

		axisZ.Normalize();									// z normalize
		axisY = axisZ.Cross(axisX);							// x축이랑 외적해서 y 구하고
		axisY.Normalize();
		axisX = axisY.Cross(axisZ);							// y구한거를 z랑 외적해서 x구하고

		float x = -camPos.Dot(axisX);
		float y = -camPos.Dot(axisY);
		float z = -camPos.Dot(axisZ);

		Matrix V
		{
			axisX.x, axisY.x, axisZ.x, 0.f,
			axisX.y, axisY.y, axisZ.y, 0.f,
			axisX.z, axisY.z, axisZ.z, 0.f,
			x, y, z, 1.0f
		};

		view = V;
	}

	void Camera::UpdateProjMatrix(ProjType _projType)
	{
		float width = static_cast<float>(WindowManager::GetInstance()->GetClientWidth());
		float height = static_cast<float>(WindowManager::GetInstance()->GetClientHeight());

		if (_projType == ProjType::PERSPECTIVE)
		{
			if (nearZ <= 0.0f)
			{
				float tmpNearZ = 0.1f;
				proj = XMMatrixPerspectiveFovLH(fovY, aspect, tmpNearZ, farZ);
			}
			else
			{
				proj = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
			}
		}

		if(_projType == ProjType::ORTHOGRAPHIC)
			proj = XMMatrixOrthographicLH(width / 50, height / 50, nearZ, farZ);

		projType = _projType;
	}

	std::vector<SimpleMath::Vector3> Camera::ScreenToRayOrigin(const Vector2& mousePos)
	{
		float viewX;
		float viewY;
		XMVECTOR origin;
		XMVECTOR direction;
		
		if(projType == ProjType::PERSPECTIVE)
		{
			// Screen에서 받은 mousePos를 ViewSpace로 바꾸는 과정
			// 스크린 -> NDC -> View
			viewX = (+2.0f * mousePos.x / WindowManager::GetInstance()->GetClientWidth() - 1.0f) / proj(0, 0);
			viewY = (-2.0f * mousePos.y / WindowManager::GetInstance()->GetClientHeight() + 1.0f) / proj(1, 1);
			origin = { 0.f, 0.f, 0.f, 1.f };
			direction = { viewX, viewY, nearZ, 0.f };
		}
		else
		{
			auto ret = proj.Invert();


			// Screen에서 받은 mousePos를 ViewSpace로 바꾸는 과정
			// 스크린 -> NDC -> View
			viewX = (+2.0f * mousePos.x / WindowManager::GetInstance()->GetClientWidth() - 1.0f) * ret(0, 0);
			viewY = (-2.0f * mousePos.y / WindowManager::GetInstance()->GetClientHeight() + 1.0f) * ret(1, 1);
			origin = { viewX, viewY, nearZ, 1.f };
			direction = { 0.f, 0.f, 1.f, 1.f };
		}

		// ViewSpace -> WorldSpace로 바꿔서 ray를 정의한다.
		Matrix viewMatrixInv = XMMatrixInverse(nullptr, view);
		Vector3 rayOrigin = XMVector3TransformCoord(origin, viewMatrixInv);				// 월드 스페이스로~
		Vector3 rayDirection = XMVector3TransformNormal(direction, viewMatrixInv);		// 월드 스페이스로~
 		rayDirection.Normalize();	// 방향은 정규화 시켜준다.

		return std::vector<SimpleMath::Vector3>({ rayOrigin, rayDirection });
	}

	/*
	bool Camera::ViewFrustumCulling(std::shared_ptr<BoundingCube> boundingCube, const Matrix& worldMatrix)
	{
		// 혜성이의 분리축 이론을 이용했습니다.

		// 바운딩 볼륨이 없으면 그대로 그립니다.
		if (boundingCube == nullptr)
			return true;

		// 분리축을 찾으면 -> 충돌한게 아니니까 -> 컬 해야한다.
			// 찾지 못하면 -> 충돌 했다 -> 프러스텀 안에 있거나 경계에 있다 -> 그려야한다.

		float nearHalfH = tan(fovY / 2) * nearZ;
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		Vector3* localPoint = boundingCube->localPoint;
		Vector3 cubePoint[8];

		const Matrix worldViewMatrix = worldMatrix * view;

		// 현재 컬링 판별 대상의 로컬 버텍스를 월드 view 스페이스로 옮겨줍니다.
		for (int i = 0; i < 8; i++)
			cubePoint[i] = localPoint[i] * worldViewMatrix;

		// 축
		Vector3 axis[3] = { cubePoint[1] - cubePoint[0], cubePoint[2] - cubePoint[0], cubePoint[3] - cubePoint[0] };

		// 길이
		float extents[3] = { axis[0].Length(), axis[1].Length(), axis[2].Length() };

		// 정규화 해줍니다.
		for (int i = 0; i < 3; i++)
		{
			axis[i] /= extents[i];

			// 정규화 뒤, half길이로 세팅해줍니다.
			extents[i] *= 0.5f;
		}

		// 센터 좌표도 같은 공간 상으로 옮겨줍니다.
		Vector3 center = boundingCube->localCenter * worldViewMatrix;

#pragma region FrustumPlane
		{
			// 변수명 중복을 막기위해 스코프로 분리.
			BoundingPlane* viewPlanes = viewFrustum->planes;

			// 0 near / 1 far / 2 left / 3 bottom / 4 right / 5 top
			for (int i = 0; i < 6; i++)
			{
				// 노말 벡터가 near와 평행.. far는 하지 않아도 된다.
				if (i == 1)
					continue;

				float MDotX = fabsf(viewPlanes[i].normal.x);
				float MDotY = fabsf(viewPlanes[i].normal.y);
				float MDotZ = viewPlanes[i].normal.z;

				float MDotC = Vector3Dot(viewPlanes[i].normal, center);

				float obbRadius = 0.f;

				// 정규화
				for (size_t j = 0; j < 3; j++)
					obbRadius += fabsf(Vector3Dot(viewPlanes[i].normal, axis[i]) * extents[i]);

				float obbMin = MDotC - obbRadius;
				float obbMax = MDotC + obbRadius;

				float p = nearHalfW * MDotX + nearHalfH * MDotY;

				float tau0 = nearZ * MDotZ - p;
				float tau1 = nearZ * MDotZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region OBB Plane
		{
			for (size_t i = 0; i < 3; i++)
			{
				const Vector& M = axis[i];

				float MoX = fabsf(M.x);
				float MoY = fabsf(M.y);
				float MoZ = M.z;

				float MoC = Vector3Dot(M, center);

				float obbRadius = extents[i];

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Edge R x A_i
		{
			for (size_t m = 0; m < 3; m++)
			{
				const Vector M = { 0.f, -axis[m].z, axis[m].y };

				float MoX = 0.f;
				float MoY = fabsf(M.y);
				float MoZ = M.z;
				float MoC = M.y * center.y + M.z * center.z;

				float obbRadius = 0.f;

				for (size_t i = 0; i < 3; i++)
					obbRadius += fabsf(Vector3Dot(axis[i], M) * extents[i]);

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Edge U x A_i
		{
			for (size_t m = 0; m < 3; m++)
			{
				const Vector M = { axis[m].z, 0.f, axis[m].y };

				float MoX = fabsf(M.x);
				float MoY = 0.f;
				float MoZ = M.z;
				float MoC = M.x * center.x + M.z * center.z;

				float obbRadius = 0.f;

				for (size_t i = 0; i < 3; i++)
					obbRadius += fabsf(Vector3Dot(axis[i], M) * extents[i]);

				float obbMin = MoC - obbRadius;
				float obbMax = MoC + obbRadius;

				// 프러스텀 proj
				float p = nearHalfW * MoX + nearHalfH * MoY;
				float tau0 = nearZ * MoZ - p;
				float tau1 = nearZ * MoZ + p;

				if (tau0 < 0.0f)
					tau0 *= farZ / nearZ;
				if (tau1 > 0.0f)
					tau1 *= farZ / nearZ;

				if (obbMin > tau1 || obbMax < tau0)
					return false;
			}
		}
#pragma endregion

#pragma region Frustum Edge x A_i
		{
			for (size_t obbEdgeIdx = 0; obbEdgeIdx < 3; obbEdgeIdx++)
			{
				const Vector M[] =
				{
					Vector3Cross(axis[obbEdgeIdx], Vector{-nearHalfW, 0.f, nearZ}),		// Left Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{nearHalfW, 0.f, nearZ}),		// Right Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{0.f, nearHalfH, nearZ}),		// Top Plane
					Vector3Cross(axis[obbEdgeIdx], Vector{0.f, -nearHalfH, nearZ})		// Bottom Plane
				};

				for (size_t m = 0; m < 4; m++)
				{
					float MoX = fabsf(M[m].x);
					float MoY = fabsf(M[m].y);
					float MoZ = M[m].z;

					const float epsilon = 0.000f;
					if(MoX < epsilon && MoY < epsilon && fabsf(MoZ) < epsilon)
						continue;

					float MoC = Vector3Dot(M[m], center);

					float obbRadius = 0.f;

					for (size_t i = 0; i < 3; i++)
						obbRadius += fabsf(Vector3Dot(axis[i], M[m]) * extents[i]);

					float obbMin = MoC - obbRadius;
					float obbMax = MoC + obbRadius;

					// 프러스텀 proj
					float p = nearHalfW * MoX + nearHalfH * MoY;
					float tau0 = nearZ * MoZ - p;
					float tau1 = nearZ * MoZ + p;

					if (tau0 < 0.0f)
						tau0 *= farZ / nearZ;
					if (tau1 > 0.0f)
						tau1 *= farZ / nearZ;

					if (obbMin > tau1 || obbMax < tau0)
						return false;
				}
			}
		}
#pragma endregion

		// 위의 모든 행동에도 분리축을 찾을 수 없으면. 화면상에 보여야 한다..
		return true;
	}

	void Camera::UpdateProjMatrix()
	{
		// 내부적으로 XMMatrixPerspectiveFovRH를 사용한다. 참고!
		//m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect, m_NearZ, m_FarZ);

		proj = PerspectiveMatrix(fovY, aspect, nearZ, farZ);

		// 프러스텀 업데이트.
		UpdateViewFrustum();
	}
	*/

 	/*void Camera::UpdateViewFrustum()
	{
		// near와 far의 길이, 너비 절반씩을 구한다.

		// 높이 절반은.. y축 시야각 절반의 tan에 nearZ 값을 곱한다..
		float nearHalfH = tan(fovY / 2) * nearZ;
		// 종횡비로 너비를 구해준다.
		float nearHalfW = (aspect * 2 * nearHalfH) / 2;

		// 같은 방식으로 farplane의 길이 너비의 반도 구해준다.
		float farHalfH = tan(fovY / 2) * farZ;
		float farHalfW = (aspect * 2 * farHalfH) / 2;

		// 평면을 구하는데 필요한 점 8개 (각각 near와 far에 위치한 점 8개)
		Vector3 nearLeftTop = Vector3(-nearHalfW, nearHalfH, nearZ);
		Vector3 nearLeftBottom = Vector3(-nearHalfW, -nearHalfH, nearZ);
		Vector3 nearRightTop = Vector3(nearHalfW, nearHalfH, nearZ);
		Vector3 nearRightBottom = Vector3(nearHalfW, -nearHalfH, nearZ);

		Vector3 farLeftTop = Vector3(-farHalfW, farHalfH, farZ);
		Vector3 farLeftBottom = Vector3(-farHalfW, -farHalfH, farZ);
		Vector3 farRightTop = Vector3(farHalfW, farHalfH, farZ);
		Vector3 farRightBottom = Vector3(farHalfW, -farHalfH, farZ);

		// Near Plane
		viewFrustum->planes[0] = BoundingPlane(nearLeftTop, nearRightTop, nearLeftBottom);
		// Far Plane
		viewFrustum->planes[1] = BoundingPlane(farRightTop, farLeftTop, farRightBottom);
		// Left Plane
		viewFrustum->planes[2] = BoundingPlane(farLeftTop, nearLeftTop, farLeftBottom);
		// Bottom Plane
		viewFrustum->planes[3] = BoundingPlane(nearLeftBottom, nearRightBottom, farLeftBottom);
		// Right Plane
		viewFrustum->planes[4] = BoundingPlane(nearRightTop, farRightTop, nearRightBottom);
		// Top Plane
		viewFrustum->planes[5] = BoundingPlane(nearRightTop, farRightTop, nearLeftTop);

		// far의 코너를 넣어준다
		// leftTop
		viewFrustum->frustumCorner[0] = Vector4(farLeftTop.x, farLeftTop.y, farLeftTop.z, 1.0f);
		// rightTop
		viewFrustum->frustumCorner[1] = Vector4(farRightTop.x, farRightTop.y, farRightTop.z, 1.0f);
		// leftBottom
		viewFrustum->frustumCorner[2] = Vector4(farLeftBottom.x, farLeftBottom.y, farLeftBottom.z, 1.0f);
		// rightBottom
		viewFrustum->frustumCorner[3] = Vector4(farRightBottom.x, farRightBottom.y, farRightBottom.z, 1.0f);
	}*/


	const Matrix& Camera::GetViewMatrix()
	{
		return view;
	}

	const Matrix& Camera::GetProjMatrix()
	{
		return proj;
	}

	const Matrix& Camera::GetIsometricProjMatrix()
	{
		return isoProj;
	}

	void Camera::Update(float tick)
	{
		UpdateViewMatrix();

		/*if (shakingIntensity > 5.0f)
		{
			static bool flag = true;

			Vector3 rotation = transform->GetWorldRotation();

			if (flag == true)
			{
				rotation.z = originalZ + shakingIntensity * tick * 10.0f;
				flag = false;
			}
			else
			{
				flag = true;
				rotation.z = originalZ - shakingIntensity * tick * 10.0f;
			}

			shakingIntensity *= (1.0f - 1.0f * tick * 10.0f);

			transform->SetRotation(rotation, Space::WORLD);
		}
		else
		{
			Vector3 rotation = transform->GetWorldRotation();

			rotation.z = originalZ;

			transform->SetRotation(rotation, Space::WORLD);

			shakingIntensity = 0.0f;
		}*/

		Matrix _view = view;
		_view.m[3][0] = 0.f;
		_view.m[3][1] = 0.f;
		_view.m[3][2] = 0.f;
		_view.m[3][3] = 1.f;

		skyBox->Update(Matrix::Identity, _view, proj);
	}

	void Camera::Render()
	{
		skyBox->Render();
	}

	void Camera::SetLens(float _fovY, float _aspect, float _nearZ, float _farZ)
	{
		// 매개 변수로 받은 값으로 멤버 변수 초기화
		fovY = _fovY;
		aspect = _aspect;
		nearZ = _nearZ;
		farZ = _farZ;

		// Proj 업데이트.
		UpdateProjMatrix(projType);
	}

	void Camera::SetShaking(float intensity)
	{
		if (shakingIntensity > 0.0f)
		{
			Vector3 rotation = transform->GetWorldRotation();

			rotation.z = originalZ;

			transform->SetRotation(rotation, Space::WORLD);
		}

		shakingIntensity = intensity;
		originalZ = transform->GetWorldRotation().z;
	}
}