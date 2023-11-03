/*
 *	@File	Button.h
 *	@Brief	�{�^���N���X�B
 *	@Date	2023-11-02
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef BUTTON
#define BUTTON

// �{�^���̏ڍאݒ�
struct ButtonParameters
{
    DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero;  // �\���ʒu
    DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2::Zero;     // �g�嗦
    float rotation = 0.0f;                                                       // ��]��
    DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2::Zero;    // ���S�ʒu
    float alpha = 0.0f;                                                          // �����x
};

class Button
{
private:

    // �{�^���f�[�^[�e�N�X�`���f�[�^�ƃp�����[�^���]
    struct ButtonData { Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture; ButtonParameters parameters; };

    // �{�^��
    std::map<const wchar_t*, ButtonData> m_buttons;

    // �X�v���C�g�o�b�`
    std::unique_ptr<DirectX::SpriteBatch> m_batch;
public:

    Button();
    ~Button();

    /// <summary>
    /// �{�^���̍쐬
    /// </summary>
    /// <param name="key">�o�^�L�[</param>
    /// <param name="imagePath">�摜�p�X</param>
    /// <returns>�Ȃ�</returns>
    void CreateButton(const wchar_t* key, const wchar_t* imagePath);

    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void SetPosition(const wchar_t* key, const DirectX::SimpleMath::Vector2& pos);
    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void SetScale(const wchar_t* key, const DirectX::SimpleMath::Vector2& scale);
    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void SetRotation(const wchar_t* key, const float& angle);
    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void SetOrigin(const wchar_t* key, const DirectX::SimpleMath::Vector2& origin);
    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void SetAlpha(const wchar_t* key, const float& alpha);

    /// <summary>
    ///
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void Draw(const wchar_t* key);

private:
    /// <summary>
    /// �摜�ǂݍ���
    /// </summary>
    /// <param name="imagePath">�摜�p�X</param>
    /// <param name="texture">�V�F�[�_�[���\�[�X�r���[</param>
    /// <returns>�Ȃ�</returns>
    void LoadTexture(const wchar_t* imagePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);
};

#endif // BUTTON