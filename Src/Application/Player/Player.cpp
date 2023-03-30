#include"../Headers.h"



enum MovieState
{
	Movie1,
	Movie2,
	Movie3
};

Player::Player(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Characters/Player.gltf");
	m_mat = Math::Matrix::Identity;	//mat初期化
	m_startMat = m_endMat = m_mat;  //アニメ初期化
	m_gameScene = _gameScene;
	m_worldPos = { 1,2.0,-280 };  //1,2.0,-280
	m_groundHitDis = FLT_MAX;
	m_hitDis = FLT_MAX;

	// PlatFormMove用
	m_PlatformUpHitDis = FLT_MAX;

	m_camLook = {};
	//角度初期化
	m_ang = 0;

	m_moveVec = {}; // 移動ベクトル
	m_animeAng = 0;
	m_gravity = 0;//重力初期化
	m_isAimming = false;//照準フラグ初期化
	m_isJumping = false;
	m_isMovie = true;    // 
	m_rButtonState = false;
	m_enemySpotted = false;


	m_movieState = MovieState::Movie1;

	m_stateFlg = PlayerState::Normal;//状態フラグ初期化
	m_camState = CamState::Normal;
	m_camMove = { 0, 2, -5.5f };


	m_aimShow = false;

	m_camNextMat = Math::Matrix::Identity;
	m_camPreviousMat = Math::Matrix::Identity;
	m_camStartMat = Math::Matrix::Identity;
	m_camNowMat = Math::Matrix::Identity;
	m_camEndMat = Math::Matrix::Identity;
	m_camFrame = 0;

	m_loadedMap = 0;// 今使っているマップ

	m_hammerVec = {};

	//マオス====================
	m_camAngX = 0;
	m_camAngY = 0;
	//ゲーム画面の中央(クライアント座標)
	m_basePt.x = ScrW / 2;
	m_basePt.y = ScrH / 2;
	//クライアント座標をスクリーン座標に修正
	ClientToScreen(APP.m_window.GetWndHandle(), &m_basePt);
	//マウスを画面中央に移動　
	SetCursorPos(m_basePt.x, m_basePt.y);
	//マウスカーソルを非表示
	ShowCursor(FALSE);

	const float shadowSize = 0.5f;
	m_shadow = std::make_shared<Shadow>(_gameScene, shadowSize);

	m_aimRayDis = 0;

	m_savePos = m_worldPos;

	//サウンド
	m_isSoundMovePlay = false;
	m_soundMove = std::make_shared<KdSoundEffect>();
	m_soundMove->Load("Data/Sound/Walk.wav");
	m_soundInstMove = m_soundMove->CreateInstance(true);
	m_soundInstMove->SetVolume(1);

}

Player::~Player()
{

	//マウスカーソルを再表示
	ShowCursor(TRUE);
	m_soundInstMove->Pause();
}

//void Player::PosInit(int _loadmap)
//{
//
//	if (_loadmap != m_loadedMap)
//	{
//		m_worldPos = { 0,2,0 };
//		m_loadedMap = _loadmap;
//	}
//}



void Player::Update()
{

	Update_Character();

	if (m_isMovie == true)
	{
		Update_Movie();
	}

	m_shadow->Update(m_worldPos);
}


void Player::Update_Character()
{
	//主人公------------------
	//Map& map = m_gameScene->GetMap();
	//int mapState = map.GetLoadedMap();
	//PosInit(mapState);

	// 右キー切り替え照準==========================
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (m_rButtonState == false && m_isAimming == false)
		{
			m_rButtonState = true;
			SetAimmingFlg(true);
			m_stateFlg = PlayerState::Aim;
		}
	}
	else {
		m_rButtonState = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (m_rButtonState == false && m_isAimming == true)
		{
			m_rButtonState = true;
			SetAimmingFlg(false);
			m_stateFlg = PlayerState::Normal;
		}
	}
	else {
		m_rButtonState = false;
	}
	// ================================================

	if (m_gameScene->GetHand().GetMapHitState() == true)  // 手とmapあたら
	{
		SetCamState(CamState::Normal);
		m_stateFlg = PlayerState::Approach;
	}

	if (m_gameScene->GetHand().GetEnemyHitState() == true) // 手とenemyあたら
	{
		SetCamState(CamState::Normal);
		m_stateFlg = PlayerState::Attack;
	}

	if (m_gameScene->GetHand().GetPlatFormMoveHitState() == true)  // 手とplatformmoveあたら
	{
		SetCamState(CamState::Normal);
		m_stateFlg = PlayerState::Approach;
	}

	if (m_gameScene->GetHand().GetPlatFormInvisibleHitState()==true)  // 手とplatformmoveあたら
	{
		SetCamState(CamState::Normal);
		m_stateFlg = PlayerState::Approach;
	}


	// 

	//マウスカーンルの現在位置を取得
	if (GetAsyncKeyState('0') & 0x8000)
	{
		m_mouseFlg = true;
		ShowCursor(TRUE);
	}
	else
	{
		m_mouseFlg = false;
		ShowCursor(FALSE);
	}

	if (m_mouseFlg == false)
	{
		POINT currentPt;
		GetCursorPos(&currentPt);

		m_camAngY += (currentPt.x - m_basePt.x) / 4.0f;
		m_camAngX += (currentPt.y - m_basePt.y) / 4.0f * 1;  //リバース　 

		SetCursorPos(m_basePt.x, m_basePt.y);
	}


	//X軸回転カメラ制御
	if (m_camAngX > 60)m_camAngX = 60;
	if (m_camAngX < -90)m_camAngX = -90;


	switch (m_stateFlg)
	{
	case PlayerState::Normal:
		Update_Normal();
		break;
	case PlayerState::Aim:
		Update_TPS();
		break;
	case PlayerState::Approach:
		Update_Approach();//
		break;
	case PlayerState::Stick:
		Update_Stick();
		break;
	case PlayerState::Attack:
		Update_Attack();
		break;

	default:
		break;
	}

	//カメラ---------------------------------
	if (m_stateFlg == PlayerState::Normal || m_stateFlg == PlayerState::Stick || m_stateFlg == PlayerState::Approach)
	{
		SetCamState(CamState::Normal);
	}
	if (m_stateFlg == PlayerState::Aim)
	{
		SetCamState(CamState::Aim);
	}

	


	//レイとモデルの当たり判定	
	Math::Vector3 vecFront, vecBack, vecLeft, vecRight, vecUp;
	vecFront = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), m_mat);
	vecBack = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -1), m_mat);
	vecLeft = DirectX::XMVector3TransformNormal(Math::Vector3(-1, 0, 0), m_mat);
	vecRight = DirectX::XMVector3TransformNormal(Math::Vector3(1, 0, 0), m_mat);
	vecUp = DirectX::XMVector3TransformNormal(Math::Vector3(0, 1, 0), m_mat);

	CheckHit(vecFront);
	//CheckHit(vecBack);
	//CheckHit(vecLeft);
	//CheckHit(vecRight);
	//CheckHit(vecUp);

	CheckHit_Ground();

	CheckHit_Ray();

}

void Player::Update_Normal()					//普通状態
{

	//重力
	m_worldPos.y += m_gravity;
	m_gravity -= 0.03f;
	if (m_worldPos.y <= m_nowGround)
	{
		m_worldPos.y = m_nowGround;
		m_gravity = 0;
		m_isJumping = false;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_isJumping == false)
		{
			m_gravity = 0.6f;
			m_isJumping = true;
		}
	}

	bool moveFlg = false;
	//m_soundInstMove->Pause();

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, PlayerSetting::movePow), camRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_moveVec -= DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, PlayerSetting::movePow), camRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_moveVec -= DirectX::XMVector3TransformNormal(Math::Vector3(PlayerSetting::movePow, 0, 0), camRotMat);
		moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_moveVec += DirectX::XMVector3TransformNormal(Math::Vector3(PlayerSetting::movePow, 0, 0), camRotMat);
		moveFlg = true;
	}



	if (moveFlg == true)
	{
		// サウンド
		if (m_isSoundMovePlay != true)
		{
			m_soundInstMove->Play(true);
			m_isSoundMovePlay = true;
		}

		//移動している表現

		//正規化
		m_moveVec.Normalize();
		m_moveVec *= PlayerSetting::movePow;//長さ１の値に本来の移動量を掛ける

		m_worldPos += m_moveVec;

		//キャラの向きをカメラの向きに変える

		//今キャラが向いている方向
		//①キャラの回転行列を作成
		Math::Matrix nowRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
		//②現在の方向（ベクトル）を変形できる
		Math::Vector3 nowVec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), nowRotMat);

		//向きたい方向
		Math::Vector3 toVec = m_moveVec;//移動したい方向の移動量
		toVec.Normalize();//長さ１のベクトル（移動したい方向）

		//角度を求める(内積を使う)
		//ベクトルAとベクトルBとコサインなす角
		Math::Vector3 dot = DirectX::XMVector3Dot(nowVec, toVec); //この処理でcos角度が入る（両ベクトルの長さが１だから)
		//丸め誤差の都合上[1]を超える可能性があるからクランプ(遮断)する
		//＊丸め誤差（小数点以下を省略した際に生じる誤差)
		if (dot.x > 1)
		{
			dot.x = 1.0f;
		}
		if (dot.x < -1)
		{
			dot.x = -1.0f;
		}

		//角度を取れる（ラジアン角）
		float ang = DirectX::XMConvertToDegrees(acos(dot.x));

		//少しでも移動方向（角度）が変わったら
		if (ang >= 0.1f)
		{
			if (ang > 15)
			{
				ang = 15.0f;
			}
			//外積（どっちに回転するを調べる）
			Math::Vector3 cross = DirectX::XMVector3Cross(toVec, nowVec);
			cross.Normalize();
			if (cross.y >= 0)
			{
				m_ang -= ang;
			}
			else
			{
				m_ang += ang;
			}
		}
	
	}

	if (moveFlg == false || m_groundHitDis>= PlayerSetting::height)
	{
		// サウンド
		m_isSoundMovePlay = false;
		m_soundInstMove->Pause();
	}
	//回転行列
	Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
	// 座標行列			
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	m_mat = rotMat * transMat;

	//キーフレームアニメ	
	m_startMat = m_mat;
	Math::Matrix animeTransMat = DirectX::XMMatrixTranslation(0, 0.2f, 0);
	m_endMat = animeTransMat * m_mat;
	m_animeFrame = (sin(DirectX::XMConvertToRadians(m_animeAng)) + 1) / 2;
	m_animeAng += 10;
	if (m_animeAng > 360)
	{
		m_animeAng -= 360;
	}
	if (moveFlg == true)
	{
		AnimeProc();
	}



}

void Player::Update_TPS()               //照準状態
{

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_worldPos += DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, PlayerSetting::movePowTPS), camRotMat);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_worldPos -= DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, PlayerSetting::movePowTPS), camRotMat);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_worldPos -= DirectX::XMVector3TransformNormal(Math::Vector3(PlayerSetting::movePowTPS, 0, 0), camRotMat);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		Math::Matrix camRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
		m_worldPos += DirectX::XMVector3TransformNormal(Math::Vector3(PlayerSetting::movePowTPS, 0, 0), camRotMat);
	}
	Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	m_mat = rotMat * DirectX::XMMatrixTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);

}


void Player::Update_Attack()
{
	Math::Vector3 vec;
	vec = m_gameScene->GetHand().GetMat().Translation() - m_worldPos;
	if (vec.Length() <= PlayerSetting::toEnemyDis)
	{
		m_gameScene->GetHand().SetShootState(false);
		m_gameScene->GetHand().SetEnemyHitState(false);
		SetAimmingFlg(false);
		m_stateFlg = PlayerState::Normal;
	}

	vec.Normalize();
	vec *= PlayerSetting::pushPow;
	m_worldPos += vec;
	Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	m_mat = rotMat * transMat;
}

void Player::Update_Approach()		//接近処理
{
	Math::Vector3 vec;
	vec = m_gameScene->GetHand().GetMat().Translation() - m_worldPos;
	if (vec.Length() <= 1.5f)
	{
		m_gameScene->GetHand().SetShootState(false);
		m_gameScene->GetHand().SetMapHitState(false);
		m_gameScene->GetHand().SetPlatFormMoveHitState(false);
		m_gameScene->GetHand().SetPlatFormInvisibleHitState(false);
		SetAimmingFlg(false);
		m_stateFlg = PlayerState::Stick;

	}

	vec.Normalize();
	vec *= PlayerSetting::pushPow;
	m_worldPos += vec;
	Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	m_mat = rotMat * transMat;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState('S') & 0x8000) // 貼り付け状態解除
	{
		m_stateFlg = PlayerState::Normal;
		m_gameScene->GetHand().SetShootState(false);
	}
}

void Player::Update_Stick()
{
	PlatFormUpstanding& PFU = m_gameScene->GetPlatFormMove();

	Math::Vector3 vecFront = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), m_mat);

	if (m_gameScene->GetPlatFormMove().GetOnFlg() == false)
	{
		PFU.CheckBoard(m_worldPos, vecFront, m_PlatformUpHitDis);
	}
	if (m_PlatformUpHitDis <= PlayerSetting::toPlatformUpDisLimit)
	{
		PFU.MoveTogether();
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState('S') & 0x8000) // 貼り付け状態解除
	{
		m_stateFlg = PlayerState::Normal;
		m_gameScene->GetHand().SetShootState(false);
		PFU.SetIsOnFlg(false);
		m_PlatformUpHitDis = FLT_MAX;
	}

}

void Player::Update_Movie()
{
	if (m_movieState == MovieState::Movie1)
	{
		Math::Vector3 pos1 = { 1,1.50f,-263 };
		Update_Auto(pos1);
	}
	if (m_movieState == MovieState::Movie2)
	{

	}
	if (m_movieState == MovieState::Movie3)
	{

	}
}

void Player::Update_Auto(Math::Vector3 _pos)
{

	Math::Vector3 vec;
	vec = _pos - m_worldPos;

	if (vec.Length() <= 1) // 目的地との距離は１いない移動させない
	{
		vec *= 0;
	}

	vec.Normalize();
	vec *= PlayerSetting::inMovieMoveDis;
	m_worldPos += vec;
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	m_mat = transMat;

}



void Player::Draw()
{

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	SHADER.m_effectShader.SetToDevice();
	m_shadow->Draw();

	SHADER.m_standardShader.SetToDevice();

}

void Player::Camera()
{
	if (m_isMovie == false)
	{
		if (m_camState == CamState::Normal)
		{
			Camera_Normal();
		}
		if (m_camState == CamState::Aim)
		{
			m_camFrame += 0.1f;
			if (m_camFrame >= 1.0f)
			{
				m_camFrame = 1.0f;
				m_aimShow = true;
			}
			//デフォルトカメラ位置
			//m_camMove = { 0, 2, -5.5f };

			Camera_TPS();
		}
		else {
			m_camFrame -= 0.1f;
			if (m_camFrame <= 0)
			{
				m_camFrame = 0;
				SetCamState(CamState::Normal);
			}
			m_aimShow = false;
		}
		CamProc();
	}
	if (m_isMovie == true)
	{
		Camera_LookAt();

		if (m_gameScene->GetMap().GetFence().GetCloseFlg() == true)
		{
			m_camFrame += 0.02f;
			if (m_camFrame >= 1)
			{
				m_camFrame = 1.0f;
				m_isMovie = false;
				SetCamState(CamState::Normal);
			}
			Camera_Back();
		}
		else {
			m_camFrame = 0;
		}
		CamProc();
	}

	/*if (m_gameScene->GetChest().GetOpenFlg() == true)
	{
		Camera_Attention();
	}*/

	SetCamAnimePra(m_camPreviousMat, m_camNextMat);
}

void Player::Camera_Normal()
{
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_camAngX));
	Math::Matrix rotMat = rotMatX * rotMatY;
	m_camPos = DirectX::XMVector3TransformNormal(m_camMove, rotMat);

	//デフォルトカメラ位置
	m_camMove = { 0, 2, -6.0f };

	m_camPos = m_camPos + m_worldPos;
	m_camLook = m_worldPos + Math::Vector3(0, 2, 0);
	Math::Vector3 camHead = DirectX::XMVector3TransformNormal(Math::Vector3(0, 1, 0), m_mat);

	//カメラ
	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(m_camPos, m_camLook, camHead);

	//if (m_camState == CamState::Aim)
	{
		m_camPreviousMat = DirectX::XMMatrixLookAtLH(m_camPos, m_camLook, camHead);
	}


	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)ScrW / (float)ScrH, 0.01f, 2000.0f);
}

void Player::Camera_TPS()
{
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_camAngX));
	Math::Matrix rotMat = rotMatX * rotMatY;
	m_camPos = DirectX::XMVector3TransformNormal(m_camMove, rotMat);
	//デフォルトカメラ位置
	m_camMove = { 1.5f,0.5f,-3 };

	m_camPos = m_camPos + m_worldPos;
	m_camLook = DirectX::XMVector3TransformNormal(Math::Vector3(1.32f, 0.38f, 0), rotMat);
	m_camLook = m_camLook + m_worldPos;
	Math::Vector3 camHead = DirectX::XMVector3TransformNormal(Math::Vector3(0, 1, 0), m_mat);


	if (m_camState == CamState::Aim)
	{
		m_camNextMat = DirectX::XMMatrixLookAtLH(m_camPos, m_camLook, camHead);
	}
	SHADER.m_cb7_Camera.Work().mV = m_camNowMat;



	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(40), (float)ScrW / (float)ScrH, 0.01f, 2000.0f);
}




//struct CAMERA
//{
//	Math::Vector3 camPos;
//	Math::Vector3 camLook;
//	Math::Vector3 camHead;
//};
//
//CAMERA CamList[5];
//CamList[0].camPos= Math::Vector3(3, 4, -280);
//
//
//void Camera::ChangeCamra(int targetNo)
//{
//
//}


void Player::Camera_LookAt()
{
	Math::Vector3 camPos = Math::Vector3(3, 4, -280);

	Math::Vector3 camLook = m_worldPos;

	Math::Vector3 camHead = Math::Vector3(0, 1, 0);

	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(camPos, camLook, camHead);

	if (m_movieState == MovieState::Movie1)
	{
		m_camPreviousMat = SHADER.m_cb7_Camera.Work().mV;
	}
	//カメラ
	//SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(camPos, camLook, camHead);


	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)ScrW / (float)ScrH, 0.01f, 2000.0f);
}

void Player::Camera_Back()
{

	Math::Vector3 camPos = Math::Vector3(1, 3.50f, -270);

	Math::Vector3 camLook = Math::Vector3(1, 3.50f, -263);
	Math::Vector3 camHead = Math::Vector3(0, 1, 0);

	if (m_movieState == MovieState::Movie1)
	{
		m_camNextMat = DirectX::XMMatrixLookAtLH(camPos, camLook, camHead);

		//カメラ
		SHADER.m_cb7_Camera.Work().mV = m_camNowMat;
	}

	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)ScrW / (float)ScrH, 0.01f, 2000.0f);

}

void Player::Camera_Attention()
{


	Math::Vector3 camPos = Math::Vector3(0, 30, 0);
	Math::Vector3 camLook = Math::Vector3(50, 0, -50);
	Math::Vector3 camHead = Math::Vector3(0, 1, 0);

	{
		//カメラ
		SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(camPos, camLook, camHead);
	}

	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)ScrW / (float)ScrH, 0.01f, 2000.0f);

}

void Player::AnimeProc()
{
	// クォータニオン用意
	Math::Quaternion startQua;
	Math::Quaternion endQua;
	Math::Quaternion nowQua;

	//最初と最後の行列からクォータニオンを作成
	startQua = DirectX::XMQuaternionRotationMatrix(m_startMat);
	endQua = DirectX::XMQuaternionRotationMatrix(m_endMat);

	//中間行列を求める(slerp=エス　ラーブ)
	nowQua = DirectX::XMQuaternionSlerp(startQua, endQua, m_animeFrame);
	m_mat = DirectX::XMMatrixRotationQuaternion(nowQua);

	//中間座標を求める
	Math::Vector3 startPos;
	Math::Vector3 endPos;
	Math::Vector3 nowPos;

	//最初と最後の位置を求める
	startPos = m_startMat.Translation();
	endPos = m_endMat.Translation();

	//中間座標を求める（lerpラーブ）
	nowPos = DirectX::XMVectorLerp(startPos, endPos, m_animeFrame);

	//中間座標を行列にセット
	m_mat.Translation(nowPos);
}

void Player::CamProc()
{
	// クォータニオン用意
	Math::Quaternion startQua;
	Math::Quaternion endQua;
	Math::Quaternion nowQua;

	//最初と最後の行列からクォータニオンを作成
	startQua = DirectX::XMQuaternionRotationMatrix(m_camStartMat);
	endQua = DirectX::XMQuaternionRotationMatrix(m_camEndMat);

	//中間行列を求める(slerp=エス　ラーブ)
	nowQua = DirectX::XMQuaternionSlerp(startQua, endQua, m_camFrame);
	m_camNowMat = DirectX::XMMatrixRotationQuaternion(nowQua);

	//中間座標を求める
	Math::Vector3 startPos;
	Math::Vector3 endPos;
	Math::Vector3 nowPos;

	//最初と最後の位置を求める
	startPos = m_camStartMat.Translation();
	endPos = m_camEndMat.Translation();

	//中間座標を求める（lerpラーブ）
	nowPos = DirectX::XMVectorLerp(startPos, endPos, m_camFrame);

	//中間座標を行列にセット
	m_camNowMat.Translation(nowPos);
}

//void Player::CamProc()
//{
//	//中間座標を求める
//	Math::Vector3 startCamPos;
//	Math::Vector3 endCamPos;
//	Math::Vector3 nowCamPos;
//
//	//中間座標を求める（lerpラーブ）
//	nowCamPos = DirectX::XMVectorLerp(startCamPos, endCamPos, m_camFrame);
//
//
//	//中間座標を求める
//	Math::Vector3 startLookPos;
//	Math::Vector3 endLookPos;
//	Math::Vector3 nowLookPos;
//
//	//中間座標を求める（lerpラーブ）
//	nowLookPos = DirectX::XMVectorLerp(startLookPos, endLookPos, m_camFrame);
//
//
//	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(nowCamPos, nowLookPos, camHead);
//
//}

void Player::CheckHit(const Math::Vector3& _vec)
{
	const Map& map = m_gameScene->GetMap();
	Math::Vector3 mapNormal;
	bool mapHit;// 
	float mapDis;// 
	mapHit = map.CheckBoard(m_worldPos, _vec, mapDis, mapNormal);
	PushBack(mapHit,mapDis, mapNormal, _vec);

	/*const Obstacle& obstacle = m_gameScene->GetObstacle();
	if (obstacle.GetAliveFlg() != false)
	{
		Math::Vector3 obsNormal;
		bool obsHit;
		float obsDis;
		obsHit = obstacle.CheckBoard(m_worldPos, _vec, obsDis, obsNormal);
		PushBack(obsHit, obsDis, obsNormal, _vec);
	}*/

	const PlatFormUpstanding& PFM = m_gameScene->GetPlatFormMove();
	Math::Vector3 PFMNormal;
	float PFMDis;
	bool PFMhit = PFM.CheckBoard(m_worldPos, _vec, PFMDis, PFMNormal);
	PushBack(PFMhit, PFMDis, PFMNormal, _vec);

	const Chest& chest = m_gameScene->GetChest();
	Math::Vector3 chestNormal;
	float ChestDis;
	bool chestHit = chest.CheckBoard(m_worldPos, _vec, ChestDis, chestNormal);
	PushBack(chestHit, ChestDis, chestNormal, _vec);

}

void Player::PushBack(bool _hit, float _hitDis, const Math::Vector3& _normal, const Math::Vector3& _vec)
{
	// 地面か壁かの判断
	Math::Vector3 upVec = { 0,1,0 };
	Math::Vector3 dot = DirectX::XMVector3Dot(upVec, _normal);
	//クランプ(遮断)する	
	if (dot.x > 1)
	{
		dot.x = 1.0f;
	}
	if (dot.x < -1)
	{
		dot.x = -1.0f;
	}
	//角度を取れる（ラジアン角）
	float ang = DirectX::XMConvertToDegrees(acos(dot.x)); 
	if (ang < 90) // 地面判明
	{
		_hit = false;
	}

	// 壁ずり
	if (_hit) // 進路上に壁がある
	{
		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(_normal, -_vec);

		//マジックナンバー　/直値 /即値
		float limit;
		limit = PlayerSetting::awayDist / dot.x;
		//立ち入り禁止エリアの幅/コサインθ＝立ち止まるべき距離
		if (limit < 0) //壁の裏側から侵入した場合
		{
			limit *= -1;//距離をプラスの値に変換
		}
		
		if (_hitDis < limit)  //壁までの距離が「立ち止まるべき距離」より短い
		{
			m_worldPos += _normal * ((limit - _hitDis) * dot.x);
			
		}
	}
}

void Player::CheckHit_Ground()
{
	float minDist = FLT_MAX;

	const Map& map = m_gameScene->GetMap();
	if (map.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}



	const Bridge& bridgeBack = m_gameScene->GetBridgeBack();
	if (bridgeBack.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	const Bridge& bridgeFront = m_gameScene->GetBridgeFront();
	if (bridgeFront.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	if (m_gameScene->GetPFI().CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	if (m_gameScene->GetPFS().CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (m_groundHitDis <= PlayerSetting::height)
		{
			m_gameScene->GetPFS().MoveTogether();
		}
		else
		{
			m_gameScene->GetPFS().SetIsOnFlg(false);
		}


		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	Wheel& whell_0 = m_gameScene->GetWhellDown();
	if (whell_0.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (m_groundHitDis <= PlayerSetting::height)
		{
			whell_0.MoveTogether();
		}
		else
		{
			whell_0.SetOnWheelFlg(false);
		}

		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	Wheel& whell_1 = m_gameScene->GetWhellUp();
	if (whell_1.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (m_groundHitDis <= PlayerSetting::height)
		{
			whell_1.MoveTogether();
		}
		else
		{
			whell_1.SetOnWheelFlg(false);
		}

		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	Gear& gear = m_gameScene->GetGear();
	if (gear.CheckBoard(m_worldPos, Math::Vector3(0, -1, 0), m_groundHitDis))
	{
		if (m_groundHitDis <= PlayerSetting::height)
		{
			gear.MoveTogether();
		}
		else
		{
			gear.SetOnGearFlg(false);
		}

		if (minDist > m_groundHitDis)
		{
			minDist = m_groundHitDis;
		}
	}

	if (minDist != FLT_MAX)
	{
		m_nowGround = m_worldPos.y + (PlayerSetting::awayDist - minDist);
	}
}

void Player::CheckHit_Ray()
{

	const Map& map = m_gameScene->GetMap();
	if (map.CheckBoard(m_camPos, GetLookVec(), m_aimRayDis))
	{
		//レイが当たったなら着弾点を計算する
		m_hitPos = m_camPos + GetLookVec() * m_aimRayDis;
	}
	else
	{
		m_aimRayDis = -1;
	}

	std::list<Enemy*>& enemyList = m_gameScene->GetEnemyManager().GetEnemyList();
	for (auto enemy : enemyList)
	{
		if (enemy->CheckBoard(m_camPos, GetLookVec(), m_aimRayDis))
		{
			m_hitPos = m_camPos + GetLookVec() * m_aimRayDis;
			m_enemySpotted = true;
			break;
		}
		else
		{
			m_enemySpotted = false;
		}
	}

}

Math::Vector3 Player::GetLookVec()
{
	Math::Vector3 rayVec;
	if (m_camState == CamState::Aim)
	{
		rayVec = m_camLook - m_camPos;

		rayVec.Normalize();

		return rayVec;
	}
	else
	{
		rayVec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), m_mat);
		return rayVec;
	}



}

void Player::SavePos()
{	
	float dist=FLT_MAX;
	
	m_savePos = m_mat.Translation();
}

void Player::SetCamState(CamState _state)
{
	m_camState = _state;
}

void Player::SetCamAnimePra(Math::Matrix _startMat, Math::Matrix _endMat)
{
	m_camStartMat = _startMat;
	m_camEndMat = _endMat;
}





