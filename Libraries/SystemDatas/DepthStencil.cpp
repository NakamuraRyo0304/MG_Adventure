/*
 *	@File	DepthStencil.cpp
 *	@Brief	�[�x�X�e���V���쐬�N���X�B
 *	@Date	2023-11-24
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "DepthStencil.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
DepthStencil::DepthStencil(DXGI_FORMAT format) noexcept
    : m_format(format)      // �t�H�[�}�b�g
    , m_width(0)            // ����
    , m_height(0)           // �c��
{
}

//==============================================================================
// �f�o�C�X��ݒ肷��
//==============================================================================
void DepthStencil::SetDevice(_In_ ID3D11Device* device)
{
    // �ݒ肳��Ă����珈�����Ȃ�
    if (device == m_device.Get()) return;

    // ��x�N���A����
    if (m_device) ReleaseDevice();

    // �w�肳�ꂽ�t�H�[�}�b�g���f�o�C�X�ŃT�|�[�g����Ă��邩�m�F
    UINT formatSupport = 0;
    if (FAILED(device->CheckFormatSupport(m_format, &formatSupport)))
    {
        throw std::runtime_error("CheckFormatSupport");
    }

    // �f�o�C�X���K�v�ȃt�H�[�}�b�g�@�\���T�|�[�g���Ă��邩�m�F
    constexpr UINT32 required = D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_DEPTH_STENCIL;
    if ((formatSupport & required) != required)
    {
        throw std::runtime_error("DepthStencil");
    }
    m_device = device;
}

//==============================================================================
// ���\�[�X�i���ƍ����j�̃T�C�Y��ݒ肷��
//==============================================================================
void DepthStencil::SizeResources(size_t width, size_t height)
{
    if (width == m_width && height == m_height) return;

    if (m_width > UINT32_MAX || m_height > UINT32_MAX)
    {
        throw std::out_of_range("�����ȕ�/����");
    }

    if (not m_device) return;

    m_width = m_height = 0;

    // �f�v�X�X�e���V���e�N�X�`���ƃr���[���쐬����
    CD3D11_TEXTURE2D_DESC depthStencilDesc(m_format, static_cast<UINT>(width), static_cast<UINT>(height),
        1, 1, D3D11_BIND_DEPTH_STENCIL);

    DX::ThrowIfFailed(m_device->CreateTexture2D(
        &depthStencilDesc,
        nullptr,
        m_texture2D.ReleaseAndGetAddressOf()
    ));

    DX::ThrowIfFailed(m_device->CreateDepthStencilView(
        m_texture2D.Get(),
        nullptr,
        m_depthStencilView.ReleaseAndGetAddressOf()
    ));

    m_width = width;
    m_height = height;
}

//==============================================================================
// Direct3D�f�o�C�X����ъ֘A���郊�\�[�X���������
//==============================================================================
void DepthStencil::ReleaseDevice() noexcept
{
    m_depthStencilView.Reset();
    m_texture2D.Reset();
    m_device.Reset();
    m_width = m_height = 0;
}

//==============================================================================
// �w�肳�ꂽRECT�\���Ɋ�Â��ăE�B���h�E�̃T�C�Y��ݒ肷��
//==============================================================================
void DepthStencil::SetWindow(const RECT& output)
{
    // �s�N�Z���P�ʂŃ����_�����O�^�[�Q�b�g�T�C�Y�����肷��
    auto const width = size_t(std::max<LONG>(output.right - output.left, 1));
    auto const height = size_t(std::max<LONG>(output.bottom - output.top, 1));

    // �E�B���h�E�T�C�Y�Ɋ�Â��ă��\�[�X�̃T�C�Y��ݒ肷��
    SizeResources(width, height);
}