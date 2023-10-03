/*
 *	@File	Blocks.cpp
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"

// CSV読み込み
#include "../../../Libraries/SystemDatas/MapLoad.h"

// モデルファクトリ
#include "../../../Libraries/Factories/ModelFactory.h"

// ユーザーユーティリティ
#include "../../../Libraries/UserUtility.h"

#include "Blocks.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
Blocks::Blocks()
	: m_coinCount{0}							// コインカウンタ
	, m_maxCoins{0}								// コイン最大値
	, m_playerPos{ SimpleMath::Vector3::Zero }	// プレイヤーポジション
	, m_grassModel{ nullptr }					// 草ブロックのモデル
	, m_coinModel{ nullptr }					// コインブロックのモデル
	, m_cloudModel{ nullptr }					// 雲ブロックのモデル
	, is_collectedFlag{ false }					// コイン回収済み判定フラグ
	, is_hitCoinFlag{ false }					// 判定フラグ
{
	m_mapLoad = std::make_unique<MapLoad>();
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Blocks::~Blocks()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="stageNum">ステージ番号</param>
/// <returns>なし</returns>
void Blocks::Initialize(int stageNum)
{
	// ファイル名の宣言
	std::wstring filePath = L"";

	// ステージセレクト
	filePath = MapSelect(stageNum);

	// マップの読み込み
	m_mapLoad->LoadMap(filePath);

	// マップの格納
	m_mapObj = m_mapLoad->GetMapData();

	// メモリの解放
	m_mapLoad->ReleaseMemory();

	// 座標補正
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// インデックス番号の格納
		m_mapObj[i].index = i;

		// マップの座標設定
		m_mapObj[i].position.x -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;

		// コインの枚数のカウント
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_maxCoins++;
		}
		// 雲フラグ登録
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// ステータス登録
			m_cloudState[i].moveFlag = false;

			// 始発点を保存
			m_cloudState[i].initPosition = m_mapObj[i].position;

			// 終着点を保存
			m_cloudState[i].endPosition = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE + CLOWD_SIZE,
				m_mapObj[i].position.z
			};
		}
		// プレイヤの座標を代入
		if (m_mapObj[i].id == MAPSTATE::PLAYER)
		{
			m_playerPos = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE / 2,
				m_mapObj[i].position.z
			};

			// 代入後に該当マスを空気に変える(判定除去)
			m_mapObj[i].id = MAPSTATE::NONE;
		}
	}

	// コイン回収済みフラグをFalseにする
	is_collectedFlag = false;
	// 当たり判定フラグをFalseにする
	is_hitCoinFlag = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間</param>
/// <returns>なし</returns>
void Blocks::Update(float elapsedTime)
{
	elapsedTime;

	// 雲は上下移動する
	for(int i = 0; i < m_mapObj.size();++i)
	{
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// 触れているときは終点まで
			if (m_cloudState[i].moveFlag)
			{
				// Y座標を終点まで動かす
				m_mapObj[i].position.y = UserUtility::Lerp
				(
					m_mapObj[i].position.y,
					m_cloudState[i].endPosition.y,
					CLOWD_SPEED
				);
			}
		}
	}

	// コインを回収したらTrueにする
	if (m_coinCount == m_maxCoins)
	{
		is_collectedFlag = true;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="context">ID3D11DeviceContextポインタ</param>
/// <param name="states">コモンステート</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="timer">経過時間</param>
/// <returns>なし</returns>
void Blocks::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj, float timer)
{
	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix world =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// 回転行列
		SimpleMath::Matrix rotateY = SimpleMath::Matrix::CreateRotationY(timer);

		// スケール行列
		float restrictedTimer = fmodf(timer, 2 * XM_PI);
		SimpleMath::Matrix cl_scale = SimpleMath::Matrix::CreateScale(sinf(restrictedTimer));

		// ライトの設定
		SimpleMath::Vector3 lightDirection(1.0f, -1.0f, -1.0f);

		// ビュー行列からカメラの回転を取得
		SimpleMath::Matrix cameraRotation;

		// ビュー行列を逆変換
		cameraRotation = view.Invert();

		// 移動量をなくす
		cameraRotation._41 = 0.0f;
		cameraRotation._42 = 0.0f;
		cameraRotation._43 = 0.0f;

		// ライトの方向をカメラの回転に逆向きにする
		lightDirection = SimpleMath::Vector3::TransformNormal(lightDirection, cameraRotation);
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

		auto setLightForModel = [&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ライトオン
				lights->SetLightEnabled(0, true);
				lights->SetLightEnabled(1, true);
				lights->SetLightEnabled(2, true);

				// ライトの方向を設定
				lights->SetLightDirection(0, lightDirection);
				lights->SetLightDirection(1, lightDirection);
				lights->SetLightDirection(2, lightDirection);

				// ライトの色を暗めのグレーに設定
				lights->SetLightDiffuseColor(0, lightColor);
				lights->SetLightDiffuseColor(1, lightColor);
				lights->SetLightDiffuseColor(2, lightColor);
			}

			// フォグ
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				// 霧を使うシェーダーに切り替える
				fog->SetFogEnabled(true);

				// フォグの色を決める
				fog->SetFogColor(Colors::White);

				// スタート
				fog->SetFogStart(-50.0f);

				// エンド
				fog->SetFogEnd(150.0f);
			}
		};


		// 草ブロック
		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(setLightForModel);
			m_grassModel->Draw(context, states, world, view, proj);
		}

		// コインブロック
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_coinModel->UpdateEffects(setLightForModel);
			m_coinModel->Draw(context, states, rotateY * world, view, proj);
		}

		// 雲ブロック
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			if (m_cloudState[i].moveFlag)
			{
				// 色を暗く変更
				ChangeModelColors(m_cloudModel, static_cast<SimpleMath::Vector4>(Colors::DarkGray));
			}
			else
			{
				// 色をデフォルトに戻す
				ChangeModelColors(m_cloudModel,static_cast<SimpleMath::Vector4>(Colors::White));
			}

			m_cloudModel->UpdateEffects(setLightForModel);
			m_cloudModel->Draw(context, states, rotateY * world, view, proj);
		}

		// 雲のリセットポイント
		if (m_mapObj[i].id == MAPSTATE::RESET)
		{
			// 反転防止
			if (sinf(restrictedTimer) < 0.0f)
			{
				cl_scale *= SimpleMath::Matrix::CreateScale(1.0f, -1.0f,1.0f);
			}

			m_resetPtModel->UpdateEffects(setLightForModel);
			m_resetPtModel->Draw(context, states, cl_scale * rotateY * world, view, proj);
		}
	}
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Blocks::Finalize()
{
	// マップの解放
	m_mapObj.clear();

	// モデルの解放
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_resetPtModel);
}

/// <summary>
/// モデル作成
/// </summary>
/// <param name="model">モデルデータ</param>
/// <param name="modelNum">対応したモデル番号</param>
/// <returns>なし</returns>
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:									// 草ブロック
		m_grassModel = std::move(model);
		break;
	case COIN:									// コイン
		m_coinModel = std::move(model);
		break;
	case CLOWD:									// 雲ブロック
		m_cloudModel = std::move(model);
		break;
	case RECLOWD:								// 雲リセット
		m_resetPtModel = std::move(model);
		break;
	default:
		break;
	}
}

/// <summary>
/// コイン取得処理
/// </summary>
/// <param name="index">当たったコインの番号</param>
/// <returns>なし</returns>
void Blocks::CountUpCoin(int index)
{
	m_coinCount++;
	m_mapObj[index].id = MAPSTATE::NONE;
}

/// <summary>
/// コインを回収判定
/// </summary>
/// <param name="引数無し"></param>
/// <returns>回収終わったらTrue</returns>
const bool& Blocks::IsCollectedFlag()
{
	return is_collectedFlag;
}

/// <summary>
/// サイズゲッター
/// </summary>
/// <param name="objName">オブジェクトの名前(構造体)</param>
/// <returns>なし</returns>
const float& Blocks::GetObjSize(const int& objName)

{
	if (objName == MAPSTATE::COIN)
	{
		// コインは小さめサイズ
		return COIN_SIZE;
	}
	else if (objName == MAPSTATE::CLOUD)
	{
		// 雲も小さめサイズ
		return CLOWD_SIZE;
	}
	else if (objName == MAPSTATE::RESET)
	{
		// 雲リセットエリアは大きめsサイズ
		return CLOWD_RESET_SIZE;
	}
	else
	{
		// デフォルトサイズ
		return COMMON_SIZE;
	}
}

/// <summary>
/// 雲の位置をもとに戻す処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Blocks::RestoreCloudPosition()
{

	for (auto& i : m_mapObj)
	{
		// 雲のみを対象とする
		if (i.id == MAPSTATE::CLOUD)
		{
			// Y座標を始点まで動かす
			i.position.y = UserUtility::Lerp
			(
				i.position.y,
				m_cloudState[i.index].initPosition.y,
				CLOWD_SPEED * 0.5f
			);
			m_cloudState[i.index].moveFlag = false;
		}
	}
}

/// <summary>
/// マップ選択
/// </summary>
/// <param name="num">ステージ番号</param>
/// <returns>ファイルパス</returns>
std::wstring Blocks::MapSelect(int num)
{
	std::wstring filePath;

	// TODO: [ステージ番号]マップ追加はここから！
	// マップの変更
	switch (num)
	{
	case 0:
		filePath = L"Resources/Maps/StageEdit.csv";
		break;
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	case 4:
		filePath = L"Resources/Maps/Stage4.csv";
		break;
	case 5:
		filePath = L"Resources/Maps/Stage5.csv";
		break;
	case 6:
		filePath = L"Resources/Maps/Stage6.csv";
		break;
	case 7:
		filePath = L"Resources/Maps/Stage7.csv";
		break;
	case 8:
		filePath = L"Resources/Maps/Stage8.csv";
		break;
	case 9:
		filePath = L"Resources/Maps/Stage9.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	return filePath;
}

/// <summary>
/// モデルの色を変更
/// </summary>
/// <param name="model">モデルデータ</param>
/// <param name="color">色情報</param>
/// <returns>なし</returns>
void Blocks::ChangeModelColors(std::unique_ptr<Model>& model, SimpleMath::Vector4 color)
{
	model->UpdateEffects([&](IEffect* effect)
		{
			// ベーシックエフェクトを宣言
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);

			if (basicEffect)
			{
				// 色を変更する
				basicEffect->SetDiffuseColor(color);
			}
		}
	);
}