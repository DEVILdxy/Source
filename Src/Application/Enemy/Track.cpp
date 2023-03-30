#include"../Headers.h"

Track::Track()
{
	m_attackPrepareTime = 30;
}

BaseState* Track::Action(GameScene* _gameScene, Math::Matrix& _mat)
{
	Player& player = _gameScene->GetPlayer();
	Hand& hand = _gameScene->GetHand();
	//ålŒö‚Ì•ûŒü‚É‚R‚¸‚ÂU‚è•Ô‚è
	//U‚èŒü‚«I‚í‚Á‚½‚ç0.2‚¸‚Â‘Oi‚·‚é
	Math::Vector3 targetVec = player.GetPos() - _mat.Translation();
	float  targetAng;
	targetAng = GetVecAngY(targetVec);
	if (TurnToAng(_mat, targetAng, 3.0f) == 0)
	{
		Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 0, 0.3f);
		_mat = transMat * _mat;

		targetVec.y = 0;
		if (targetVec.Length() <= 30)
		{
			m_attackPrepareTime--;
			if (m_attackPrepareTime <= 0)
			{
				return new Attack();
			}
		}
	}
	

	Math::Vector3 toHandVec = hand.GetMat().Translation() - _mat.Translation();
	const float toHandDist = 1.0f;

	if (hand.GetShootState() == true && toHandVec.Length() <= toHandDist)
	{
		return new BeAttacked();
	}


	return nullptr;

	
}
