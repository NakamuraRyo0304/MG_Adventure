/*
 *	@File	SoundManager.h
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDMANAGER
#define SOUNDMANAGER

#include <map>

class SoundManager
{
private:

	// オーディオエンジンへのポインタ
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

public:
	SoundManager();
	~SoundManager();

	// リソース追加関数
	void AddSoundEffects(const wchar_t* key, const wchar_t* path);
};

#endif // SOUNDMANAGER