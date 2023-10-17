/*
 *	@File	Blocks.h
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// マップの使用
#include <map>

// エイリアス
using MAPSTATE = MapLoad::BOXSTATE;

class Blocks
{
private:

	// マップローダー
	std::unique_ptr<MapLoad> m_mapLoad;

	// ブロック
	std::vector<Object> m_mapObj;

	// 雲ステータス
	struct Cloud
	{
		bool moveFlag = false;
		float timer = 0.0f;
		DirectX::SimpleMath::Vector3 initPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// フラグ管理
	std::map<int, Cloud> m_cloudState;

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

	// ブロックのシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psCloud;

private:
	// マップサイズ
	const float COMMON_SIZE = 0.9f;

	// コインのサイズ
	const float COIN_SIZE = COMMON_SIZE / 3;

	// 雲のサイズ
	const float CLOUD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOUD_SPEED = 0.1f;

	// 雲リセットエリアサイズ
	const float CLOUD_RESET_SIZE = 0.85f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// モデル
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_cloudModel;
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
	void Update();

	// 描画
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer);

	// 終了処理
	void Finalize();

	// モデル作成
	void CreateModels(std::unique_ptr<Model> model, int modelName);

	// シェーダーの作成
	void CreateShader(ID3D11Device1* device);

public:
	// コインのカウントアップ
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const int& GetMaxCoinCount() { return m_maxCoins; }
	const bool& IsCollectedFlag();

	// プレイヤー座標ゲッター
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// マップ全体のゲッター
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// ブロック単体の座標ゲッター
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index)
	{
		if (index >= m_mapObj.max_size()) throw std::out_of_range("Index out of range");
		return m_mapObj[index].position;
	}
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	// ブロックの大きさゲッター
	const float& GetObjSize(const int& objName);

	// 雲を元の場所に戻す処理
	void RestoreCloudPosition();

	// 雲の判定アクセサ
	void SetCloudHitFlag(const int& index, bool flag) { m_cloudState[index].moveFlag = flag; }

private:

	// マップセレクト
	std::wstring MapSelect(int num);
	// モデルの色を変更
	void ChangeModelColors(std::unique_ptr<Model>& model,DirectX::SimpleMath::Vector4 color);
};