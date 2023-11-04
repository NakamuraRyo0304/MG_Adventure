/*
 *	@File	FactoryManager.h
 *	@Brief	�t�@�N�g���[�̊Ǘ��N���X�B
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FACTORYMANAGER
#define FACTORYMANAGER

//-------------------------------------------------------------------------------------//
// �t�@�N�g���[�̃C���N���[�h
#include "Libraries/Factories/ModelFactory.h"
#include "Libraries/Factories/ShaderFactory.h"

//-------------------------------------------------------------------------------------//

class FactoryManager
{
private:

	// ���f���t�@�N�g��
	std::unique_ptr<ModelFactory> m_modelFactory;

	// �V�F�[�_�[�t�@�N�g��
	std::unique_ptr<ShaderFactory> m_shaderFactory;

public:
	FactoryManager();
	~FactoryManager();

	// �����쐬�֐�
	void CreateFactory();

	// ���f���t�@�N�g��������
	void BuildModelFactory();
	// ���f���t�@�N�g����K��
	const std::unique_ptr<ModelFactory>& VisitModelFactory();
	// ���f���t�@�N�g����ޏo
	void LeaveModelFactory();
	// �V�F�[�_�[�t�@�N�g��������
	void BuildShaderFactory();
	// �V�F�[�_�[�t�@�N�g����K��
	const std::unique_ptr<ShaderFactory>& VisitShaderFactory();
	// �V�F�[�_�[�t�@�N�g����ޏo
	void LeaveShaderFactory();

};

#endif // FACTORYMANAGER