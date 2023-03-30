#include"../Headers.h"

Enemy::Enemy(GameScene* _gameScene, Math::Vector3 _pos)
{
	m_gameScene = _gameScene;
	m_modelDraw = MODELMANAGER.GetModel("Data/Models/Enemy/Enemy.gltf");
	m_modelUpdate = MODELMANAGER.GetModel("Data/Models/Enemy/Enemy_Update.gltf");
	m_aliveFlg = true;
	const Map& map = m_gameScene->GetMap();
	float dist = -1;
	map.CheckBoard(_pos, Math::Vector3{ 0,-1,0 }, dist);

	const float modelHeight = 1.0f;

	float y = _pos.y - dist + modelHeight;
	m_mat = DirectX::XMMatrixTranslation(_pos.x, y, _pos.z);


	m_currentState = new Appear(m_gameScene, this, m_mat);

	m_aimCone = new AimCone(this);

}

Enemy::~Enemy()
{

	if (m_currentState != nullptr)
	{
		delete m_currentState;
	}
}

void Enemy::Update()
{
	float gravity = -0.3f;
	//重力
	m_mat._42 += gravity;
	if (m_mat._42 <= m_nowGround)
	{
		m_mat._42 = m_nowGround;
		gravity = 0;
	}


	Math::Vector3 vecFront, vecBack, vecLeft, vecRight, vecUp;
	vecFront = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), m_mat);
	vecBack = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -1), m_mat);
	vecLeft = DirectX::XMVector3TransformNormal(Math::Vector3(-1, 0, 0), m_mat);
	vecRight = DirectX::XMVector3TransformNormal(Math::Vector3(1, 0, 0), m_mat);

	CheckHit(vecFront);
	CheckHit(vecBack);
	CheckHit(vecLeft);
	CheckHit(vecRight);

	Check_Ground();

	m_aimCone->Update();

	if (m_currentState != nullptr)
	{
		BaseState* nextState;
		nextState = m_currentState->Action(m_gameScene, m_mat);

		//別のステートへの移行希望が返ってきたら
		if (nextState != nullptr)
		{
			//現在ステートを削除
			delete m_currentState;

			//移行先を現在のステートとする
			m_currentState = nextState;
		}
	}




}


void Enemy::Draw()
{
	SHADER.m_standardShader.SetAppearEnable(true);

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_modelDraw);

	SHADER.m_standardShader.SetAppearEnable(false);

	if (m_gameScene->GetPlayer().GetEnemySpotFlg() == true && m_gameScene->GetUI().GetLimit() == true)
	{
		m_aimCone->Draw();
	}


}


bool Enemy::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_modelUpdate, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

void Enemy::Check_Ground()
{
	const Map& map = m_gameScene->GetMap();
	float groundHitDis;
	if (map.CheckBoard(m_mat.Translation(), Math::Vector3(0, -1, 0), groundHitDis))
	{
		m_nowGround = m_mat._42 + (1.0f - groundHitDis);
	}
}

void Enemy::CheckHit(Math::Vector3 _vec)
{
	const Map& map = m_gameScene->GetMap();
	Math::Vector3 mapNormal;
	bool mapHit;//レイがあった
	mapHit = map.CheckBoard(m_mat.Translation(), _vec, m_hitDis, mapNormal);
	PushBack(mapHit,mapNormal, _vec);

	const Obstacle& obstacle = m_gameScene->GetObstacle();
	Math::Vector3 obsNormal;
	bool obsHit;
	obsHit = obstacle.CheckBoard(m_mat.Translation(), _vec, m_hitDis, obsNormal);
	PushBack(obsHit,obsNormal, _vec);
}

void Enemy::PushBack(bool _hit,Math::Vector3& _normal, Math::Vector3 _vec)
{
	if (_hit) // 進路上に壁がある
	{

		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(_normal, -_vec);

		// マジックナンバー　/直値 /即値
		float limit;
		limit = 3.0f / dot.x;
		// 立ち入り禁止エリアの幅/コサインθ＝立ち止まるべき距離
		if (limit < 0) //壁の裏側から侵入した場合
		{
			limit *= -1;//距離をプラスの値に変換
		}
		if (m_hitDis < limit)  //壁までの距離が「立ち止まるべき距離」より短い
		{
			m_mat._41 += (_normal * ((limit - m_hitDis) * dot.x)).x;
			m_mat._42 += (_normal * ((limit - m_hitDis) * dot.x)).y;
			m_mat._43 += (_normal * ((limit - m_hitDis) * dot.x)).z;
			//法線方向に適切な距離を押し返す
		}
	}

}

void Enemy::AddPos(Math::Vector3 _addPos)
{
	m_mat._41 += _addPos.x;
	m_mat._42 += _addPos.y;
	m_mat._43 += _addPos.z;
}

