/*
 *	@File	MouseCursor.h
 *	@Brief	�}�E�X�J�[�\���̕ύX�B
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MOUSECURSOR
#define MOUSECURSOR

class MouseCursor
{
private:

	// �X�v���C�g�o�b�`�̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �e�N�X�`���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView > m_SRV;

	// ���W
	DirectX::SimpleMath::Vector2 m_cursorPos;

public:
	MouseCursor();
	~MouseCursor();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="filename">�}�E�X�|�C���^�̉摜�p�X</param>
	/// <returns>�Ȃ�</returns>
	void Initialize(const wchar_t* filename);

	/// <summary>
	/// �}�E�X�ʒu�̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Render();

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();
};

#endif // MOUSECURSOR