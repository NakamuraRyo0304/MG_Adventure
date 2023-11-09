/*
 *	@File	MouseCursor.cpp
 *	@Brief	�}�E�X�J�[�\���̕ύX�B
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MouseCursor.h"


// �R���X�g���N�^
MouseCursor::MouseCursor()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

// �f�X�g���N�^
MouseCursor::~MouseCursor()
{
	Finalize();
}

// ����������
void MouseCursor::Initialize(const wchar_t* filename)
{
	// �摜�̓o�^
	CreateDDSTextureFromFile(
		DX::DeviceResources::GetInstance()->GetD3DDevice(),
		filename,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
}

// �}�E�X�ʒu�̍X�V
void MouseCursor::Update()
{
	auto _mouse = Mouse::Get().GetState();
	m_cursorPos = SimpleMath::Vector2{ static_cast<float>(_mouse.x),static_cast<float>(_mouse.y) };
}

// �`�揈��
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

// �I������
void MouseCursor::Finalize()
{
	m_SRV.Reset();
}
