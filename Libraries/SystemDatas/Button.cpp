/*
 *	@File	Button.cpp
 *	@Brief	�{�^���N���X�B
 *	@Date	2023-11-02
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Button.h"

// �R���X�g���N�^
Button::Button()
    : m_buttons{}           // �{�^��
    , m_batch{}             // �X�v���C�g�o�b�`
{
	// �X�v���C�g�o�b�`���쐬
	m_batch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

// �f�X�g���N�^
Button::~Button()
{
	m_batch.reset();
}

// �{�^���̍쐬
void Button::CreateButton(const wchar_t* key, const wchar_t* imagePath)
{
    ButtonData buttonData;
    LoadTexture(imagePath, buttonData.texture);  // �摜��ǂݍ���Ńe�N�X�`����ݒ�

    // �����p�����[�^�[��ݒ�i�f�t�H���g�l�j
    buttonData.parameters.position = SimpleMath::Vector2(0.0f, 0.0f);
    buttonData.parameters.scale = SimpleMath::Vector2(1.0f, 1.0f);
    buttonData.parameters.rotation = 0.0f;
    buttonData.parameters.origin = SimpleMath::Vector2(0.0f, 0.0f);
    buttonData.parameters.alpha = 1.0f;

    m_buttons[key] = buttonData;  // �}�b�v�Ƀ{�^���f�[�^��o�^
}

// ���W���Z�b�g
void Button::SetPosition(const wchar_t* key, const SimpleMath::Vector2& pos)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.position = pos;
    }
}

// �g�嗦���Z�b�g
void Button::SetScale(const wchar_t* key, const SimpleMath::Vector2& scale)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.scale = scale;
    }
}

// ��]�����Z�b�g
void Button::SetRotation(const wchar_t* key, const float& angle)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.rotation = angle;
    }
}

// ���S���W���Z�b�g
void Button::SetOrigin(const wchar_t* key, const SimpleMath::Vector2& origin)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.origin = origin;
    }
}

// �����x���Z�b�g
void Button::SetAlpha(const wchar_t* key, const float& alpha)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.alpha = alpha;
    }
}

// �`�揈��
void Button::Draw(const wchar_t* key)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        // �p�����[�^�[��K�p���ă{�^����`��
        m_batch->Begin();
        m_batch->Draw(m_buttons[key].texture.Get(),
            m_buttons[key].parameters.position,
            nullptr,
            {1.f,1.f,1.f,m_buttons[key].parameters.alpha},
            m_buttons[key].parameters.rotation,
            m_buttons[key].parameters.origin,
            m_buttons[key].parameters.scale,
            SpriteEffects_None, 0.0f);
        m_batch->End();
    }
}

// �摜�ǂݍ���
void Button::LoadTexture(const wchar_t* imagePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
    DX::DeviceResources* deviceResources = DX::DeviceResources::GetInstance();
    ID3D11Device* device = deviceResources->GetD3DDevice();
    ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

    // �摜�t�@�C����ǂݍ���Ńe�N�X�`���𐶐�
    CreateWICTextureFromFile(device, context, imagePath, nullptr, texture.ReleaseAndGetAddressOf());
}