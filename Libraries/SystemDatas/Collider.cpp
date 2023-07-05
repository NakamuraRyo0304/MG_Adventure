/*
 *	@File	Collider.cpp
 *	@Brief	�����m�̓����蔻��B
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Collider.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
Collider::BoxCollider::BoxCollider():
	m_hitFace(),
	is_hitFlag(),
	is_pushMode()
{
	m_hitFace = HIT_FACE::NONE;
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// �����߂�����
/// </summary>
/// <param name= moveObj">�����߂������I�u�W�F</param>
/// <param name= constObj">�Ԃ���ΏۃI�u�W�F</param>
/// <param name= sz1">�傫��</param>
/// <param name= sz2">�傫��</param>
void Collider::BoxCollider::PushBox(SimpleMath::Vector3* moveObj,
			                        const SimpleMath::Vector3& constObj,
			                        const SimpleMath::Vector3& sz1, 
	                                const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����蔻������
	if (!(moveObj->x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		  moveObj->x + sz1.x / 2 > constObj.x - sz2.x / 2 && 
		  moveObj->z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		  moveObj->z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		  moveObj->y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		  moveObj->y + sz1.y / 2 > constObj.y - sz2.y / 2))return;

	// �����Ă���ʒu��������
	m_hitFace = HIT_FACE::NONE;

	// �����Ă���
	is_hitFlag = true;

	// ���g�̕��ƍ������v�Z
	float lenghtX = (moveObj->x + sz1.x / 2) - (moveObj->x - sz1.x / 2);
	float lengthY = (moveObj->y + sz1.y / 2) - (moveObj->y - sz1.y / 2);
	float lengthZ = (moveObj->z + sz1.z / 2) - (moveObj->z - sz1.z / 2);

	// �e�����̂߂荞�݋
	float leftRatio  = ((moveObj->x + sz1.x / 2) - (constObj.x - sz2.x / 2)) / lenghtX;
	float rightRatio = ((constObj.x + sz2.x / 2) - (moveObj->x - sz1.x / 2)) / lenghtX;

	float upRatio    = ((moveObj->y + sz1.y / 2) - (constObj.y - sz2.y / 2)) / lengthY;
	float downRatio  = ((constObj.y + sz2.y / 2) - (moveObj->y - sz1.y / 2)) / lengthY;

	float frontRatio = ((moveObj->z + sz1.z / 2) - (constObj.z - sz2.z / 2)) / lengthZ;
	float backRatio  = ((constObj.z + sz2.z / 2) - (moveObj->z - sz1.z / 2)) / lengthZ;

	// �ő�l���Z�o
	float maxRatio = std::max({leftRatio, rightRatio,
								upRatio,   downRatio,
								frontRatio,backRatio });

	// �����Ă���ʒu���i�[����
	if (maxRatio == leftRatio)	m_hitFace = HIT_FACE::LEFT;		// �������ʒu�F��
	if (maxRatio == rightRatio)	m_hitFace = HIT_FACE::RIGHT;	// �������ʒu�F�E
	if (maxRatio == upRatio)	m_hitFace = HIT_FACE::UP;		// �������ʒu�F��
	if (maxRatio == downRatio)	m_hitFace = HIT_FACE::DOWN;		// �������ʒu�F��
	if (maxRatio == frontRatio)	m_hitFace = HIT_FACE::FRONT;	// �������ʒu�F�O
	if (maxRatio == backRatio)	m_hitFace = HIT_FACE::BACK;		// �������ʒu�F��

	// �����߂����������Ȃ��ꍇ���^�[��
	if (!is_pushMode)return;

	// �����Ă���ʒu���i�[���A�����߂��������s��
	if (maxRatio == leftRatio)
	{
		(*moveObj).x = constObj.x + (sz1.x + sz2.x) / 2;
	}
	if (maxRatio == rightRatio)
	{
		(*moveObj).x = constObj.x - (sz1.x + sz2.x) / 2;
	}
	if (maxRatio == upRatio)
	{
		(*moveObj).y = constObj.y + (sz1.y + sz2.y) / 2;
	}
	if (maxRatio == downRatio)
	{
		(*moveObj).y = constObj.y - (sz1.y + sz2.y) / 2;
	}
	if (maxRatio == frontRatio)
	{
		(*moveObj).z = constObj.z + (sz1.z + sz2.z) / 2;
	}
	if (maxRatio == backRatio)
	{
		(*moveObj).z = constObj.z - (sz1.z + sz2.z) / 2;
	}
}

/// <summary>
/// �������Ă��邩�ǂ����̂ݔ��肷��(�ʂ��i�[�����)
/// </summary>
/// <param name= moveObj">�����߂������I�u�W�F(�����Ă������)</param>
/// <param name= constObj">�Ԃ���ΏۃI�u�W�F(��苭������)</param>
/// <param name= sz1">�傫��</param>
/// <param name= sz2">�傫��</param>
void Collider::BoxCollider::PushBox(SimpleMath::Vector3& moveObj,
	const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1,
	const SimpleMath::Vector3& sz2)
{
	// �����Ă��Ȃ�
	is_hitFlag = false;

	// �����蔻������
	if (!(moveObj.x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		moveObj.x + sz1.x / 2 > constObj.x - sz2.x / 2 &&
		moveObj.z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		moveObj.z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		moveObj.y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		moveObj.y + sz1.y / 2 > constObj.y - sz2.y / 2))return;

	// �����Ă���ʒu��������
	m_hitFace = HIT_FACE::NONE;

	// �����Ă���
	is_hitFlag = true;

	// ���g�̕��ƍ������v�Z
	float lenghtX = (moveObj.x + sz1.x / 2) - (moveObj.x - sz1.x / 2);
	float lengthY = (moveObj.y + sz1.y / 2) - (moveObj.y - sz1.y / 2);
	float lengthZ = (moveObj.z + sz1.z / 2) - (moveObj.z - sz1.z / 2);

	// �e�����̂߂荞�݋
	float leftRatio = ((moveObj.x + sz1.x / 2) - (constObj.x - sz2.x / 2)) / lenghtX;
	float rightRatio = ((constObj.x + sz2.x / 2) - (moveObj.x - sz1.x / 2)) / lenghtX;

	float upRatio = ((moveObj.y + sz1.y / 2) - (constObj.y - sz2.y / 2)) / lengthY;
	float downRatio = ((constObj.y + sz2.y / 2) - (moveObj.y - sz1.y / 2)) / lengthY;

	float frontRatio = ((moveObj.z + sz1.z / 2) - (constObj.z - sz2.z / 2)) / lengthZ;
	float backRatio = ((constObj.z + sz2.z / 2) - (moveObj.z - sz1.z / 2)) / lengthZ;

	// �ő�l���Z�o
	float maxRatio = std::max({ leftRatio, rightRatio,
								upRatio,   downRatio,
								frontRatio,backRatio });

	// �����Ă���ʒu���i�[����
	if (maxRatio == leftRatio)	m_hitFace = HIT_FACE::LEFT;		// �������ʒu�F��
	if (maxRatio == rightRatio)	m_hitFace = HIT_FACE::RIGHT;	// �������ʒu�F�E
	if (maxRatio == upRatio)	m_hitFace = HIT_FACE::UP;		// �������ʒu�F��
	if (maxRatio == downRatio)	m_hitFace = HIT_FACE::DOWN;		// �������ʒu�F��
	if (maxRatio == frontRatio)	m_hitFace = HIT_FACE::FRONT;	// �������ʒu�F�O
	if (maxRatio == backRatio)	m_hitFace = HIT_FACE::BACK;		// �������ʒu�F��
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// ���̓����蔻��
/// </summary>
/// <param name="pos1">���W</param>
/// <param name="pos2">���W</param>
/// <param name="radius1">���a</param>
/// <param name="raidus2">���a</param>
void Collider::SphereCollider::PushSphere(SimpleMath::Vector3& pos1, 
	                                      SimpleMath::Vector3& pos2, 
	                                      float radius1, float radius2)
{
	// ��̋��̋������v�Z
	float distance = SimpleMath::Vector3::Distance(pos1, pos2);

	// ��̋��̔��a�̘a���v�Z
	float sumOfRadius = radius1 + radius2;

	// ��̋��̔��a�̘a������_�Ԃ̋������Z�������瓖���Ă��锻��ɂ���
	if (distance <= sumOfRadius) 
	{
		// �Փ˂������̕����x�N�g�����v�Z
		SimpleMath::Vector3 collisionDirection = pos2 - pos1;

		// ���K������
		collisionDirection.Normalize();

		// �Փ˂������̉����߂��������v�Z
		float pushDistance = sumOfRadius - distance;

		if (is_pushMode)
		{
			// ��1�������߂�
			pos1 -= collisionDirection * pushDistance / 2;

			// ��2�������߂�
			pos2 += collisionDirection * pushDistance / 2;
		}

		is_hitFlag = true;
	}

	is_hitFlag = false;
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
Collider::AABBCollider::AABBCollider()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="pos1">���W</param>
/// <param name="pos2">���W</param>
/// <param name="sz1">�傫��</param>
/// <param name="sz2">�傫��</param>
// ��P�A�Q�����F���W ��R�A�S�����F�T�C�Y
bool Collider::AABBCollider::HitAABB_2D(const SimpleMath::Vector2& pos1, 
								     const SimpleMath::Vector2& pos2,
								     const SimpleMath::Vector2& sz1, 
								     const SimpleMath::Vector2& sz2)
{
	// XZ���ʂɂ����铖���蔻��
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.y - sz1.y / 2 < pos2.y + sz2.y / 2 &&
		pos1.y + sz1.y / 2 > pos2.y - sz2.y / 2)return true;

	return false;
}

