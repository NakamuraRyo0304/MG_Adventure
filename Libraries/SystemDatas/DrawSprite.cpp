/*
 *	@File	DrawSprite.cpp
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DrawSprite.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
DrawSprite::DrawSprite()
	: m_spriteBatch{}	// �X�v���C�g�o�b�`
	, m_textures{}		// �e�N�X�`���z��
	, m_SRV{}			// �V�F�[�_�[���\�[�X�r���[
	, m_rotate{}		// �摜�̉�]��
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

/// <summary>
/// �X�v���C�g�o�b�`���쐬
/// </summary>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
/// <returns>�Ȃ�</returns>
void DrawSprite::MakeSpriteBatch(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// �摜��o�^����֐�
/// </summary>
/// <param name="key">�o�^�L�[(������w�肵�ČĂяo��)</param>
/// <param name="path">�摜�̃p�X(L"Resources/Textures/....dds)�g���q�́u.dds�v</param>
/// <param name="device">ID3D11Device�|�C���^</param>
/// <returns>�Ȃ�</returns>
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// �摜�̒ǉ�
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator _it = m_textures.begin(); _it != m_textures.end(); ++_it)
	{
		// �摜�̓o�^
		CreateDDSTextureFromFile(
			device,										// �f�o�C�X�|�C���^
			_it->second,								// �e�N�X�`���̃p�X
			nullptr,									// �����͎��ʂ��Ȃ�
			m_SRV[_it->first].ReleaseAndGetAddressOf()	// �Ή�����L�[�ԍ��ɓo�^
		);
	}

	// �������łO������
	m_rotate.emplace(key, 0.0f);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="key">�L�[</param>
/// <param name="pos">�\�����W</param>
/// <param name="color">�F</param>
/// <param name="rate">�g�嗦</param>
/// <param name="origin">���S�ʒu</param>
/// <param name="rect">�؂���ʒu</param>
/// <returns>�Ȃ�</returns>
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos,
	SimpleMath::Vector4 color, SimpleMath::Vector2 rate, SimpleMath::Vector2 origin, RECT_U rect)
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

/// <summary>
/// ��]�ʃZ�b�^�[
/// </summary>
/// <param name="key">�o�^�L�[</param>
/// <param name="rotate">��]��</param>
/// <returns>�Ȃ�</returns>
void DrawSprite::CreateRotation(const wchar_t* key, const float& rotate)
{
	// ��]�ʂ��i�[
	std::map<const wchar_t*, float>::iterator _rt = m_rotate.find(key);

	_rt->second = rotate;
}
