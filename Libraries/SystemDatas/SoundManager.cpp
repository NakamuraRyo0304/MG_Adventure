/*
 *	@File	SoundManager.cpp
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SoundManager.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
SoundManager::SoundManager()
	: m_masterVolume{1.0f}		// マスター音量
	, m_volumes{ 1.0f }			// 個々の音量
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(eflags);

	m_waveBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"Resources/Sounds/skbx.xwb");

	// リソースの作成
	Create();
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SoundManager::~SoundManager()
{
}

/// <summary>
/// サウンドを鳴らす
/// </summary>
/// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">ならしたい音のID</param>
/// <param name="playType">ループか単発か(trueでループ、falseで単発)</param>
/// <returns>なし</returns>
void SoundManager::PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType)
{
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Stop();
	m_soundEffectInstances[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]->Play(playType);
}

/// <summary>
/// オーディオエンジンの更新
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
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

/// <summary>
/// マスター音量の設定
/// </summary>
/// <param name="volume">音量(0.0f～1.0f)</param>
/// <returns>なし</returns>
void SoundManager::SetMasterVolume(const float& volume)
{
	for (int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		m_volumes[i] *= volume;
		m_soundEffectInstances[i]->SetVolume(m_volumes[i]);
	}

	m_masterVolume = volume;
}

/// <summary>
/// 指定した音の音量調整
/// </summary>
/// <param name="volume">音量</param>
/// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">音のID</param>
/// <returns>なし</returns>
void SoundManager::SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME)
{
	m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME] = volume;
}

/// <summary>
/// 音をフェードさせる
/// </summary>
/// <param name="speed">速度</param>
/// <returns>なし</returns>
void SoundManager::FadeVolume(const float& speed)
{
	m_masterVolume = m_masterVolume + speed * (0 - m_masterVolume);
}

/// <summary>
/// リソース保存関数
/// </summary>
/// <param name="引数無し">/param>
/// <returns>なし</returns>
void SoundManager::Create()
{
	for (unsigned int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		// サウンドエフェクトインスタンスを作成する
		m_soundEffectInstances[i] = m_waveBank->CreateInstance(i);
		m_volumes[i] = 1.0f;
	}
}
