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

#include<vector>

class SoundManager
{
private:

    // オーディオエンジンへのポインタ
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

    // ウェーブバンクへのポインタ
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // サウンドエフェクトインスタンス
    std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectInstances;

public:
	SoundManager();
	~SoundManager();

    // TYPE : BGM or SE //// NAME : ならしたい音の名前 : bool trueでループ再生　falseで単発再生
    void SetSound(const XACT_WAVEBANK_SKBX& WAVEBANKXACT_WAVEBANK_SKBX_TYPE_NAME , const bool& playType );

    // 音の更新
    void Update();

private:
	// リソース作成関数
	void Create();
};

#endif // SOUNDMANAGER