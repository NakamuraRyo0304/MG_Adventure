/*
 *	@File	SoundManager.cpp
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SoundManager.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
SoundManager::SoundManager()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(eflags);

	m_waveBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"Resources/Sounds/skbx.xwb");

	// ���\�[�X�̍쐬
	Create();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SoundManager::~SoundManager()
{
	m_soundEffectInstances.clear();
}

/// <summary>
/// �T�E���h��炷
/// </summary>
/// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">�Ȃ炵��������ID</param>
/// <param name="playType">���[�v���P����(true�Ń��[�v�Afalse�ŒP��)</param>
/// <returns>�Ȃ�</returns>
void SoundManager::SetSound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType)
{
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Play(playType);
}

/// <summary>
/// �I�[�f�B�I�G���W���̍X�V
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// ���\�[�X�ۑ��֐�
/// </summary>
/// <param name="��������">/param>
/// <returns>�Ȃ�</returns>
void SoundManager::Create()
{
	for (unsigned int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		// �T�E���h�G�t�F�N�g�C���X�^���X���쐬����
		m_soundEffectInstances.push_back(m_waveBank->CreateInstance(i));
	}

	m_soundEffectInstances.shrink_to_fit();
}
