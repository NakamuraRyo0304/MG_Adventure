/*
 *	@File	Blocks.h
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// マップの使用
#include <map>

// エイリアス
using MapState = MapLoad::BoxState;

class Blocks
{
private:

	// マップローダー
	std::unique_ptr<MapLoad> m_mapLoad;

	// ブロック
	std::vector<Object> m_mapObj;

	// 雲ステータス
	struct Clowd
	{
		bool moveFlag = false;
		float timer = 0.0f;
		DirectX::SimpleMath::Vector3 initPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// フラグ管理
	std::map<int, Clowd> m_clowdState;

	// プレイヤー座標
	DirectX::SimpleMath::Vector3 m_playerPos;

	// コインカウンタ
	int m_coinCount;
	// コインの最大値
	int m_maxCoins;
	// コイン回収済みフラグ
	bool is_collectedFlag;

	// 当たったかどうかの判定
	bool is_hitCoinFlag;

private:
	// マップサイズ
	const float COMMON_SIZE = 0.9f;

	// コインのサイズ
	const float COIN_SIZE = COMMON_SIZE / 3;

	// 雲のサイズ
	const float CLOWD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOWD_SPEED = 0.1f;

	// 雲リセットエリアサイズ
	const float CLOWD_RESET_SIZE = 0.85f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// モデル
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_clowdModel;
	std::unique_ptr<DirectX::Model> m_resetPtModel;

public:
	// ブロックの種類
	enum { GRASS, COIN, CLOWD, RECLOWD, LENGTH };

public:

	// コンストラクタ
	Blocks();

	// デストラクタ
	~Blocks();

	// 初期化
	void Initialize(int stageNum);

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer);

	// 終了処理
	void Finalize();

	// モデル作成
	void CreateModels(std::unique_ptr<Model> model, int modelName);


public:
	// コインのカウントアップ
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const bool& IsCollectedFlag();

	// プレイヤー座標ゲッター
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// マップ全体のゲッター
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// ブロック単体の座標ゲッター
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index) { return m_mapObj[index].position; }
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	// ブロックの大きさゲッター
	const float& GetObjSize(const int& objName);

	// 雲を元の場所に戻す処理
	void RestoreClowdPosition();

	// 雲の判定アクセサ
	void SetClowdHitFlag(const int& index, bool flag) { m_clowdState[index].moveFlag = flag; }

private:

	// マップセレクト
	std::wstring MapSelect(int num);

private:

	// モデルの色を変更
	void ChangeModelColors(std::unique_ptr<Model>& model,DirectX::SimpleMath::Vector4 color);
};