/*
 *	@File	Fade.h
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FADE
#define FADE

class Fade
{
private:
	// �X�v���C�g�o�b�`�ւ̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �摜���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

	// �t�F�[�h�C�����J�n����t���O
	bool is_fadeInFlag;

	// �t�F�[�h�A�E�g���J�n����t���O
	bool is_fadeOutFlag;

	// �t�F�[�h�I������t���O
	bool is_endFlag;

	// �t�F�[�h�̐��l0-255
	float m_fadeNum;

	// �ő�l/�ŏ��l
	const float MAX_NUM = 255.0f;
	const float MIN_NUM = 0.0f;

public:
	Fade();
	~Fade();

	// ����������
	void Initialize(ID3D11DeviceContext1* context,ID3D11Device1* device);

	// �X�V����
	void Update(float speed);

	// �`�揈��
	void Draw();

	// �t�F�[�h�C�����J�n����֐�
	void SetFadeIn();

	// �t�F�[�h�A�E�g���J�n����֐�
	void SetFadeOut();

	// �t�F�[�h�A�E�g�̒l��Ԃ�
	const float& GetMaxValue() { return MAX_NUM; }
	
	// �l�̃��Z�b�g��������֐�
	void Reset();
		
	// �t�F�[�h�I���𔻒肷��֐�
	const bool& GetEndFlag() { return is_endFlag; }

	// �t�F�[�h�̐��l�̎擾�֐�
	const float& GetFadeNum() { return m_fadeNum; }

};

#endif // FADE