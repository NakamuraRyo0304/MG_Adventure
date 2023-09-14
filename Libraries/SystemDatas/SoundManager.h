/*
 *	@File	SoundManager.h
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDMANAGER
#define SOUNDMANAGER

 // �T�E���h���X�g
#include "SoundList.h"

#include<vector>

class SoundManager
{
private:

    // �I�[�f�B�I�G���W���ւ̃|�C���^
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

    // �E�F�[�u�o���N�ւ̃|�C���^
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // �T�E���h�G�t�F�N�g�C���X�^���X
    std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectInstances;

public:
	SoundManager();
	~SoundManager();

    // TYPE : BGM or SE //// NAME : �Ȃ炵�������̖��O : bool true�Ń��[�v�Đ��@false�ŒP���Đ�
    void SetSound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME , const bool& playType );

    // ���̍X�V
    void Update();

private:
	// ���\�[�X�쐬�֐�
	void Create();
};

#endif // SOUNDMANAGER