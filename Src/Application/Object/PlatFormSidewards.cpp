#include"../Headers.h"

PlatFormSidewards::PlatFormSidewards(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/PlatFormMove.gltf");
	m_startPos = { -10,163,160 };
	m_endPos = { -10,248,160 };
	m_pos = m_startPos;
	m_savePos = {};

	m_speed = 0;
	m_distance = (m_endPos - m_startPos).Length();// 全体の距離

	m_gameScene = _gameScene;
}

PlatFormSidewards::~PlatFormSidewards()
{

}

void PlatFormSidewards::Update()
{

	Math::Vector3 vec;
	vec = m_endPos - m_pos;

	float len;
	len = vec.Length();

	// もうゴールまで間近
	if ((len <= 0.1f))
	{
		// ゴールに到着
		m_pos = m_endPos;

		// スタートとゴールを入れ替え
		Math::Vector3 tmpPos;
		tmpPos = m_endPos;
		m_endPos = m_startPos;
		m_startPos = tmpPos;

		// 速度をゼロになる
		m_speed = 0;
	}
	else   // 加速ー＞等速ー＞減速
	{
		if (len < m_distance * 0.3f) // 移動終盤
		{
			m_speed = len * 0.02f;// 減速
		}
		else //　序盤
		{
			if (len > m_distance * 0.8f)
			{
				m_speed += 0.01f;// 加速
			}

		}

		vec.Normalize();
		m_pos += vec * m_speed;
	}

	Math::Matrix rotMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = rotMat * transMat;
}

void PlatFormSidewards::MoveTogether()
{
	if (m_isOnFlg == false)
	{
		m_isOnFlg = true;
		m_savePos = m_mat.Translation();
	}
	else
	{
		Math::Vector3 plusPos = m_mat.Translation() - m_savePos;

		m_gameScene->GetPlayer().AddPos(plusPos);

		m_savePos =  m_mat.Translation();
		
	}
}

bool PlatFormSidewards::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}
