#include "Transform.h"

#include "GameObject\GameObejct.h"


namespace TL_Math
{
	const Vector2 Vector2::Zero = { 0.f, 0.f };
	const Vector2 Vector2::One = { 1.f, 1.f };
	const Vector2 Vector2::UnitX = { 1.f, 0.f };
	const Vector2 Vector2::UnitY = { 0.f, 1.f };

	const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
	const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
	const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
	const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
	const Vector3 Vector3::Forward = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::Backward = { 0.f, 0.f, 1.f };

	const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
	const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
	const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
	const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

	const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,
									  0.f, 1.f, 0.f, 0.f,
									  0.f, 0.f, 1.f, 0.f,
									  0.f, 0.f, 0.f, 1.f };

	const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
}

Transform::Transform(GameObject* _gameObject, const std::wstring& _typeName) :
	m_LocalTM(Matrix::Identity)
	, m_WorldTM(Matrix::Identity)
	, m_bWorldTMShouldUpdate(true)
	, m_GameObject(_gameObject)
{

}


void Transform::UpdateWorldMatrix(const Matrix& _parentTM)
{
	m_WorldTM = GetLocalTM() * _parentTM;

	m_bWorldTMShouldUpdate = false;
}

void Transform::ChangeParent(Transform* _parent)
{
	assert(_parent != nullptr);

	if (_parent == nullptr)
	{
		m_LocalTM = m_WorldTM;
	}
	else
	{
		auto _parentInv = _parent->GetWorldTM().Invert();

		m_LocalTM = m_WorldTM * _parentInv;
	}
}

void Transform::SetLocalPosition(const Vector3& _newPosition)
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetLocalTM().Decompose(_scale, _rotation, _position);

	SetLocalTM(
		Matrix::CreateScale(_scale)
		* Matrix::CreateFromQuaternion(_rotation)
		* Matrix::CreateTranslation(_newPosition)
	);
}

void Transform::SetLocalRotation(const Quaternion& _newRotation)
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetLocalTM().Decompose(_scale, _rotation, _position);

	SetLocalTM(
		Matrix::CreateScale(_scale)
		* Matrix::CreateFromQuaternion(_newRotation)
		* Matrix::CreateTranslation(_position)
	);
}

void Transform::SetLocalRotation(const Vector3& _euler)
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetLocalTM().Decompose(_scale, _rotation, _position);

	SetLocalTM(
		Matrix::CreateScale(_scale)
		* Matrix::CreateFromYawPitchRoll(_euler)
		* Matrix::CreateTranslation(_position)
	);
}

void Transform::SetLocalScale(const Vector3& _newScale)
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetLocalTM().Decompose(_scale, _rotation, _position);

	SetLocalTM(
		Matrix::CreateScale(_newScale)
		* Matrix::CreateFromQuaternion(_rotation)
		* Matrix::CreateTranslation(_position)
	);
}

void Transform::SetLocalPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation)
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetLocalTM().Decompose(_scale, _rotation, _position);

	SetLocalTM(
		Matrix::CreateScale(_scale)
		* Matrix::CreateFromQuaternion(_newRotation)
		* Matrix::CreateTranslation(_newPosition)
	);
}

void Transform::SetWorldPosition(const Vector3& _newPosition)
{
	if (!m_GameObject) { SetLocalPosition(_newPosition); return; }

	const auto _parent = m_GameObject->GetParent();
	if (_parent == nullptr)
	{
		SetLocalPosition(_newPosition);
	}
	else
	{
		auto _newWorldTM = GetWorldTM();

		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;
		_newWorldTM.Decompose(_scale, _rotation, _position);

		_newWorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_rotation) * Matrix::CreateTranslation(_newPosition);

		const auto _parentWorldTMInvert = m_GameObject->GetParent()->m_Transform.GetWorldTM().Invert();
		auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
		SetLocalTM(_newLocalTM);
	}
}

//todo : 이거 쓰는 함수 다 고쳐야함
void Transform::SetWorldRotation(const Quaternion& _newRotation)
{
	if (!m_GameObject) { SetLocalRotation(_newRotation); return; }
	const auto _parent = m_GameObject->GetParent();
	if (_parent == nullptr)
	{
		SetLocalRotation(_newRotation);
	}
	else
	{
		auto _newWorldTM = GetWorldTM();

		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;
		_newWorldTM.Decompose(_scale, _rotation, _position);

		_newWorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_newRotation) * Matrix::CreateTranslation(_position);

		const auto _parentWorldTMInvert = m_GameObject->GetParent()->m_Transform.GetWorldTM().Invert();
		auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
		SetLocalTM(_newLocalTM);
	}
}

void Transform::SetWorldRotation(const Vector3& _euler)
{
	if (!m_GameObject) { SetLocalRotation(_euler); return; }
	const auto _parent = m_GameObject->GetParent();
	if (_parent == nullptr)
	{
		SetLocalRotation(_euler);
	}
	else
	{
		auto _newWorldTM = GetWorldTM();

		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;
		_newWorldTM.Decompose(_scale, _rotation, _position);

		_newWorldTM = Matrix::CreateScale(_scale)
			* DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_euler))
			* Matrix::CreateTranslation(_position);

		const auto _parentWorldTMInvert = m_GameObject->GetParent()->m_Transform.GetWorldTM().Invert();
		auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
		SetLocalTM(_newLocalTM);
	}
}

void Transform::SetWorldScale(const Vector3& _newScale)
{
	if (!m_GameObject) { SetLocalScale(_newScale); return; }
	const auto _parent = m_GameObject->GetParent();
	if (_parent == nullptr)
	{
		SetLocalScale(_newScale);
	}
	else
	{
		auto _newWorldTM = GetWorldTM();

		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;
		_newWorldTM.Decompose(_scale, _rotation, _position);

		_newWorldTM = Matrix::CreateScale(_newScale) * Matrix::CreateFromQuaternion(_rotation) * Matrix::CreateTranslation(_position);

		const auto _parentWorldTMInvert = m_GameObject->GetParent()->m_Transform.GetWorldTM().Invert();
		auto _newLocalTM = _newWorldTM * _parentWorldTMInvert;
		SetLocalTM(_newLocalTM);
	}
}



void Transform::SetPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation)
{
	if (!m_GameObject) { SetLocalPositionAndRotation(_newPosition, _newRotation); return; }
	const auto _parent = m_GameObject->GetParent();
	if (_parent == nullptr)
	{
		SetLocalPositionAndRotation(_newPosition, _newRotation);
	}
	else
	{
		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;
		m_WorldTM.Decompose(_scale, _rotation, _position);

		m_WorldTM = Matrix::CreateScale(_scale) * Matrix::CreateFromQuaternion(_newRotation) * Matrix::CreateTranslation(_newPosition);

		SetWorldTM(m_WorldTM);
	}
}

Vector3 Transform::GetWorldPosition()
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetWorldTM().Decompose(_scale, _rotation, _position);

	return _position;
}

Quaternion Transform::GetWorldRotation()
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetWorldTM().Decompose(_scale, _rotation, _position);
	return _rotation;
}

Vector3 Transform::GetWorldRotationEuler()
{
	return GetWorldRotation().ToEuler();
}

Vector3 Transform::GetWorldScale()
{
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _position;
	GetWorldTM().Decompose(_scale, _rotation, _position);

	return _scale;
}

void Transform::SetLocalTM(const Matrix& _localTM)
{
	m_LocalTM = _localTM;
	MarkWorldTMDirt();
}

void Transform::SetWorldTM(const Matrix& _worldTM)
{
	if (!m_GameObject) { SetLocalTM(_worldTM); return; }
	const Matrix _localTM = m_GameObject->GetParent() == nullptr
		? _worldTM
		: _worldTM * m_GameObject->GetParent()->m_Transform.GetWorldTM().Invert();

	SetLocalTM(_localTM);
}

Matrix Transform::GetLocalTM()
{
	return m_LocalTM;
}

Matrix Transform::GetWorldTM()
{
	TryUpdateWorldTM();

	return m_WorldTM;
}

Vector3 Transform::GetRightDirection()
{
	return GetWorldTM().Right();
}

Vector3 Transform::GetUpDirection()
{
	return GetWorldTM().Up();
}

Vector3 Transform::GetForwardDirection()
{
	return GetWorldTM().Forward();
}

void Transform::TryUpdateWorldTM()
{
	if (m_bWorldTMShouldUpdate == true)
	{
		if (!m_GameObject) {UpdateWorldMatrix(Matrix::Identity); return; }
		// 부모의 WorldMatrix를 가져와 새로고칩니다.
		if (m_GameObject->GetParent() == nullptr)
			UpdateWorldMatrix(Matrix::Identity);
		else
			UpdateWorldMatrix(m_GameObject->GetParent()->m_Transform.GetWorldTM());
	}
}

void Transform::MarkWorldTMDirt()
{
	if (!m_GameObject) return;

	this->m_bWorldTMShouldUpdate = true;

	// 이 게임 오브젝트의 하위 자식 오브젝트들의 TM도 모두 바뀌었기 때문에
	// MarkWorldTMDirt를 전파합니다.
	const auto& _childs = m_GameObject->m_Childs;
	for (auto _child : _childs)
		_child->m_Transform.MarkWorldTMDirt();
}
