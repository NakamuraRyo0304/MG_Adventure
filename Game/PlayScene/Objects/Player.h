/*
 *	@File	Player.h
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

struct PlayerParameter
{
	DirectX::SimpleMath::Vector3 velocity;		// 移動量
	DirectX::SimpleMath::Quaternion rotate;		// 向いている方向
	float gravity;								// 重力
	float accelerate;							// 加速度
	void reset()								// リセット関数
	{
		velocity = DirectX::SimpleMath::Vector3::Zero;
		gravity = 0.0f;
		accelerate = 0.0f;
		rotate = SimpleMath::Quaternion::Identity;
	}
};

class SystemManager;
class Player
{
private:
	// 時間
	float m_timer;

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// プレイヤのパラメータ
	PlayerParameter m_parameter;

	// モデルデータ
	std::unique_ptr<DirectX::Model> m_model;

	// システム
	std::weak_ptr<SystemManager> m_system;

	// 死亡判定
	bool is_deathFlag;

	// プレイヤのサイズ
	const float SIZE = 0.95f;
	// 回転速度
	const float ROT_SPEED = 0.05f;
	// 摩擦係数
	const float DECELERATION = 0.842f;
	// プレイヤの浮遊
	const float OFFSET_Y = 0.2f;
	// 死亡ライン
	const float DEATH_LINE = -10.0f;


public:
	// コンストラクタ（モデルデータ）
	Player(std::unique_ptr<DirectX::Model> model);
	~Player();

	// 初期化処理
	void Initialize(std::shared_ptr<SystemManager> system);

	// 更新処理（キーボード）
	void Update(DirectX::Keyboard::State& keyState,float timer);
	
	// 描画処理（コンテキスト、ステート、ビュー行列、射影行列）
	void Render(ID3D11DeviceContext* context, DirectX::DX11::CommonStates& states, 
		        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// 終了処理
	void Finalize();

	// 重力処理
	void UpdateGravity();

	// モデルのセット
	void SetModel(std::unique_ptr<DirectX::Model> model) { m_model = std::move(model); }

	// ポジションアクセサ
	DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	// スポーン関数
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// 重力リセット
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// 重力アクセサ
	const float& GetGravity() { return m_parameter.gravity; }
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }

	// サイズアクセサ
	const float& GetSize() { return SIZE; }

	// 死亡判定ゲッター
	const bool& GetDeathFlag() { return is_deathFlag; }
};

#endif // PLAYER