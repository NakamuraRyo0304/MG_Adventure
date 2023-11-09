/*
 *	@File	FactoryManager.cpp
 *	@Brief	�t�@�N�g���[�̊Ǘ��N���X�B
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "FactoryManager.h"

// �R���X�g���N�^
FactoryManager::FactoryManager()
{
}

// �f�X�g���N�^
FactoryManager::~FactoryManager()
{
	m_modelFactory.reset();
	m_shaderFactory.reset();
}

// �����쐬�֐�
void FactoryManager::CreateFactory()
{
	m_modelFactory = std::make_unique<ModelFactory>();
	m_shaderFactory = std::make_unique<ShaderFactory>();
}

// ���f���t�@�N�g��������
void FactoryManager::BuildModelFactory()
{
	m_modelFactory = std::make_unique<ModelFactory>();
}

// ���f���t�@�N�g����K��
const std::unique_ptr<ModelFactory>& FactoryManager::VisitModelFactory()
{
	if (not m_modelFactory)
	{
		m_modelFactory = std::make_unique<ModelFactory>();
	}
	return m_modelFactory;
}

// ���f���t�@�N�g����ޏo
void FactoryManager::LeaveModelFactory()
{
	if (not m_modelFactory) return;

	m_modelFactory.reset();
}

// �V�F�[�_�[�t�@�N�g��������
void FactoryManager::BuildShaderFactory()
{
	m_shaderFactory = std::make_unique<ShaderFactory>();
}

// �V�F�[�_�[�t�@�N�g����K��
const std::unique_ptr<ShaderFactory>& FactoryManager::VisitShaderFactory()
{
	if (not m_shaderFactory)
	{
		m_shaderFactory = std::make_unique<ShaderFactory>();
	}
	return m_shaderFactory;
}

// �V�F�[�_�[�t�@�N�g����ޏo
void FactoryManager::LeaveShaderFactory()
{
	if (not m_shaderFactory) return;

	m_shaderFactory.reset();
}
