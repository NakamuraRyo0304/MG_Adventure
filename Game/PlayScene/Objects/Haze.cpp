/*
 *	@File	Haze.cpp
 *	@Brief	��ʂ̎���̂������B
 *	@Date	2023-10-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Haze.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Haze::Haze()
	: m_pDR{ nullptr }		// �f�o�C�X���\�[�X
	, m_timer{ 0.0f }		// �^�C�}�[
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Haze::~Haze()
{
}

/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
/// <param name="pDR">�f�o�C�X���\�[�X�̃|�C���^</param>
/// <returns>�Ȃ�</returns>
void Haze::CreateShader(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	auto _context = m_pDR->GetD3DDeviceContext();
	auto _device = m_pDR->GetD3DDevice();



}

