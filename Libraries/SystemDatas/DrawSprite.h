/*
 *	@File	DrawSprite.h
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSPRITE
#define DRAWSPRITE

// �f�t�H���g�����ݒ�p�̌p��
struct RECT_U : public RECT
{
	RECT_U(LONG left = 0, LONG top = 0, LONG right = 1920, LONG bottom = 1080)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

class DrawSprite
{
private:
	// �摜�̊i�[�z��
	std::map<const wchar_t*, const wchar_t*> m_textures;

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �摜��`�悷��ϐ�
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_SRV;

	// ��]��
	std::map <const wchar_t*,float> m_rotate;

public:
	DrawSprite();
	~DrawSprite();

	/// <summary>
	/// �X�v���C�g�o�b�`�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void MakeSpriteBatch();

	/// <summary>
	/// �摜��o�^����֐�
	/// </summary>
	/// <param name="key">�o�^�L�[(������w�肵�ČĂяo��)</param>
	/// <param name="path">�摜�̃p�X(L"Resources/Textures/....dds)�g���q�́u.dds�v</param>
	/// <returns>�Ȃ�</returns>
	void AddTextureData(const wchar_t* key, const wchar_t* path);

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
	void DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos,
						DirectX::SimpleMath::Vector4 color, DirectX::SimpleMath::Vector2 rate,
						DirectX::SimpleMath::Vector2 origin, RECT_U rect = RECT_U());


	/// <summary>
	/// ��]�ʃZ�b�^�[
	/// </summary>
	/// <param name="key">�o�^�L�[</param>
	/// <param name="rotate">��]��</param>
	/// <returns>�Ȃ�</returns>
	void CreateRotation(const wchar_t* key, const float& rotate);


	/// <summary>
	/// ��]�ʃQ�b�^�[
	/// </summary>
	/// <param name="key">�o�^�L�[</param>
	/// <returns>��]��</returns>
	const float GetRotate(const wchar_t* key) { auto it = m_rotate.find(key); return it->second; }
};

#endif // DRAWSPRITE