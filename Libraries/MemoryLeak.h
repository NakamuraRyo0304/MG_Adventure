/*
 *	@File	MemoryLeak.h
 *	@Brief	���������[�N�����m����N���X�B
 *	@Date	2023-06-16
 *  @Author NakamuraRyo
 */

#include <crtdbg.h>

// �f�o�b�O�ł̏ꍇ�A���������[�N�ꏊ����肷��}�N��
#ifdef _DEBUG
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

inline int MemoryLeak()
{	
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}