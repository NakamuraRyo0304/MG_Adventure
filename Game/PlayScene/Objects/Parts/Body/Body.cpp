/*
 *	@File	Body.cpp
 *	@Brief	プレイヤーの身体。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "Body.h"

 // コンストラクタ
Body::Body(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// 基底クラス
	, m_param{}					// パラメータ
{
}

// デストラクタ
Body::~Body()
{
}

// 通常の更新
void Body::Update()
{
	auto _key = Keyboard::Get().GetState();

	// 左右旋回をする
	float _rotationAngle = 0.0f;
	_rotationAngle = _key.A ? ROT_SPEED : _key.D ? -ROT_SPEED : 0.0f;
	SimpleMath::Quaternion _q = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, _rotationAngle);
	m_param.rotation *= _rotationAngle != 0.0f ? _q : SimpleMath::Quaternion::Identity;

	// 前後移動をする
	SimpleMath::Vector3 _moveDirection(0.0f, 0.0f, 0.0f);
	_moveDirection.z = _key.W ? -m_param.accelerate : _key.S ? m_param.accelerate * 0.5f : 0.0f;
	_moveDirection = SimpleMath::Vector3::Transform(_moveDirection, m_param.rotation);
	m_param.velocity += _moveDirection;

	// 移動量の計算
	m_param.position += m_param.velocity;

	// 減速処理
	if (m_param.velocity != SimpleMath::Vector3::Zero)
	{
		m_param.velocity *= DECELERATION;
	}

	// 重力の加算
	m_param.gravity += 0.015f;

	// 重力の反映
	m_param.position.y -= m_param.gravity;

	// 回転行列
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_param.rotation);

	// 移動行列
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_param.position.x, m_param.position.y + OFFSET_Y, m_param.position.z);

	// 行列の作成
	SimpleMath::Matrix _world = _rotate * _trans;

	// 行列の設定
	SetMatrix(_world);
}

// 描画処理
void Body::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix(), view, proj);
}

// パラメータリセット
void Body::ResetAll()
{
	m_param.reset();

	// 加速度の設定
	m_param.accelerate = MOVE_SPEED;
}
