/*
 *	@File	MouseCursor.cpp
 *	@Brief	�}�E�X�J�[�\���̕ύX�B
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MouseCursor.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="context">�R���e�L�X�g�̃|�C���^</param>
 /// <returns>�Ȃ�</returns>
MouseCursor::MouseCursor(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
MouseCursor::~MouseCursor()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="filename">�}�E�X�|�C���^�̉摜�p�X</param>
/// <param name="device">�f�o�C�X�̃|�C���^</param>
/// <returns>�Ȃ�</returns>
void MouseCursor::Initialize(const wchar_t* filename, ID3D11Device* device)
{
	// �摜�̓o�^
	CreateDDSTextureFromFile(
		device,
		filename,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void MouseCursor::Render()
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_SRV.Get(),
		m_cursorPos,				// �\������ʒu
		nullptr,					// �؂���ʒu
		Colors::White,				// �`��F
		0.0f,						// ��]
		SimpleMath::Vector2{128.0f},// �摜�̌��_
		0.5f,						// �g�嗦
		SpriteEffects_None, 0.0f	// �`�惌�C���[
	);

	m_spriteBatch->End();
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void MouseCursor::Finalize()
{
	m_SRV.Reset();
}
