#include"../Headers.h"

Attack::Attack()
{
	m_movePow = 0.8f;
	m_returnTrackTime = 60;
	m_pushPow = 0;
}

BaseState* Attack::Action(GameScene* _gameScene, Math::Matrix& _mat)
{
	Math::Vector3 targetVec = _gameScene->GetPlayer().GetPos() - _mat.Translation();


	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 0, m_movePow);
	_mat = transMat * _mat;
	m_returnTrackTime--;
	if (m_returnTrackTime <= 0)
	{
		return new Track();
	}


	Hand& hand = _gameScene->GetHand();

	Math::Vector3 toHandVec = hand.GetMat().Translation() - _mat.Translation();
	const float toHandDist = 1.0f;

	if (hand.GetShootState() == true && toHandVec.Length() <= toHandDist)
	{
		return new BeAttacked();
	}
	pushPlayer(_gameScene, _mat.Translation());

	return nullptr;
}

void Attack::SetTargetPos(Math::Vector3 _pos)
{

}

void Attack::pushPlayer(GameScene* _gameScene, Math::Vector3 _pos)
{
	Player& player = _gameScene->GetPlayer();
	Math::Vector3 v = player.GetPos() - _pos;
	if (v.Length() <= 2.0f)
	{	//“–‚½‚Á‚Ä‚é		
		m_pushPow = 2.0f;
	}
	else
	{
		m_pushPow -= 0.1f;
		if (m_pushPow <= 0)
		{
			m_pushPow = 0;
		}
	}
	v.y = 0;
	v.Normalize();
	v *= m_pushPow;
	player.AddPos(v);
}
