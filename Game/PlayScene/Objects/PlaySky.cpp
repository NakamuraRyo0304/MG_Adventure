/*
 *	@File	PlaySky.h
 *	@Brief	プレイシーンのスカイドーム。
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "PlaySky.h"

// コンストラクタ
PlaySky::PlaySky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
{
	fm->BuildModelFactory();
	m_model = fm->VisitModelFactory()->GetCreateModel(path);
	fm->LeaveModelFactory();

	m_model->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, false);
				_lights->SetLightEnabled(2, false);
			}
			// 自己発光する
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
	// 色を初期化
	m_skyColor = Color::Color();
}

// デストラクタ
PlaySky::~PlaySky()
{
}

// 更新関数
void PlaySky::Update(const float& gameTimer)
{
	m_skyColor.red = gameTimer;
	m_skyColor.green = gameTimer;
	m_skyColor.blue = gameTimer;
}

// 描画関数
void PlaySky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(timer * SKY_ROT_SPEED);
	m_model->Draw(_context, states, skyMat, view, proj);
	m_model->UpdateEffects([&](IEffect* effect)
		{
			// 色を徐々に暗くする
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// 徐々に暗くなっていく
				_basicEffect->SetEmissiveColor(
					SimpleMath::Color{
						m_skyColor.red,		 // 赤
						m_skyColor.green,	 // 緑
						m_skyColor.blue,	 // 青
						1.0f				 // 透明度
					}
				);
			}
		}
	);
}