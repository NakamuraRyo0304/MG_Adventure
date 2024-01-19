/*
 *	@File	Player.h
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

class Head;
class Body;
class RightLeg;
class LeftLeg;
class FactoryManager;
class Player
{
private:

	// ファクトリ
	std::unique_ptr<FactoryManager> m_factory;

	// 親(身体)
	std::unique_ptr<Body> m_parent;

	// 頭
	std::unique_ptr<Head> m_head;

	// 右足
	std::unique_ptr<RightLeg> m_legR;

	// 左足
	std::unique_ptr<LeftLeg> m_legL;

	// セレクトから受け取ったコイン数
	int m_coinNum;

	// 死亡判定
	bool is_deathFlag;

private:

	// プレイヤのサイズ
	const float SIZE = 0.85f;
	// 移動速度
	const float NORMAL_SPEED = 0.01f;
	// 回転速度
	const float ROT_SPEED = 0.05f;
	// 摩擦係数
	const float DECELERATION = 0.851f;
	// プレイヤの浮遊
	const float OFFSET_Y = 0.2f;
	// 死亡ライン
	const float DEATH_LINE = -10.0f;
	// ウインクスパン
	const float WINK_SPAN = 0.95f;
	// ライトの変換速度
	const float LIGHT_SPEED = 0.1f;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="head">頭部のモデルデータ</param>
	/// <param name="body">胴体のモデルデータ</param>
	/// <param name="right">右足モデルデータ</param>
	/// <param name="left">左足モデルデータ</param>
	/// <returns>なし</returns>
	Player(std::unique_ptr<DirectX::Model> head,std::unique_ptr<DirectX::Model> body,
		std::unique_ptr<DirectX::Model> rightLeg, std::unique_ptr<DirectX::Model> leftLeg);
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states,DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

private:

	/// <summary>
	/// nullだったらオブジェクトを作成する
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void NewCreate();

public:

	/// <summary>
	/// リスポーンさせる関数
	/// </summary>
	/// <param name="spawnPosition">プレイヤーの座標を指定する</param>
	/// <returns>なし</returns>
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// アクセサ----------------------------------------------------------------------------//

	// ポジションを取得
	const DirectX::SimpleMath::Vector3& GetPosition();
	// ポジションを設定
	void SetPosition(const DirectX::SimpleMath::Vector3& position);
	// 重力リセット
	void ResetGravity();
	// 重力を取得
	const float& GetGravity();
	// 重力を設定
	void SetGravity(const float& gravity);
	// 回転量を取得
	const DirectX::SimpleMath::Quaternion& GetRotation();
	// サイズを取得
	const float& GetSize() { return SIZE; }
	// 死亡判定を取得
	const bool& GetDeathFlag() { return is_deathFlag; }
	// 合計コイン数を設定
	void SetAllCoins(const int& allCoins) { m_coinNum = allCoins; }
};

#endif // PLAYER