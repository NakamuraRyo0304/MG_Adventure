/*
 *	@File	MemoryLeak.h
 *	@Brief	メモリリークを検知するクラス。
 *	@Date	2023-06-16
 *  @Author NakamuraRyo
 */

#include <crtdbg.h>

// デバッグ版の場合、メモリリーク場所を特定するマクロ
#ifdef _DEBUG
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

inline int MemoryLeak()
{	
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}