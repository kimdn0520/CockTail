#include "GamePch.h"
#include "Transform.h"

#include "Object/GameObject.h"
// #include "Component/Camera.h"

namespace GameEngineSpace
{
	Transform::Transform(std::weak_ptr<GameObject> _gameObject) :
		ComponentBase(_gameObject, ComponentType::TRANSFORM)
		, localTM(Matrix::Identity)
		, worldTM(Matrix::Identity)
		, localPosition(Vector3::Zero)
		, localRotation(Vector3::Zero)
		, localScale(Vector3(1.0f, 1.0f, 1.0f))
		, worldPosition(Vector3::Zero)
		, worldRotation(Vector3::Zero)
		, worldScale(Vector3(1.0f, 1.0f, 1.0f))
		, look(Vector3::UnitZ)
		, up(Vector3::UnitY)
		, right(Vector3::UnitX)
		, needTMUpdate(true)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Release()
	{
		__super::Release();
	}

	void Transform::Update(float tick)
	{
		this->UpdateLocalTM();
		this->UpdateWorldTM();
	}

	void Transform::UpdateLocalTM()
	{
		// 내부적으로 Local TM을 계산해줍니다.
			// SRT 순으로 계산해주며, R의 경우 Z Roll X Pitch Y Yaw 순으로 회전 합니다.
		//localTM = MatrixScalingFromVector(localScale) * MatrixRotationFromVector(localRotation) * MatrixTranslationFromVector(localPosition);
		localTM = Matrix::CreateScale(localScale) * XMMatrixRotationQuaternion(EulerToQuat(localRotation)) * Matrix::CreateTranslation(localPosition);

		// 로컬이 변경되었기 때문에.. TM 업데이트가 필요하다고 체크 해줍니다.
			// 월드 TM을 변경하고 난 뒤 TM 업데이트가 필요 하지 않다고 해줍니다.
		CheckingNeedTMUpdate();
	}

	void Transform::UpdateWorldTM()
	{
		// 부모와의 관계가 반드시 필요..
		// 부모의 월드TM에 자신의 Local을 곱해준 것 => 자신의 WorldTM
		// 부모의 현재 위치를 자신의 로컬 원점으로 보고.. 거기서 로컬 TM 만큼 옮겨준 것이 자신의 World TM이다.

		// 부모를 얻어와서.. 부모의 월드에 자신의 로컬을 곱해준다.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			// 부모가 존재한다면..
			// 부모의 월드에 자신의 로컬을 곱해줍니다.
			worldTM = localTM * (_parent->GetComponent<Transform>()->GetWorldTM());

			// 월드의 요소를 분리해서 각각의 스케일, 로테이션, 포지션에 넣어줍니다.
			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}
		else
		{
			// 부모가 없는 경우. 로컬TM을 그대로 WorldTM으로 넣어줍니다.
			worldTM = localTM;

			DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		}

		// look up right를 수정해줍니다.
		// World TM을 기준으로 노말 트랜스폼을 해줍니다..
		Vector3::TransformNormal(Vector3::UnitZ, worldTM, look);
		Vector3::TransformNormal(Vector3::UnitY, worldTM, up);
		Vector3::TransformNormal(Vector3::UnitX, worldTM, right);
		/*right = Vector3::UnitX * worldTM;
		up = Vector3::UnitY * worldTM;
		look = Vector3::UnitZ * worldTM;

		// 각 벡터 정규화
		right = Vector3Normalize(right);
		up = Vector3Normalize(up);
		look = Vector3Normalize(look);*/

		// WorldTM을 변경 했기 때문에 TM Update가 필요하지 않습니다..
		needTMUpdate = false;
	}


	void Transform::CheckingNeedTMUpdate()
	{
		needTMUpdate = true;

		// 이 게임 오브젝트의 모든 자식들의 TM 업데이트가 필요하도록 해줍니다.
		// 현재 변경된 오브젝트의 TM이 변경되었기 때문에 자연스럽게 자식들의 TM도 변경되어야 하기 때문입니다.

		std::vector<std::shared_ptr<GameObject>>& children = this->GetGameObject()->GetChildren();

		// 모든 자식의 트랜스폼에 TM Update가 필요하도록!
		for (auto& child : children)
		{
			child->GetComponent<Transform>()->CheckingNeedTMUpdate();
		}
	}

	const DirectX::SimpleMath::Quaternion& Transform::GetWorldRotationQuat()
	{
		DirectX::SimpleMath::Quaternion _wRot = EulerToQuat(worldRotation);
		return _wRot;
	}

	void Transform::LookAt(const Vector3& targetPos)
	{
		Vector3 nowPos = Vector3{GetWorldTM().m[3][0], GetWorldTM().m[3][1], GetWorldTM().m[3][2]};
		if (targetPos - nowPos == Vector3::Zero)
			return;

		// Eye direction을 포커스 위치에서 지금 위치를 빼서 구한 뒤.
		// up direction으로 look At 계산
		Vector3 eyeDirection = targetPos - nowPos;

		// 지훈이형의 코드
		assert((eyeDirection == Vector3::Zero) != true);
		assert((isinf(eyeDirection.x) || isinf(eyeDirection.y) || isinf(eyeDirection.z)) != true);
		// UnitY를 쓸 것이기 때문에 up direction에 대한 검증은 따로 하지 않는다.
		Vector3 _look;
		eyeDirection.Normalize(_look);

		Vector3 _right;
		Vector3::UnitY.Cross(_look, _right);
		_right.Normalize(_right);

		Vector3 _up;
		_look.Cross(_right, _up);

		Matrix matrix = Matrix{_right, _up, _look};
		matrix.Transpose(matrix);

		right = Vector3{ matrix.m[0][0], matrix.m[1][0], matrix.m[2][0] };
		up = -Vector3{ matrix.m[0][1], matrix.m[1][1], matrix.m[2][1] };
		look = Vector3{ matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };

		// 각 벡터 정규화
		right.Normalize(right);
		up.Normalize(up);
		look.Normalize(look);

		matrix.Invert(matrix);
		Vector3 rotation = matrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);

		SetRotation(rotation, Space::LOCAL);
	}

	void Transform::SetLook(const Vector3& _look)
	{
		// Look이 바뀌었으니 나머지도 바꿔주자.
		look = _look;
		// 왼손잡이로 변경
		look.Normalize(look);

		// look과 월드 업을 이용해서 right를 뽑아준다.
		Vector3::UnitY.Cross(look, right); // right = UnitY X look
		//look.Cross(Vector3::UnitY, right);
		//right = -right;
		right.Normalize(right);


		look.Cross(right, up);
		//right.Cross(look, up);
		//up = -up;
		up.Normalize(up);

		Matrix matrix = Matrix{ right, up, look };
		matrix.Transpose(matrix);

		right = Vector3{ matrix.m[0][0], matrix.m[1][0], matrix.m[2][0] };
		up = -Vector3{ matrix.m[0][1], matrix.m[1][1], matrix.m[2][1] };
		look = Vector3{ matrix.m[0][2], matrix.m[1][2], matrix.m[2][2] };

		// 각 벡터 정규화
		right.Normalize(right);
		up.Normalize(up);
		look.Normalize(look);

		matrix.Invert(matrix);
		Vector3 rotation = matrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);

		SetRotation(rotation, Space::LOCAL);
	}

	// 현재 코드에 문제가 있습니다..
		// Up 벡터와 Right벡터로 나머지 기저 벡터를 구하는 것은 무리가 있는가..
	void Transform::SetUp(const Vector3& _up)
	{
		up = _up;
		up.Normalize(up);

		up.Cross(Vector3::UnitZ, right);
		right.Normalize(right);

		right.Cross(up, look);
		look.Normalize(look);
	}

	void Transform::SetRight(const Vector3& _right)
	{
		right = _right;
		right.Normalize(right);

		Vector3::UnitZ.Cross(right, up);
		up.Normalize(up);

		right.Cross(up, look);
		look.Normalize(look);
	}

	void Transform::DecomposeMatrix(Vector3& scale, Vector3& rotation, Vector3& position, Matrix matrix)
	{
		// Position, Scale
		position.x = matrix.m[3][0];
		position.y = matrix.m[3][1];
		position.z = matrix.m[3][2];

		scale.x = sqrtf(powf(matrix.m[0][0], 2.0f) + powf(matrix.m[0][1], 2.0f) + powf(matrix.m[0][2], 2.0f));
		scale.y = sqrtf(powf(matrix.m[1][0], 2.0f) + powf(matrix.m[1][1], 2.0f) + powf(matrix.m[1][2], 2.0f));
		scale.z = sqrtf(powf(matrix.m[2][0], 2.0f) + powf(matrix.m[2][1], 2.0f) + powf(matrix.m[2][2], 2.0f));

		Vector3 matrix0 = Vector3{ matrix.m[0] };
		Vector3 matrix1 = Vector3{ matrix.m[1] };
		Vector3 matrix2 = Vector3{ matrix.m[2] };

		Vector3 def;
		matrix0.Cross(matrix1, def);

		// Negative Scale
		if (def.Dot(matrix2) < 0.0f)
		{
			scale *= -1.0f;
		}

		// Rotation
		matrix0 /= scale.x;
		matrix1 /= scale.y;
		matrix2 /= scale.z;

		Matrix rotMatrix = Matrix{matrix0, matrix1, matrix2};

		rotation = rotMatrix.ToEuler();

		rotation.x = XMConvertToDegrees(rotation.x);
		rotation.y = XMConvertToDegrees(rotation.y);
		rotation.z = XMConvertToDegrees(rotation.z);
	}

	void Transform::SetPosition(const Vector3& _newPos, Space relativeTo)
	{
		if (relativeTo == Space::WORLD)
		{
			// 월드 자체의 포지션을 변경한 경우
			worldPosition = _newPos;

			// 월드 포지션이 변경 되었으니, 부모입장에서 본 로컬 포지션도 변경 되었다.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentPos = Vector3::Zero;

			// 부모가 있으면
			if (_parent != nullptr)
			{
				_parentPos = _parent->GetComponent<Transform>()->GetWorldPosition();
			}

			// 현재 월드의 위치에서 부모의 월드 위치를 빼준게, 자신의 로컬 위치가 된다.
			Vector3 _newLocalPos = _newPos - _parentPos;

			localPosition = _newLocalPos;
		}
		else
		{
			localPosition = _newPos;
		}

		// 로컬 변경후 update
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::SetRotation(const Vector3& _eulerAngle, Space relativeTo)
	{
		// Position과 마찬가지로 로컬과 월드에 따른 연산을 해준다.
		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eulerAngle;

			// 월드를 바꾼 뒤 로컬을 바꿔준다.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentEulerAngle = Vector3::Zero;

			// 부모 여부 확인
			if (_parent != nullptr)
			{
				_parentEulerAngle = _parent->GetComponent<Transform>()->GetWorldRotation();
			}

			Vector3 _newLocalRot = _eulerAngle - _parentEulerAngle;

			localRotation = _newLocalRot;
		}
		else
		{
			localRotation = _eulerAngle;
		}

		// 로컬의 특정 스케일, 회전값, 위치가 변경되면 바로 TM을 변경해준다.
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::SetRotation(const Quaternion& _quat, Space relativeTo /*= Space::WORLD*/)
	{
		Vector3 _eular = QuatToEuler(_quat);

		if (relativeTo == Space::WORLD)
		{
			worldRotation = _eular;

			// 월드를 바꾼 뒤 로컬을 바꿔준다.
			std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

			Vector3 _parentEulerAngle = Vector3::Zero;

			// 부모 여부 확인
			if (_parent != nullptr)
			{
				_parentEulerAngle = _parent->GetComponent<Transform>()->GetWorldRotation();
			}

			Vector3 _newLocalRot = _eular - _parentEulerAngle;

			localRotation = _newLocalRot;
		}
		else
		{
			localRotation = _eular;
		}

		// 로컬의 특정 스케일, 회전값, 위치가 변경되면 바로 TM을 변경해준다.
		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::AddRotation(const Vector3& _eulerAngle, Space relativeTo)
	{
		this->SetRotation(this->GetLocalRotation() + _eulerAngle);
	}

	void Transform::SetScale(const Vector3& _newLocalScale)
	{
		localScale = _newLocalScale;

		UpdateLocalTM();
	}

	const Matrix& Transform::GetLocalTM()
	{
		return localTM;
	}

	const Matrix& Transform::GetWorldTM()
	{
		// TM Update가 체크 되어있으면.. 업데이트 하고 반환해준다.
		if (needTMUpdate)
		{
			UpdateWorldTM();
		}

		return worldTM;
	}

	void Transform::SetLocalTM(const Matrix& _localTM)
	{
		localTM = _localTM;

		// 로컬을 변경한 후 각각의 크기 각도 위치를 Transpose해줍니다
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		// 로컬이 변경되었으니.. TMUpdate가 필요하다고 체크 해줍니다.
		CheckingNeedTMUpdate();
	}

	void Transform::SetWorldTM(const Matrix& _worldTM)
	{
		worldTM = _worldTM;

		// 월드가 바뀌었으니.. 로컬 값을 변경해줍니다.
			// 로컬은 부모와 연관되어 있기 때문에 부모를 찾아줍니다.
		std::shared_ptr<GameObject> _parent = GetGameObject()->GetParent();

		if (_parent != nullptr)
		{
			Matrix _parentWorld = _parent->GetComponent<Transform>()->GetWorldTM();

			// 부모의 월드 역행렬을 자신의 월드에 곱해주면 로컬이 나옵니다.
				// => 자신의 Local * 부모의 World = 자신의 World
				// => 자신의 Local == 자신의 World * 부모의 World의 역행렬
			_parentWorld.Invert(_parentWorld);
			localTM = worldTM * _parentWorld;
		}
		else
		{
			localTM = worldTM;
		}

		// 각각을 Transpose해줍니다.
		DecomposeMatrix(worldScale, worldRotation, worldPosition, worldTM);
		DecomposeMatrix(localScale, localRotation, localPosition, localTM);

		CheckingNeedTMUpdate();
	}

	void Transform::Rotate(const Vector3& rot)
	{
		localRotation += rot;

		UpdateLocalTM();
		UpdateWorldTM();
	}

	void Transform::TransLate(const Vector3& pos)
	{
		localPosition += pos;

		UpdateLocalTM();
		UpdateWorldTM();
	}

	Vector4 Transform::EulerToQuat(Vector3 euler)
	{
		Vector4 result;

		float x = XMConvertToRadians(euler.x) * 0.5f;
		float y = XMConvertToRadians(euler.y) * 0.5f;
		float z = XMConvertToRadians(euler.z) * 0.5f;

		float sinX = sinf(x);
		float cosX = cosf(x);
		float sinY = sinf(y);
		float cosY = cosf(y);
		float sinZ = sinf(z);
		float cosZ = cosf(z);

		result.x = (cosY * sinX * cosZ) + (sinY * cosX * sinZ);
		result.y = (sinY * cosX * cosZ) - (cosY * sinX * sinZ);
		result.z = (cosY * cosX * sinZ) - (sinY * sinX * cosZ);
		result.w = (cosY * cosX * cosZ) + (sinY * sinX * sinZ);

		return result;
	}

	Vector3 Transform::QuatToEuler(Quaternion value)
	{
		Vector3 rotation;

		double x = value.x;
		double y = value.y;
		double z = value.z;
		double w = value.w;
		double sqx = x * x;
		double sqy = y * y;
		double sqz = z * z;
		double sqw = w * w;
		float exceptCheck = 2.0f * (w * x - y * z);
		float eulerX = 0.f;

		if (abs(exceptCheck) >= 1.f)
			eulerX = copysign(3.141592f / 2, exceptCheck);
		else
			eulerX = asin(2.0f * (w * x - y * z));

		float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
		float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
		rotation = Vector3{ eulerX, eulerY, eulerZ };

		rotation = Vector3{ XMConvertToDegrees(rotation.x),
					   XMConvertToDegrees(rotation.y),
					   XMConvertToDegrees(rotation.z) };

		return rotation;
	}
}