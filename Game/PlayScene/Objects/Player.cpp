/*
 *	@File	Player.cpp
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "Parts/Head/Head.h"
#include "Parts/Body/Body.h"
#include "Parts/RightLeg/RightLeg.h"
#include "Parts/LeftLeg/LeftLeg.h"
#include "Player.h"

// �R���X�g���N�^
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left)
	: m_coinNum{}					// ���v�l���R�C����
	, is_deathFlag{}				// ���S�t���O
{
	m_factory = std::make_unique<FactoryManager>();
	m_parent = std::make_unique<Body>(std::move(body));
	m_head = std::make_unique<Head>(std::move(head));
	m_legR = std::make_unique<RightLeg>(std::move(right));
	m_legL = std::make_unique<LeftLeg>(std::move(left));
}

// �f�X�g���N�^
Player::~Player()
{
	Finalize();
}

// ����������
void Player::Initialize()
{
	// �t�@�N�g���̍쐬
	m_factory->CreateFactory();

	// �p�����[�^�̃��Z�b�g
	m_parent->ResetAll();

	// ���S����̏�����
	is_deathFlag = false;
}

// �X�V����
void Player::Update()
{
	// �e�̍X�V
	m_parent->Update();

	// ���S����
	is_deathFlag = m_parent->GetPosition().y < DEATH_LINE;

	// ���̍X�V����
	m_head->Update();

	// ���̍X�V����
	m_legR->Update();
	m_legL->Update();
}

// �`�揈��
void Player::Render(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _world = m_parent->GetMatrix();

	// �E���̕`��
	m_legR->SetParentMatrix(_world);
	m_legR->Draw(states, view, proj);

	// �����̕`��
	m_legL->SetParentMatrix(_world);
	m_legL->Draw(states, view, proj);

	// �����̕`��
	m_head->SetParentMatrix(_world);
	m_head->Draw(states, view, proj);

	// �g�̂̕`��
	m_parent->SetParentMatrix(SimpleMath::Matrix::Identity);
	m_parent->Draw(states, view, proj);
}

// �I������
void Player::Finalize()
{
	m_head.reset();
	m_legR.reset();
	m_legL.reset();
	m_parent.reset();
	m_factory.reset();
}

// �V�K�쐬
void Player::NewCreate()
{
	// �t�@�N�g���}�l�[�W��
	m_factory->BuildModelFactory();

	// �t�@�N�g���[���烂�f�������炤
	auto _head = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");

	// �V�K�쐬
	m_parent = std::make_unique<Body>(std::move(_body));
	m_head	 = std::make_unique<Head>(std::move(_head));
	m_legR   = std::make_unique<RightLeg>(std::move(_legR));
	m_legL   = std::make_unique<LeftLeg>(std::move(_legL));

	// �t�@�N�g����j��
	m_factory->LeaveModelFactory();
}

// ���X�|�[���֐�
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parent->ResetAll();

	// ���W�̐ݒ�
	m_parent->SetPosition(spawnPosition);
}

// ���W�̎擾
const SimpleMath::Vector3& Player::GetPosition()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetPosition();
}

// ���W�̐ݒ�
void Player::SetPosition(const SimpleMath::Vector3& position)
{
	if (not m_parent)
	{
		NewCreate();
	}
	m_parent->SetPosition(position);
}

// �d�͂̃��Z�b�g
void Player::ResetGravity()
{
	m_parent->ZeroGravity();
}

// �d�͂��擾
const float& Player::GetGravity()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetGravity();
}

// �d�͂̐ݒ�
void Player::SetGravity(const float& gravity)
{
	m_parent->SetGravity(gravity);
}

// ��]�̎擾
const SimpleMath::Quaternion& Player::GetRotation()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetRotation();
}
