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
	m_soundEffectInstances.clear();
}

/// <summary>
/// サウンドを鳴らす
/// </summary>
/// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">ならしたい音のID</param>
/// <param name="playType">ループか単発か(trueでループ、falseで単発)</param>
/// <returns>なし</returns>
void SoundManager::SetSound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType)
{
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
/// リソース保存関数
/// </summary>
/// <param name="引数無し">/param>
/// <returns>なし</returns>
void SoundManager::Create()
{
	for (unsigned int i = 0; i < XACT_WAVEBANK_SKBX_ENTRY_COUNT; ++i)
	{
		// サウンドエフェクトインスタンスを作成する
		m_soundEffectInstances.push_back(m_waveBank->CreateInstance(i));
	}

	m_soundEffectInstances.shrink_to_fit();
}
