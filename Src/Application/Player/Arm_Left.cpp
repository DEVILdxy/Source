
#include "../Headers.h"

Arm_Left::Arm_Left(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	
	m_mat = m_startMat = m_endMat = Math::Matrix::Identity;
	
	m_mat = m_startMat;
	/*Math::Matrix tmpMat = DirectX::XMMatrixTranslation(-0.4f, 0.6f, 0);
	m_mat = tmpMat * m_gameScene->GetPlayer().GetMat();*/
	m_mat = Math::Matrix::Identity;
	m_animeFrame = 0;
}

void Arm_Left::Update()
{		
	

	Math::Matrix transMat = DirectX::XMMatrixTranslation(-0.3f, -0.5f, 0);
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-90));
	m_mat = rotMatX * transMat * m_gameScene->GetPlayer().GetMat();


}


