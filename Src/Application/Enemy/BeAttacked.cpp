#include "../Headers.h"

BeAttacked::BeAttacked()
{
	m_hitDis = 0;
	m_returnTrackTime = 60;
	m_hitFlg = false;
}

BaseState* BeAttacked::Action(GameScene* _gameScene, Math::Matrix& _mat)
{
	// ˆÚ“®‚³‚¹‚Ü‚¹‚ñ
	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 0, 0);
	_mat = transMat * _mat;

	// ‰Ÿ‚³‚ê‚éŽž
	Math::Vector3 fromPlayerPow = _mat.Translation() - _gameScene->GetPlayer().GetMat().Translation();

	m_returnTrackTime--;

	if (m_returnTrackTime <= 0)
	{
		return new Track();
	}

	if (fromPlayerPow.Length() <= 1.5f)
	{
		m_pushPow = 3.0f;
	}
	else
	{
		m_pushPow -= 0.2f;
		if (m_pushPow <= 0)
		{
			m_pushPow = 0;
		}
	}
	fromPlayerPow.y = 0;
	fromPlayerPow.Normalize();
	fromPlayerPow *= m_pushPow;
	AddEnemyPos(fromPlayerPow, _mat);

	//// ‚ÆáŠQ•¨‚ð“–‚½‚éŽž
	//Math::Vector3 vec = _mat.Translation() - m_gameScene->GetObstacle().GetMat().Translation();
	//if (vec.Length() < 10)
	//{
	//	if (m_hitFlg == false)
	//	{
	//		m_hitFlg = true;
	//		m_gameScene->GetObstacle().Hit();
	//	}
	//}
	//else
	//{
	//	m_hitFlg = false;
	//}





	return nullptr;
}

void BeAttacked::AddEnemyPos(Math::Vector3 _addPos, Math::Matrix& _mat)
{
	_mat._41 += _addPos.x;

	_mat._42 += _addPos.y;

	_mat._43 += _addPos.z;
}
