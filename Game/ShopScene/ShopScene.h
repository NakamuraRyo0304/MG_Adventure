/*
 *	@File	ShopScene.h
 *	@Brief	�X�L���V���b�v�̃V�[���B
 *	@Date	2023-10-04
 *  @Author NakamuraRyo
 */

#ifndef SHOPSCENE
#define SHOPSCENE

#include "../IScene.h"

class ShopScene final : public IScene
{
private:

public:
	// �R���X�g���N�^
	ShopScene();

	// �f�X�g���N�^
	~ShopScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

private:

	// �V�[�����̕ϐ��������֐�
	void SetSceneValues() override;



};

#endif // SHOPSCENE