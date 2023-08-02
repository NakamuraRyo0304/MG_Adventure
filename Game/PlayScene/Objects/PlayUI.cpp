/*
 *	@File	PlayUI.cpp
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e���}�l�[�W��
#include "../../../Libraries/SystemManager/SystemManager.h"

#include "PlayUI.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
PlayUI::PlayUI(const DirectX::SimpleMath::Vector2& windowSize):
	m_system{},
	m_windowSize{ windowSize },
	m_timeLimit{0},
    m_oneSecPos{SimpleMath::Vector2::Zero},
    m_tenSecPos{SimpleMath::Vector2::Zero},
	is_effectFlag{false}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayUI::~PlayUI()
{
	Finalize();
	m_system.reset();
}

/// <summary>
/// �쐬����
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Create(std::shared_ptr<SystemManager> system ,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �V�X�e��
	m_system = system;

	// �摜�̐ݒ�
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// �摜��o�^
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Death", L"Resources/Textures/DeathEffect.dds", device);

	// ��ʃT�C�Y�̊i�[
	float width =
		static_cast<float>(m_system->GetDeviceResources()->GetOutputSize().right);

	// �䗦���v�Z
	float span = static_cast<float>(width) / FULL_SCREEN_SIZE.x;

	// �X�v���C�g�̈ʒu���v�Z
	m_oneSecPos = { 1010.0f * span, 80.0f * span };
	m_tenSecPos = {  910.0f * span, 80.0f * span };

    // ���W�␳ FIXED
    if (static_cast<int>(m_windowSize.x) == 1280)
    {
        m_oneSecPos.x -= 50.0f * span;
        m_tenSecPos.x -= 50.0f * span;
    }

	// �����t���O��؂�
	is_effectFlag = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timelimit">�c�莞��</param>
/// <returns>�Ȃ�</returns>
void PlayUI::Update(const float& timelimit)
{
	m_timeLimit = static_cast<int>(timelimit);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Render()
{
    // �䗦���v�Z
    float scale = static_cast<float>(m_windowSize.x / FULL_SCREEN_SIZE.x);

    // �b�����v�Z
    int oneSec = m_timeLimit / 60;

    // �����̕��ƍ���
    const int digitWidth = 100;
    const int digitHeight = 100;

    // �ꌅ�ڂ̐�����\��
    RenderDigit(oneSec % 10, m_oneSecPos, scale, digitWidth, digitHeight);

    // �\�̌��̐�����\��
    RenderDigit((oneSec / 10) % 10, m_tenSecPos, scale, digitWidth, digitHeight);

	// �����G�t�F�N�g
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // �o�^�L�[
			SimpleMath::Vector2::Zero,         // ���W
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
			scale,                             // �g�嗦
			SimpleMath::Vector2::Zero          // ���S�ʒu
		);
	}
}

/// <summary>
/// �J�n�̃J�E���g�_�E��
/// </summary>
/// <param name="countDown">�J�E���g�_�E��</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderCountDown(const float& countDown)
{
	countDown;

}

/// <summary>
/// ������`�悷��
/// </summary>
/// <param name="digit">�`�悷�鐔��</param>
/// <param name="position">���W</param>
/// <param name="scale">�g�嗦</param>
/// <param name="digitWidth">�����̕�</param>
/// <param name="digitHeight">�����̍���</param>
/// <returns>�Ȃ�</returns>
void PlayUI::RenderDigit(int digit, const SimpleMath::Vector2& position, float scale, int digitWidth, int digitHeight)
{
	// �X�v���C�g�̈ʒu���v�Z
	float spritePosX = position.x * scale;
	float spritePosY = position.y * scale;

	// �X�v���C�g�̒��S�ʒu���v�Z
	SimpleMath::Vector2 center = { spritePosX * scale / 2.0f, spritePosY * scale / 2.0f };

	// �؂���ʒu�̐ݒ�
	RECT_U rect;

	// �؂���J�n�ʒu��ݒ�(��)
	rect.left = digit * digitWidth;

	// �؂���I���ʒu��ݒ�(��)
	rect.right = rect.left + digitWidth;

	// �摜�c����ݒ�
	rect.bottom = digitHeight;

	// �����\��
	m_system->GetDrawSprite()->DrawTexture(
		L"Number",                         // �o�^�L�[
		position + center,                 // ���W
		{ 1.0f, 1.0f, 1.0f, 1.0f },        // �F
		scale,                             // �g�嗦
		center,                            // ���S�ʒu
		rect                               // �؂���ʒu
	);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayUI::Finalize()
{
}
