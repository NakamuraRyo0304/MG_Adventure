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
IScene::IScene():
	m_system{std::make_shared<SystemManager>()},
	m_nextScene{SCENE::NONE},
	is_changeFlag{false},
	m_fadeSpeed{4.0f}
{
	// �쐬����Ă��Ȃ���΃V�X�e�����쐬
	if (!m_system)
	{
		m_system = std::make_shared<SystemManager>();
	}
}