/*
 *	@File	SoundManager.h
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDMANAGER
#define SOUNDMANAGER

 // サウンドリスト
#include "SoundList.h"

class SoundManager
{
private:

    // オーディオエンジンへのポインタ
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

    // ウェーブバンクへのポインタ
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // サウンドエフェクトインスタンス
    std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffectInstances[XACT_WAVEBANK_SKBX_ENTRY_COUNT];

private:
    // ボリューム関連
    // マスター音量
    float m_masterVolume;

    // 各項目に対応した音量
    float m_volumes[XACT_WAVEBANK_SKBX_ENTRY_COUNT];

public:
	SoundManager();
	~SoundManager();

    // 音の更新
    void Update();

public:
    //-------------------------------------------------------------------------------------//
    // アクセサ  --------------------------------------------------------------------------//

    // TYPE : BGM or SE //// NAME : ならしたい音の名前 : bool trueでループ再生　falseで単発再生
    void PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType);

    // マスター音量の調整
    void SetMasterVolume(const float& volume);
    const float& GetMasterVolume() { return m_masterVolume; }

    // 指定された音量の調整
    void SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME);
    const float& GetVolume(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME) { return m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]; }

    // フェードさせる
    void FadeVolume(const float& speed);

private:
	// リソース作成関数
	void Create();
};

#endif // SOUNDMANAGER