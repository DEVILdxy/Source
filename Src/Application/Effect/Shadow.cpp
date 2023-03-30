#include"../Headers.h"

Shadow::Shadow(GameScene* _gameScene,float _size)
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/Shadow.png");
	// 板ボリ初期化
	m_v[0].uv = { 0,0 };
	m_v[1].uv = { 1,0 };
	m_v[2].uv = { 0,1 };
	m_v[3].uv = { 1,1 };

	

	m_v[0].pos = { -_size,_size,0 };
	m_v[1].pos = { _size,_size,0 };
	m_v[2].pos = { -_size,-_size,0 };
	m_v[3].pos = { _size,-_size,0 };

	

	m_shadowPosY = FLT_MAX;
	m_minDist = FLT_MAX;

	m_gameScene = _gameScene;
}

Shadow::~Shadow()
{


}

void Shadow::Update(Math::Vector3 _objPos)
{
	m_shadowPosY = DecideHeight(_objPos);

	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix transMat = DirectX::XMMatrixTranslation(_objPos.x, m_shadowPosY, _objPos.z);
	m_mat =rotMatX * transMat;

	float shadowAlpha = 1 / m_minDist;
	m_v[0].color = { 1,1,1,shadowAlpha };
	m_v[1].color = { 1,1,1,shadowAlpha };
	m_v[2].color = { 1,1,1,shadowAlpha };
	m_v[3].color = { 1,1,1,shadowAlpha };

	D3D11_SUBRESOURCE_DATA srd = {};//ゼロクリア(中身全てゼロ)
	srd.pSysMem = m_v;//&v[0]　頂点データが入っている先頭アドレス
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(m_v), D3D11_USAGE_DEFAULT, &srd);

}

float Shadow::DecideHeight(Math::Vector3 _objPos)
{
	m_minDist = FLT_MAX;

	float hitDis;

	float height;

	const Map& map = m_gameScene->GetMap();
	if (map.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	const Bridge& bridgeBack = m_gameScene->GetBridgeBack();
	if (bridgeBack.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	const Bridge& bridgeFront = m_gameScene->GetBridgeFront();
	if (bridgeFront.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	if (m_gameScene->GetPFI().CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}


	if (m_gameScene->GetPFS().CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (hitDis <= PlayerSetting::height)
		{
			m_gameScene->GetPFS().MoveTogether();
		}
		else
		{
			m_gameScene->GetPFS().SetIsOnFlg(false);
		}


		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	Wheel& whell_0 = m_gameScene->GetWhellDown();
	if (whell_0.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (hitDis <= PlayerSetting::height)
		{
			whell_0.MoveTogether();
		}
		else
		{
			whell_0.SetOnWheelFlg(false);
		}

		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	Wheel& whell_1 = m_gameScene->GetWhellUp();
	if (whell_1.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (hitDis <= PlayerSetting::height)
		{
			whell_1.MoveTogether();
		}
		else
		{
			whell_1.SetOnWheelFlg(false);
		}

		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	Gear& gear = m_gameScene->GetGear();
	if (gear.CheckBoard(_objPos, Math::Vector3(0, -1, 0), hitDis))
	{
		if (hitDis <= PlayerSetting::height)
		{
			gear.MoveTogether();
		}
		else
		{
			gear.SetOnGearFlg(false);
		}

		if (m_minDist > hitDis)
		{
			m_minDist = hitDis;
		}
	}

	if (m_minDist != FLT_MAX)
	{
		height = _objPos.y  - m_minDist+0.01f; // 影のワールド座標を決める
	}

	return height;
}
