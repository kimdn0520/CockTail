#pragma once

namespace GameEngineSpace
{
	/// 충돌 처리에 사용될 바운딩 볼륨 구조체들을 정리해둔 헤더 파일.
	///	3차원 큐브, 2차원 Rect, 3차원 평면, 3차원 절두체에 대한 bounding volume이 있다.

	/// <summary>
	/// 3차원 큐브 바운딩 볼륨
	/// </summary>
	struct BoundingCube
	{
		// 자신을 기준으로 했을 때의 스케일
		Vector3 localScale;
		// 자신을 기준으로 했을 때의 중심(위치 벡터)
		Vector3 localCenter;
		// 자신을 기준으로 했을 때의 매트릭스(회전 등을 고려..?)
		Matrix localMatrix;
		// 각 점의 위치 벡터.
		Vector3 localPoint[8];

		// 중심을 기준으로 scale 만큼의 크기로 볼륨을 잡는다.
		BoundingCube(const Vector3& _localScale, const Vector3& _localCenter, const Matrix& _localMatrix)
			: localScale(_localScale)
			, localCenter(_localCenter)
			, localMatrix(_localMatrix)
		{
			// 스케일 크기를 기준으로 절반을 얻는다.
			float halfX = localScale.x / 2;
			float halfY = localScale.y / 2;
			float halfZ = localScale.z / 2;

			// 큐브를 이루는 점들의 위치를 초기화 시켜줍니다.
			localPoint[0] = localCenter + Vector3{ -halfX, -halfY, -halfZ };
			localPoint[1] = localCenter + Vector3{ -halfX, halfY, -halfZ };
			localPoint[2] = localCenter + Vector3{ halfX, -halfY, -halfZ };
			localPoint[3] = localCenter + Vector3{ -halfX, -halfY, halfZ };
			localPoint[4] = localCenter + Vector3{ halfX, -halfY, halfZ };
			localPoint[5] = localCenter + Vector3{ halfX, halfY, -halfZ };
			localPoint[6] = localCenter + Vector3{ -halfX, halfY, halfZ };
			localPoint[7] = localCenter + Vector3{ halfX, halfY, halfZ };

		}
	};

	// 일반적인 2차원 rect
	struct BoundingRect
	{
		// lefttop과 rightbottom
		Vector2 leftTop;
		Vector2 rightBottom;

		BoundingRect(Vector2 _leftTop, Vector2 _rightBottom)
			: leftTop(_leftTop)
			, rightBottom(_rightBottom)
		{

		}
		BoundingRect(Vector4 rect)
			: leftTop(rect.x, rect.y)
			, rightBottom(rect.z, rect.w)
		{
		}
	};

	// 3차원 평면
	struct BoundingPlane
	{
		Vector3 normal;

		float distance;

		BoundingPlane()
			: normal(Vector3::Zero)
			, distance()
		{

		}

		// 3차원에서 점 세 개가 주어졌을 때 평면을 만들어줍니다.
		BoundingPlane(Vector3 first, Vector3 second, Vector3 third)
		{
			// 첫 점에서 second로 향하는 벡터
			Vector3 e1 = first - second;
			Vector3 e2 = first - third;

			// 반시계로 외적을 뽑아냅니다. CCW
				// => 해당 값이 이 평면의 노말이 됩니다.
			e1.Cross(e2, normal);
			normal.Normalize(normal);

			// 산술적인 의미의 dot연산
				// 평면의 방정식 => ax + by + cz = - d;
				// 여기서 a, b, c는 normal, x, y, z가 first
			distance = -normal.Dot(first);
		}
	};

	// 바운딩 프러스텀 (절두체..!)
	struct BoundingFrustum
	{
		BoundingPlane planes[6];

		// far plane의 코너 점 좌표
			// lefttop의 x, y, z, 1
			// rightTop의 x, y, z, 1
			// leftbottom의 x, y, z, 1
			// rightbottom의 x, y, z, 1.
		Vector4 frustumCorner[4];
	};
}
