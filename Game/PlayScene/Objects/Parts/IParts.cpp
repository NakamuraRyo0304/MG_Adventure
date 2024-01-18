/*
 *	@File	IParts.cpp
 *	@Brief	�p�[�c�̊��N���X�B
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "IParts.h"

// �R���X�g���N�^
IParts::IParts(std::unique_ptr<DirectX::Model> model)
	: m_matrix{}		// �}�g���N�X
	, m_parentMatrix{}  // �e�}�g���N�X
{
	m_model = std::move(model);
}