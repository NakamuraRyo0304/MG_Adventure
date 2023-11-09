/*
 *	@File	FileName.h
 *	@Brief	処理内容。
 *	@Date	2023-00-00
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Logo.h"

// コンストラクタ
Logo::Logo(std::shared_ptr<FactoryManager> factory, const wchar_t* path)
	: ITitleObject()
	, m_position{}			// 座標
	, m_logoMoveScale{}		// 拡大率
	, m_logoMoveY{}			// 移動量
	, is_startFlag{ false } // スタートしたかどうか
	, is_endFlag{ false }   // 目的地を越えたらＵＩを表示する
	, is_nextFlag{ false }  // 次のシーンに行くフラグ
{
	// 位置情報など初期化
	m_position = { 0.0f,0.1f,0.0f };
	m_logoMoveY = 10.0f;
	m_logoMoveScale = 1.0f;

	auto& _mf = factory;
	_mf->BuildModelFactory();

	auto _model = _mf->VisitModelFactory()->GetCreateModel(path);

	_mf->LeaveModelFactory();

	m_model = std::move(_model);
	m_model->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ライトの指定
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, false);

				// ライトの方向を設定
				SimpleMath::Vector3 _lightDir = -SimpleMath::Vector3::UnitZ;
				_lights->SetLightDirection(0, _lightDir);
				_lights->SetLightDirection(1, _lightDir);
			}
		}
	);
}

// デストラクタ
Logo::~Logo()
{
}

// 更新処理
void Logo::Update()
{
	// 起動時のロゴの動き
	m_logoMoveY = UserUtility::Lerp(m_logoMoveY, END_MOVE_POS, 0.1f);

	// 目的地を越えたらTrueにする
	if (static_cast<int>(m_logoMoveY) == static_cast<int>(END_MOVE_POS))
	{
		is_endFlag = true;
	}

	// スタート演出
	if (is_startFlag)
	{
		m_position.y++;

		// 演出が終わったら遷移
		if (m_position.y > MAX_HEIGHT)
		{
			is_nextFlag = true;
		}
	}
}

// 描画処理
void Logo::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	float _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// 行列を計算
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix _rotMat = SimpleMath::Matrix::CreateRotationX(sinf(_timer) * 0.5f);
	SimpleMath::Matrix _transMat = SimpleMath::Matrix::CreateTranslation(0.0f, m_logoMoveY, cosf(_timer) * 0.5f);

	// ロゴも一緒に動く
	if (is_startFlag)
	{
		m_logoMoveScale *= LOGO_CHANGE_SCALE;
		_world *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// 結果を反映
	_world *= _rotMat * _transMat;

	m_model->Draw(_context, states, _world, view, proj);
}

// 終了処理
void Logo::Finalize()
{
	m_model.reset();
}
