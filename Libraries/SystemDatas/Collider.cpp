/*
 *	@File	Collider.cpp
 *	@Brief	�����蔻��N���X
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Collider.h"

// �R���X�g���N�^
Collider::BoxCollider::BoxCollider()
	: m_hitFace{ HIT_FACE::NONE }		// �������Ă����
	, is_hitFlag{ false }				// ����t���O
	, is_pushMode{ false }				// �����߂��L���t���O
{
}

// �����߂�����
void Collider::BoxCollider::Judgement(SimpleMath::Vector3* moveObj, const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1, const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����Ă���ʒu��������
	m_hitFace = HIT_FACE::NONE;

	// ���S�ʒu���v�Z
	SimpleMath::Vector3 _sizeCenter1 = sz1 / 2, _sizeCenter2 = sz2 / 2;

	// �����蔻������
	if (not (moveObj->x - _sizeCenter1.x < constObj.x + _sizeCenter2.x &&
			 moveObj->x + _sizeCenter1.x > constObj.x - _sizeCenter2.x &&
			 moveObj->z - _sizeCenter1.z < constObj.z + _sizeCenter2.z &&
			 moveObj->z + _sizeCenter1.z > constObj.z - _sizeCenter2.z &&
			 moveObj->y - _sizeCenter1.y < constObj.y + _sizeCenter2.y &&
			 moveObj->y + _sizeCenter1.y > constObj.y - _sizeCenter2.y)) return;

	// �����Ă���
	is_hitFlag = true;

	// ���g�̕��ƍ������v�Z
	float _lengthX = (moveObj->x + _sizeCenter1.x) - (moveObj->x - _sizeCenter1.x);
	float _lengthY = (moveObj->y + _sizeCenter1.y) - (moveObj->y - _sizeCenter1.y);
	float _lengthZ = (moveObj->z + _sizeCenter1.z) - (moveObj->z - _sizeCenter1.z);

	// �e�����̂߂荞�݋
	float _leftRatio  = ((moveObj->x + _sizeCenter1.x) - (constObj.x - _sizeCenter2.x)) / _lengthX;
	float _rightRatio = ((constObj.x + _sizeCenter2.x) - (moveObj->x - _sizeCenter1.x)) / _lengthX;

	float _upRatio    = ((moveObj->y + _sizeCenter1.y) - (constObj.y - _sizeCenter2.y)) / _lengthY;
	float _downRatio  = ((constObj.y + _sizeCenter2.y) - (moveObj->y - _sizeCenter1.y)) / _lengthY;

	float _frontRatio = ((moveObj->z + _sizeCenter1.z) - (constObj.z - _sizeCenter2.z)) / _lengthZ;
	float _backRatio  = ((constObj.z + _sizeCenter2.z) - (moveObj->z - _sizeCenter1.z)) / _lengthZ;

	// �ő�l���Z�o
	float _maxRatio = std::max({ _leftRatio ,_rightRatio ,_upRatio ,_downRatio ,_frontRatio ,_backRatio });

	// �����Ă���ʒu���i�[����
	// �������ʒu: �Y���ʒu or �ω��Ȃ�
	m_hitFace = _maxRatio == _leftRatio  ? HIT_FACE::LEFT  : m_hitFace;
	m_hitFace = _maxRatio == _rightRatio ? HIT_FACE::RIGHT : m_hitFace;
	m_hitFace = _maxRatio == _upRatio    ? HIT_FACE::UP    : m_hitFace;
	m_hitFace = _maxRatio == _downRatio  ? HIT_FACE::DOWN  : m_hitFace;
	m_hitFace = _maxRatio == _frontRatio ? HIT_FACE::FRONT : m_hitFace;
	m_hitFace = _maxRatio == _backRatio  ? HIT_FACE::BACK  : m_hitFace;

	// �����߂����������Ȃ��ꍇ���^�[��
	if (not is_pushMode) return;

	// �����Ă���ʒu���i�[���A�����߂��������s��
	// �������ʒu: �Y���ʒu or �ω��Ȃ�
	(*moveObj).x = _maxRatio == _leftRatio  ? constObj.x + (sz1.x + sz2.x) / 2 : (*moveObj).x;
	(*moveObj).x = _maxRatio == _rightRatio ? constObj.x - (sz1.x + sz2.x) / 2 : (*moveObj).x;
	(*moveObj).y = _maxRatio == _upRatio    ? constObj.y + (sz1.y + sz2.y) / 2 : (*moveObj).y;
	(*moveObj).y = _maxRatio == _downRatio  ? constObj.y - (sz1.y + sz2.y) / 2 : (*moveObj).y;
	(*moveObj).z = _maxRatio == _frontRatio ? constObj.z + (sz1.z + sz2.z) / 2 : (*moveObj).z;
	(*moveObj).z = _maxRatio == _backRatio  ? constObj.z - (sz1.z + sz2.z) / 2 : (*moveObj).z;
}

// �q�b�g����̂ݕԂ�
void Collider::BoxCollider::Judgement(const SimpleMath::Vector3& moveObj, const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1, const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����蔻������
	if (not (moveObj.x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		     moveObj.x + sz1.x / 2 > constObj.x - sz2.x / 2 &&
		     moveObj.z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		     moveObj.z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		     moveObj.y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		     moveObj.y + sz1.y / 2 > constObj.y - sz2.y / 2)) return;

	// �����Ă���
	is_hitFlag = true;
}

// �R���X�g���N�^
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

// ���̓����蔻��
void Collider::SphereCollider::PushSphere(SimpleMath::Vector3& pos1,
	                                      SimpleMath::Vector3& pos2,
	                                      float radius1, float radius2)
{
	// ��̋��̋������v�Z
	float _distance = SimpleMath::Vector3::Distance(pos1, pos2);

	// ��̋��̔��a�̘a���v�Z
	float _sumOfRadius = radius1 + radius2;

	// ��̋��̔��a�̘a������_�Ԃ̋������Z�������瓖���Ă��锻��ɂ���
	if (_distance <= _sumOfRadius)
	{
		// �Փ˂������̕����x�N�g�����v�Z
		SimpleMath::Vector3 _collisionDirection = pos2 - pos1;

		// ���K������
		_collisionDirection.Normalize();

		// �Փ˂������̉����߂��������v�Z
		float _pushDistance = _sumOfRadius - _distance;

		if (is_pushMode)
		{
			// ��1�������߂�
			pos1 -= _collisionDirection * _pushDistance / 2;

			// ��2�������߂�
			pos2 += _collisionDirection * _pushDistance / 2;
		}

		is_hitFlag = true;
	}

	is_hitFlag = false;
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Collider::AABBCollider::AABBCollider()
{
}

/// <summary>
/// ���ʂ�AABB�����蔻��
/// </summary>
/// <param name="pos1">���W</param>
/// <param name="pos2">���W</param>
/// <param name="sz1">�傫��</param>
/// <param name="sz2">�傫��</param>
/// <returns>�������Ă�����True</returns>
bool Collider::AABBCollider::IsHitAABB2D(const SimpleMath::Vector2& pos1,
										 const SimpleMath::Vector2& pos2,
										 const SimpleMath::Vector2& sz1,
										 const SimpleMath::Vector2& sz2)
{
	// XZ���ʂɂ����铖���蔻��
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.y - sz1.y / 2 < pos2.y + sz2.y / 2 &&
		pos1.y + sz1.y / 2 > pos2.y - sz2.y / 2) return true;

	return false;
}

