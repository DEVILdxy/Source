#include "../Headers.h"

Hand::Hand(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Characters/Hand.gltf");
	m_mat = Math::Matrix::Identity;
	m_gameScene = _gameScene;
	m_isShoot = false;
	m_mapHit = false;
	m_enemyHit = false;
	m_PFMhit = false;
	m_PFIhit = false;

	//chain===============
	m_chainModel = MODELMANAGER.GetModel("Data/Models/Characters/Chain.gltf");
	for (int i = 0; i < 50; i++)
	{
		m_chainMat[i] = Math::Matrix::Identity;
	}

	//fire=================
	m_fireTex = TEXTUREMANAGER.GetTexture("Data/Texture/Smoke.png");
	m_fireMat = Math::Matrix::Identity;
	m_anime = 0;

	//サウンド
	m_isSoundPlay = false;
	m_isTimeKeep = false;
	m_timer = 0;
	m_sound = std::make_shared<KdSoundEffect>();
	m_sound->Load("Data/Sound/Shoot.wav");
	m_soundInst = m_sound->CreateInstance(true);
	m_soundInst->SetVolume(1);
}


Hand::~Hand()
{
	m_fireVb.Release();
}

void Hand::Update()
{
	if (m_gameScene->GetPlayer().GetAimState() == true) // 照準する
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			SetShootState(true);
			Math::Vector3 firePos, tmpPos;
			tmpPos = { 0.3,0,0 };
			firePos = m_gameScene->GetRightArm().GetMat().Translation();
			m_gameScene->GetEffectManager().Add(new ShootFire(firePos));
			m_isTimeKeep = true;
		}
	}
	if (m_isTimeKeep == true)
	{
		m_timer++;
		if (m_isSoundPlay != true)
		{
			m_isSoundPlay = true;
			m_soundInst->Play();
		}		
	}
	if (m_timer >= 60)
	{
		m_timer = 0;
		m_isSoundPlay = false;
		m_isTimeKeep = false;
		m_soundInst->Pause();
	}


	if (m_isShoot == true) //発射したら
	{
		Move();
		//Math::Vector3 rayVec = DirectX::XMVector3Transform(Math::Vector3(0, 0, 1), m_mat);
		Math::Vector3 rayVec = m_gameScene->GetPlayer().GetLookVec();
		CheckHit(rayVec); // レイ判定	
		
	}
	else				// 発射しない
	{
		m_moveVec = { 0,0 ,0 };
		Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
		Math::Matrix transMat = DirectX::XMMatrixTranslation(0, -0.5, 0);
		m_mat = rotMatX * transMat * m_gameScene->GetRightArm().GetMat();				//プレイヤーと一緒に移動
	}

	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_moveVec.x, m_moveVec.y, m_moveVec.z);
	m_mat = transMat * m_mat;				//移動合成

	Update_Line();
}

void Hand::Update_Line()
{

	Math::Matrix transMat[50];
	v_hand_rightArm = m_mat.Translation() - m_gameScene->GetRightArm().GetMat().Translation();

	for (int i = 0; i < 50; i++)

	{
		transMat[i] = DirectX::XMMatrixTranslation(0, 0, -i / 1.5f);
		m_chainMat[i] = transMat[i] * m_mat;
	}

}


void Hand::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	float length = v_hand_rightArm.Length();  //手と腕の距離
	if (length >= 32)
	{
		length = 32;
		SetShootState(false);
	}
	for (int i = 0; i < (int)length * 1.5; i++)
	{
		SHADER.m_standardShader.SetWorldMatrix(m_chainMat[i]);
		SHADER.m_standardShader.DrawModel(m_chainModel);
	}

}

void Hand::Draw_Chain(float _length)
{
}


void Hand::Draw_Frie()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//使ってほしい頂点バッファ
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_fireVb.GetAddress(), &stride, &offset);
	//頂点の利用法(プリミティブトポロジー)をセット 原始拓扑
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャをセット							　　
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_fireTex->GetSRViewAddress());
	//---------------------------
	//板ボリ表示
	SHADER.m_effectShader.SetWorldMatrix(m_fireMat);
	SHADER.m_effectShader.WriteToCB();//今までに設定した内容を反映
	//描画					  ↓何個使うか？
	D3D.GetDevContext()->Draw(4, 0);
}

void Hand::CheckHit(Math::Vector3 _vec)
{
	//float minDist = FLT_MAX;

	const Map& map = m_gameScene->GetMap();
	bool mapHit = map.CheckBoard(m_mat.Translation(), _vec, m_hitMapDis);
	if (m_hitMapDis <= PlayerSetting::ToObjectDist) // とmapの距離	
	{
		m_moveVec = { 0,0,0 };
		m_mapHit = true;
	}

	std::list<Enemy*>& enemyList = m_gameScene->GetEnemyManager().GetEnemyList();
	for (auto enemy : enemyList)
	{
		Math::Vector3 toEnemyVec = enemy->GetMat().Translation() - m_mat.Translation();
		if (toEnemyVec.Length() <= PlayerSetting::ToObjectDist)
		{
			m_moveVec = { 0,0,0 };
			m_enemyHit = true;
			break;
		}
	}

	const PlatFormUpstanding& pFM = m_gameScene->GetPlatFormMove();
	bool pFMHit = pFM.CheckBoard(m_mat.Translation(), _vec, m_hitMapDis);
	if (m_hitMapDis <= PlayerSetting::ToObjectDist)
	{
		m_moveVec = { 0,0,0 };
		m_PFMhit = true;
	}

	bool pFIHit = m_gameScene->GetPFI().CheckBoard(m_mat.Translation(), _vec, m_hitMapDis);
	if (m_hitMapDis <= PlayerSetting::ToObjectDist)
	{
		m_moveVec = { 0,0,0 };
		m_PFIhit = true;
	}
}

void Hand::Move()
{
	m_moveVec = { 0,0 ,0.8f };
}

