/*
 *	@File	DepthStencil.h
 *	@Brief	�[�x�X�e���V���쐬�N���X�B
 *	@Date	2023-11-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DEPTHSTENCIL
#define DEPTHSTENCIL

class DepthStencil
{
private:
    // �f�o�C�X�|�C���^
    Microsoft::WRL::ComPtr<ID3D11Device>                m_device;
    // �e�N�X�`���|�C���^
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_texture2D;
    // �f�v�X�X�e���V���r���[�|�C���^
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depthStencilView;
    // �t�H�[�}�b�g
    DXGI_FORMAT                                         m_format;
    // ��ʃT�C�Y
    size_t                                              m_width;
    size_t                                              m_height;

public:
    // DXGI_FORMAT���󂯎��R���X�g���N�^
    explicit DepthStencil(DXGI_FORMAT format) noexcept;

    // ���[�u�R���X�g���N�^����у��[�u������Z�q
    DepthStencil(DepthStencil&&) = default;
    DepthStencil& operator= (DepthStencil&&) = default;

    // �R�s�[���֎~���邽�߂̍폜���ꂽ�R�s�[�R���X�g���N�^����уR�s�[������Z�q
    DepthStencil(DepthStencil const&) = delete;
    DepthStencil& operator= (DepthStencil const&) = delete;

    // �f�o�C�X��ݒ肷��
    void SetDevice(_In_ ID3D11Device* device);

    // ���\�[�X�i���ƍ����j�̃T�C�Y��ݒ肷��
    void SizeResources(size_t width, size_t height);

    // �f�o�C�X����ъ֘A���郊�\�[�X���������
    void ReleaseDevice() noexcept;

    // �w�肳�ꂽRECT�\���Ɋ�Â��ăE�B���h�E�̃T�C�Y��ݒ肷��
    void SetWindow(const RECT& rect);

    // �e�N�X�`�����擾����
    ID3D11Texture2D* GetTexture2D() const noexcept { return m_texture2D.Get(); }
    // �f�v�X�X�e���V���r���[���擾����
    ID3D11DepthStencilView* GetDepthStencilView() const noexcept { return m_depthStencilView.Get(); }
    // �t�H�[�}�b�g���擾����
    DXGI_FORMAT GetFormat() const noexcept { return m_format; }
};


#endif // DEPTHSTENCIL