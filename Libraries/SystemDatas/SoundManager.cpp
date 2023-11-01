/*
 *	@File	SoundManager.cpp
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SoundManager.h"

// �R���X�g���N�^
SoundManager::SoundManager()
	: m_masterVolume{1.0f}		// �}�X�^�[����
	, m_volumes{ 1.0f }			// �X�̉���
{
	AUDIO_ENGINE_FLAGS _eFlags = AudioEngine_Default;
#ifdef _DEBUG
	_eFlags |= AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(_eFlags);

	m_waveBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"Resources/Sounds/skbx.xwb");

	// ���\�[�X�̍쐬
	Create();

	for (auto& i : m_volumes)
	{
		i = 1.0f;
	}
}

// �f�X�g���N�^
SoundManager::~SoundManager()
{
}

// �T�E���h��炷
void SoundManager::PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType)
{
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Stop();
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Play(playType);

	// �Ȃ炵���猳�ɖ߂�
	m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME] = 1.0f;
}

// �X�V����
void SoundManager::Update()
{
	// �I�[�f�B�I�G���W���̍X�V
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"AudioEngine Error!\n");
		}
	}
}

// �}�X�^�[���ʐݒ�
void SoundManager::SetMasterVolume(const float& volume)
{
	for (int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		m_volumes[i] *= volume;
		m_soundEffectInstances[i]->SetVolume(m_volumes[i]);
	}

	m_masterVolume = volume;
}

// ����̉��ʒ���
void SoundManager::SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME)
{
	m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME] = volume;
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->SetVolume(
		m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]);
}

// ���̌���
void SoundManager::FadeVolume(const float& speed)
{
	m_masterVolume = m_masterVolume + speed * (0 - m_masterVolume);

	for (auto& i : m_soundEffectInstances)
	{
		i->SetVolume(m_masterVolume);
	}
}

// ���̍쐬
void SoundManager::Create()
{
	for (unsigned int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		// �T�E���h�G�t�F�N�g�C���X�^���X���쐬����
		m_soundEffectInstances[i] = m_waveBank->CreateInstance(i);
		m_volumes[i] = 1.0f;
	}
}
