/*
 *	@File	SoundEffect.cpp
 *	@Brief	SEの再生に使うマネージャクラス。
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "xaudio2.h"

#include "SoundEffect.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
SoundEffect::SoundEffect()
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SoundEffect::~SoundEffect()
{
}

/// <summary>
/// リソース保存関数
/// </summary>
/// <param name="key">登録キー</param>
/// <param name="path">リソースのパス</param>
/// <returns>なし</returns>
void SoundEffect::AddSoundEffects(const wchar_t* key, const wchar_t* path)
{
	// キーパスの保存
	m_sounds.emplace(key, path);

	// 対象パスを確保
	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_sounds.find(key);


}
