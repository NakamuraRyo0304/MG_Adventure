/*
 *	@File	IScene.cpp
 *	@Brief	���N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
IScene::IScene()
	: m_system{std::make_shared<SystemManager>()}	// �V�X�e���}�l�[�W��
	, m_nextScene{SCENE::NONE}						// ���̃V�[��
	, m_fullScreenSize{}							// �t���X�N���[���T�C�Y
	, m_fadeSpeed{4.0f}								// �t�F�[�h�̑��x
	, m_fadeValue{0.0f}								// �t�F�[�h�̌��ݒl
	, is_changeFlag{false}							// �V�[���؂�ւ��t���O
{
	// �쐬����Ă��Ȃ���΃V�X�e�����쐬
	if (!m_system)
	{
		m_system = std::make_shared<SystemManager>();
	}
}