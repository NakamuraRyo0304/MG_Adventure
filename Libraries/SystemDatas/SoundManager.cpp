/*
 *	@File	SoundManager.cpp
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SoundManager.h"

// コンストラクタ
SoundManager::SoundManager()
	: m_masterVolume{1.0f}		// マスター音量
	, m_volumes{ 1.0f }			// 個々の音量
{
	AUDIO_ENGINE_FLAGS _eFlags = AudioEngine_Default;
#ifdef _DEBUG
	_eFlags |= AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(_eFlags);

	m_waveBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"Resources/Sounds/skbx.xwb");

	// リソースの作成
	Create();

	for (auto& i : m_volumes)
	{
		i = 1.0f;
	}
}

// デストラクタ
SoundManager::~SoundManager()
{
}

// サウンドを鳴らす
void SoundManager::PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType)
{
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Stop();
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Play(playType);

	// ならしたら元に戻す
	m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME] = 1.0f;
}

// 更新処理
void SoundManager::Update()
{
	// オーディオエンジンの更新
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"AudioEngine Error!\n");
		}
	}
}

// マスター音量設定
void SoundManager::SetMasterVolume(const float& volume)
{
	for (int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		m_volumes[i] *= volume;
		m_soundEffectInstances[i]->SetVolume(m_volumes[i]);
	}

	m_masterVolume = volume;
}

// 特定の音量調整
void SoundManager::SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME)
{
	m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME] = volume;
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->SetVolume(
		m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]);
}

// 音の減衰
void SoundManager::FadeVolume(const float& speed)
{
	m_masterVolume = m_masterVolume + speed * (0 - m_masterVolume);

	for (auto& i : m_soundEffectInstances)
	{
		i->SetVolume(m_masterVolume);
	}
}

// 音の作成
void SoundManager::Create()
{
	for (unsigned int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		// サウンドエフェクトインスタンスを作成する
		m_soundEffectInstances[i] = m_waveBank->CreateInstance(i);
		m_volumes[i] = 1.0f;
	}
}
