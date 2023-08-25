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
	MouseCursor(ID3D11DeviceContext1* context);
	~MouseCursor();

	void Initialize(const wchar_t* filename, ID3D11Device* device);
	void Update(DirectX::SimpleMath::Vector2 pos) { m_cursorPos = pos; }
	void Render();
	void Finalize();
};

#endif // MOUSECURSOR