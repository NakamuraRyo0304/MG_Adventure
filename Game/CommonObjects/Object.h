/*
 *	@File	Object.h
 *	@Brief	�I�u�W�F�N�g�w�b�_�[�B
 *	@Date	2024-01-23
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef OBJECT
#define OBJECT

// SimpleMath::Vector3�Ŏg�p
#include "SimpleMath.h"

// �}�b�v�u���b�N�̍\����
class Object
{
private:

	int m_id;									// �}�b�vID
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	bool is_hit;								// �������Ă�����True
	int m_idx;									// �C���f�b�N�X�ԍ����i�[

public:

	Object()
		: m_id{ 0 }
		, m_position{ DirectX::SimpleMath::Vector3::Zero }
		, is_hit{ false }
		, m_idx{ 0 }
	{
	}
	// ���Z�q�̃I�[�o�[���[�h
	bool operator==(const Object& name) const
	{
		return m_id == name.m_id && m_position == name.m_position;
	}

	int& GetID() { return m_id; }
	void SetID(const int& id) { m_id = id; }

	DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }

	bool& IsHit() { return is_hit; }
	void SetHit(const bool& hit) { is_hit = hit; }

	int GetIndex()& { return m_idx; }
	void SetIndex(const int& index) { m_idx = index; }
};

#endif // OBJECT