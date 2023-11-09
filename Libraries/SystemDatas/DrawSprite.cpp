/*
 *	@File	DrawSprite.cpp
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "DrawSprite.h"

// �R���X�g���N�^
DrawSprite::DrawSprite()
	: m_spriteBatch{}	// �X�v���C�g�o�b�`
	, m_textures{}		// �e�N�X�`���z��
	, m_SRV{}			// �V�F�[�_�[���\�[�X�r���[
	, m_rotate{}		// �摜�̉�]��
{
}

// �f�X�g���N�^
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

// �X�v���C�g�o�b�`���쐬
void DrawSprite::MakeSpriteBatch()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

//  �摜��o�^����֐�
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path)
{
	// �摜�̒ǉ�
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator i = m_textures.begin(); i != m_textures.end(); ++i)
	{
		// �摜�̓o�^
		CreateDDSTextureFromFile(
			DX::DeviceResources::GetInstance()->GetD3DDevice(),	// �f�o�C�X�|�C���^
			i->second,											// �e�N�X�`���̃p�X
			nullptr,											// �����͎��ʂ��Ȃ�
			m_SRV[i->first].ReleaseAndGetAddressOf()			// �Ή�����L�[�ԍ��ɓo�^
		);
	}

	// �������łO������
	m_rotate.emplace(key, 0.0f);
}

// �摜��`�悷��
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos,
	SimpleMath::Vector4 color, SimpleMath::Vector2 rate, SimpleMath::Vector2 origin, RECT rect)
{
	m_spriteBatch->Begin();

	// �`�悵�����L�[�ԍ��ɑΉ�����}�b�v���C�e���[�^�Ɋi�[
	std::map<const wchar_t*, const wchar_t*>::const_iterator _it = m_textures.find(key);

	// ��]�ʂ��i�[
	std::map<const wchar_t*, float>::const_iterator _rt = m_rotate.find(key);

	// �摜�̕`��
	m_spriteBatch->Draw(
		m_SRV[_it->first].Get(),				// �Ή�����C�e���[�^�̉摜��`��
		pos,									// �\������ʒu
		&rect,								    // �؂���ʒu
		color,									// �`��F
		m_rotate[_it->first],					// ��]
		origin,									// �摜�̌��_
		rate,									// �g�嗦
		SpriteEffects_None, 0.0f				// �`�惌�C���[
	);

	m_spriteBatch->End();
}

// ��]�ʃZ�b�^�[
void DrawSprite::CreateRotation(const wchar_t* key, const float& rotate)
{
	// ��]�ʂ��i�[
	std::map<const wchar_t*, float>::iterator _rt = m_rotate.find(key);

	_rt->second = rotate;
}
