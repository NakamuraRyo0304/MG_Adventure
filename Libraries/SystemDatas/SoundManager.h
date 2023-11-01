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

    /// <summary>
    /// オーディオエンジンの更新
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Update();

public:
    //-------------------------------------------------------------------------------------//
    // アクセサ  --------------------------------------------------------------------------//

    /// <summary>
    /// サウンドを鳴らす
    /// </summary>
    /// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">ならしたい音のID</param>
    /// <param name="playType">ループか単発か(trueでループ、falseで単発)</param>
    /// <returns>なし</returns>
    void PlaySound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME, const bool& playType);

    /// <summary>
    /// マスター音量の設定
    /// </summary>
    /// <param name="volume">音量(0.0f～1.0f)</param>
    /// <returns>なし</returns>
    void SetMasterVolume(const float& volume);

    /// <summary>
    /// マスター音量の取得
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>マスター音量</returns>
    const float& GetMasterVolume() { return m_masterVolume; }

    /// <summary>
    /// 指定した音の音量を設定
    /// </summary>
    /// <param name="volume">音量</param>
    /// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">音のID</param>
    /// <returns>なし</returns>
    void SetVolume(const float& volume, const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME);

    /// <summary>
    /// 指定した音の音量を取得
    /// </summary>
    /// <param name="WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME">音のID</param>
    /// <returns>なし</returns>
    const float& GetVolume(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME) { return m_volumes[WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME]; }

    /// <summary>
    /// 音をフェードさせる
    /// </summary>
    /// <param name="speed">速度</param>
    /// <returns>なし</returns>
    void FadeVolume(const float& speed);

private:

    /// <summary>
    /// リソース保存関数
    /// </summary>
    /// <param name="引数無し">/param>
    /// <returns>なし</returns>
	void Create();
};

#endif // SOUNDMANAGER