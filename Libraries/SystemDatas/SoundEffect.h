/*
 *	@File	SoundEffect.h
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDEFFECT
#define SOUNDEFFECT

#include <map>

class SoundEffect
{
private:
	// 音源の格納配列
	std::map<const wchar_t*, const wchar_t*> m_sounds;

	// サウンドへのポインタ

public:
	SoundEffect();
	~SoundEffect();

	// リソース追加関数
	void AddSoundEffects(const wchar_t* key, const wchar_t* path);
};

#endif // SOUNDEFFECT