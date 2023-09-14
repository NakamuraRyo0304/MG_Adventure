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

class SoundManager
{
private:

    // �I�[�f�B�I�G���W���ւ̃|�C���^
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

    // �E�F�[�u�o���N�ւ̃|�C���^
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // �T�E���h�G�t�F�N�g�C���X�^���X
    std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffectInstances[XACT_WAVEBANK_SKBX_ENTRY_COUNT];

private:
    // �{�����[���֘A
    // �}�X�^�[����
    float m_masterVolume;

    // �e���ڂɑΉ���������
    float m_volumes[XACT_WAVEBANK_SKBX_ENTRY_COUNT];

public:
	SoundManager();
	~SoundManager();

    // ���̍X�V
    void Update();

public:
    //-------------------------------------------------------------------------------------//
    // �A�N�Z�T  --------------------------------------------------------------------------//

    // TYPE : BGM or SE //// NAME : �Ȃ炵�������̖��O : bool true�Ń��[�v�Đ��@false�ŒP���Đ�
    void PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType);

    // �}�X�^�[���ʂ̒���
    void SetMasterVolume(const float& volume);
    const float& GetMasterVolume() { return m_masterVolume; }

    // �w�肳�ꂽ���ʂ̒���
    void SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME);
    const float& GetVolume(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME) { return m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]; }

    // �t�F�[�h������
    void FadeVolume(const float& speed);

private:
	// ���\�[�X�쐬�֐�
	void Create();
};

#endif // SOUNDMANAGER