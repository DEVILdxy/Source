#include"../Headers.h"

Ball::Ball(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/Ball.gltf");
	m_mat = DirectX::XMMatrixTranslation(3, 330, -101); // 0, 400, 0
	m_gameScene = _gameScene;
	m_moveVec = { 0.3f,0,0 };

	m_nowPos = {};
	m_oldPos = m_mat.Translation();
}

void Ball::Update()
{
	// 重力
	float gravity = -0.5f;
	m_mat._42 += gravity;

	m_oldPos = m_mat.Translation();
	Move();

	Move_Slope();


	Math::Vector3 moveVec = m_nowPos - m_oldPos;
	Roll(moveVec);


}

void Ball::Move_Slope()
{
	const Map& map = m_gameScene->GetMap();
	Math::Vector3 mapNormal;
	Math::Vector3 downVec = { 0,-1,0 };
	float hitDis;
	bool hit = map.CheckBoard(m_mat.Translation(), downVec, hitDis, mapNormal);
	if (hit) // 下に飛ばすレイとmap当たったら
	{
		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(mapNormal, -downVec);
		constexpr float awayDistance = 3.0f;

		float limit;
		limit = awayDistance / dot.x;
		//立ち入り禁止エリアの幅/コサインθ＝立ち止まるべき距離
		if (limit < 0) //壁の裏側から侵入した場合
		{
			limit *= -1;//距離をプラスの値に変換
		}

		//Math::Vector3 moveVec;
		if (hitDis < limit)  //壁までの距離が「立ち止まるべき距離」より短い
		{
			m_mat._41 += (mapNormal * ((limit - hitDis) * dot.x)).x;
			m_mat._42 += (mapNormal * ((limit - hitDis) * dot.x)).y;
			m_mat._43 += (mapNormal * ((limit - hitDis) * dot.x)).z;
			//法線方向に適切な距離を押し返す
			m_nowPos = m_mat.Translation();
		}
	}
}

void Ball::Roll(Math::Vector3& _dir)
{
	constexpr float ballDiameter = 6.0f;// ボールの直径の長さ
	float ang = (_dir.Length() / (ballDiameter * M_PI) * 360); // 回転角度＝進行長さとボールの周長の比率と360°のかける結果

	Math::Vector3 wRotAxis = DirectX::XMVector3Cross(_dir, Math::Vector3(0, 1, 0));// ワールドの回転軸	

	if (wRotAxis != Math::Vector3::Zero)
	{
		Math::Matrix invMat;
		invMat = DirectX::XMMatrixInverse(nullptr, m_mat); // 逆行列
		Math::Vector3 lRotAxis = DirectX::XMVector3TransformNormal(wRotAxis, invMat); // ローカルの回転軸

		Math::Matrix rotMat = DirectX::XMMatrixRotationAxis(lRotAxis, DirectX::XMConvertToRadians(-ang));

		m_mat = rotMat * m_mat;
	}
}

void Ball::Move()
{
	if (m_moveVec != Math::Vector3::Zero)
	{
		Math::Vector3 rayVec = m_moveVec;
		rayVec.Normalize();
		const Wall_Reflect& WR = m_gameScene->GetWallReflect();
		Math::Vector3 mapNormal;
		float hitDis;
		bool hit = WR.CheckBoard(m_mat.Translation(), rayVec, hitDis, mapNormal);
		if (hit) // 進行方向に障害物がある場合
		{
			float moveLen;
			moveLen = m_moveVec.Length();

			Math::Vector3 dot;
			dot = DirectX::XMVector3Dot(mapNormal, -rayVec);

			float limit;
			limit = 3.0f / dot.x;// 弾の半径がめり込まないための限界距離
			if (limit < 0)
			{
				limit *= -1;
			}

			if ((hitDis - limit) <= moveLen)// 障害物までの距離が予定移動量より短い
			{
				Math::Vector3 vec;
				vec = rayVec * (hitDis - limit);

				Move(vec);

				m_moveVec += mapNormal * (moveLen * dot.x) * 2;
			
				// 新moveVec = 旧moveVec + 衝突面の法線 x 直角三角形の底辺の長さ x 2
			}
			else
			{
				Move(m_moveVec);// 通常移動
			}
		}
		else
		{
			Move(m_moveVec);// 通常移動
		}
	}

}

void Ball::Move(const Math::Vector3& _vec)
{
	m_mat._41 += _vec.x;
	//m_mat._42 += _vec.y;
	m_mat._43 += _vec.z;
}

void Ball::SetPos(Math::Vector3 _pos)
{
	m_mat._41 = _pos.x;
	m_mat._42 = _pos.y;
	m_mat._43 = _pos.z;
}


