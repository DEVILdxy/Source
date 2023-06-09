#include"../Headers.h"

Arm_Right::Arm_Right(GameScene* _gameScene)
{
	m_gameScene = _gameScene;
}

void Arm_Right::Update()
{
	Player& player = m_gameScene->GetPlayer();
	//照準している======================================
	Math::Matrix rotMatZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(30));

	Math::Matrix transMat = DirectX::XMMatrixTranslation(0.3f, -0.5f, 0);
	m_mat = rotMatZ * transMat * m_gameScene->GetPlayer().GetMat();
	//==================================================

	if (player.GetAimState() == true)
	{
		Math::Matrix tmpMat;

		tmpMat = DirectX::XMMatrixLookAtLH(     //at
			m_mat.Translation(),		  //位置	
			player.GetRayHitPos(),		  //向く方向
			Math::Vector3(0, 1, 0));	  //頭上方向

		tmpMat = DirectX::XMMatrixInverse(nullptr, tmpMat);

		Math::Matrix rotMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-90));

		m_mat = rotMat * tmpMat;
	}


}
