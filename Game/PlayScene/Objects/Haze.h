/*
 *	@File	Haze.h
 *	@Brief	��ʂ̎���̂������B
 *	@Date	2023-10-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef HAZE
#define HAZE

class Haze
{
private:

	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;

	// �^�C�}�[
	float m_timer;

public:

	Haze();
	~Haze();

	// �V�F�[�_�[�̍쐬
	void CreateShader(DX::DeviceResources* pDR);

	// �^�C�}�[�̃Z�b�g
	void SetTimer(const float& timer) { m_timer = timer; };
};

#endif // HAZE