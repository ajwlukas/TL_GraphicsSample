#pragma once

#include <string>
#include "Math/TL_Math.h"

using namespace TL_Math;
class Transform
{
public:
	Transform(class GameObject* _gameObject, const std::wstring& _typeName = L"Transform");

private:
	void UpdateWorldMatrix(const Matrix& _parentTM);

	/**
	 * \brief ���� ��ȯ ����Դϴ�.
	 */
	Matrix m_LocalTM;

	/**
	 * \brief ���� ��ȯ ����Դϴ�.
	 */
	Matrix m_WorldTM;

	/**
	 * \brief ���� ���������� ���� ��ȯ ����� ���� ����
	 * ���� �� ���̶� �����Ǿ ����� �ʿ䰡 �ִ����� ���� �Ǻ����Դϴ�.
	 * �� ������ ��� ����� �ּ������� ���̱� ���� ���˴ϴ�.
	 */
	bool m_bWorldTMShouldUpdate;

public:

	/**
	 * \brief �θ� ��ȯ�� ���� ���� Ʈ�������� ���� ����� �θ���
	 * ���� Ʈ������ �������� �����մϴ�.
	 */
	void ChangeParent(Transform* _parent);

	void SetLocalPosition(const Vector3& _newPosition);

	void SetLocalRotation(const Quaternion& _newRotation);

	void SetLocalRotation(const Vector3& _euler);

	void SetLocalScale(const Vector3& _newScale);

	void SetWorldPosition(const Vector3& _newPosition);

	void SetWorldRotation(const Quaternion& _newRotation);

	void SetWorldRotation(const Vector3& _euler);

	void SetWorldScale(const Vector3& _newScale);



	/*
	 * Add for physics update
	 * Author	: LWT
	 * Date		: 2022. 10. 31
	 */
	void SetLocalPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation);
	void SetPositionAndRotation(const Vector3& _newPosition, const Quaternion& _newRotation);

	inline Vector3 GetLocalPosition()
	{
		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;

		Matrix _localTM = GetLocalTM();
		_localTM.Decompose(_scale, _rotation, _position);

		return _position;
	}

	inline Quaternion GetLocalRotation()
	{
		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;

		Matrix _localTM = GetLocalTM();
		_localTM.Decompose(_scale, _rotation, _position);

		return _rotation;
	}

	inline Vector3 GetLocalRotationEuler()
	{
		return GetLocalRotation().ToEuler();
	}

	inline Vector3 GetLocalScale()
	{
		Vector3 _scale;
		Quaternion _rotation;
		Vector3 _position;

		Matrix _localTM = GetLocalTM();
		_localTM.Decompose(_scale, _rotation, _position);

		return _scale;
	}

	Vector3 GetWorldPosition();

	Quaternion GetWorldRotation();

	Vector3 GetWorldRotationEuler();

	Vector3 GetWorldScale();

	void SetLocalTM(const Matrix& _localTM);

	void SetWorldTM(const Matrix& _worldTM);

	Matrix GetLocalTM();

	Matrix GetWorldTM();

	Vector3 GetRightDirection();

	Vector3 GetUpDirection();

	Vector3 GetForwardDirection();

private:
	void TryUpdateWorldTM();

	void MarkWorldTMDirt();

private:
	static constexpr float RAD_TO_DEG = 57.295779513f;

	static constexpr float DEG_TO_RAD = 0.01745329252f;

	friend class GameObject;

	class GameObject* m_GameObject;
};