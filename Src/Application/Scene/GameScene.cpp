#include"../Headers.h"

bool ModelIntersects_IncludeNormal(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) //壁チェック
{
	float nearestDist = FLT_MAX;	//FLT_MAX==float型の最大値

	Math::Vector3 hitVPos[3];  //レイが当たったポリゴンの頂点座標
	Math::Matrix hitMat;  //レイが当たったポリゴンの行列

	for (auto&& node : _model.GetAllNodes())
	{
		//メッシュが含まれていないノードなら飛ばす
		if (node.Mesh == nullptr)continue;

		//そのノードのワールド行列を取得
		Math::Matrix worldMat = node.LocalTransform * _mat;

		//逆行列化
		Math::Matrix inv = worldMat.Invert();

		//逆行列を使ってローカル座標に変換
		Math::Vector3 localPos = DirectX::XMVector3Transform(_rayPos, inv);
		Math::Vector3 localDir = DirectX::XMVector3TransformNormal(_rayVec, inv);

		//拡大行列を使っているメッシュ用にレイの長さを計っておく
		float rayLen = localDir.Length();

		//方向は必ず長さ１にする
		localDir.Normalize();

		//メッシュ内の全ポリゴンを回す
		for (auto&& face : node.Mesh->GetFaces())
		{
			float dist;

			Math::Vector3 vPos[3];  //今回レイ判定するポリゴンの３頂点座標
			vPos[0] = node.Mesh->GetVertexPositions()[face.Idx[0]];   //faceは向いているポリコム
			vPos[1] = node.Mesh->GetVertexPositions()[face.Idx[1]];
			vPos[2] = node.Mesh->GetVertexPositions()[face.Idx[2]];

			bool hit = DirectX::TriangleTests::Intersects(
				localPos,
				localDir,
				vPos[0],//１頂点目の座標
				vPos[1],//２頂点目の座標
				vPos[2],//３頂点目の座標
				dist	//レイが当たったところまでの距離(参照型)
			);

			if (hit == false)continue;

			//拡大分を反映
			dist /= rayLen;

			//もっとも近い距離を残す
			if (dist < nearestDist)
			{
				nearestDist = dist;

				//レイが当たったポリゴンの頂点座標を保存
				hitVPos[0] = vPos[0];
				hitVPos[1] = vPos[1];
				hitVPos[2] = vPos[2];

				//レイが当たったポリゴンの行列を保存
				hitMat = worldMat;
			}
		}
	}
	if (nearestDist == FLT_MAX)
	{
		return false;
	}
	else {
		_dis = nearestDist;

		//ポリゴンの縁（辺）を表すベクトルを求める
		Math::Vector3 vec1, vec2;
		vec1 = hitVPos[1] - hitVPos[0];
		vec2 = hitVPos[2] - hitVPos[0];

		//ポリゴン２辺に対する外積（＝そのポリゴンの法線）
		_normalVec = DirectX::XMVector3Cross(vec1, vec2);

		//法線を現在のモデルの向きに合わせて回転
		_normalVec = DirectX::XMVector3TransformNormal(_normalVec, hitMat);

		//法線を正規化
		_normalVec.Normalize();

		return true;
	}
}

bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)  //地面チェック
{
	float nearestDist = FLT_MAX;	//FLT_MAX==float型の最大値
	for (auto&& node : _model.GetAllNodes())
	{
		//メッシュが含まれていないノードなら飛ばす
		if (node.Mesh == nullptr)continue;

		//そのノードのワールド行列を取得
		Math::Matrix worldMat = node.LocalTransform * _mat;

		//逆行列化
		Math::Matrix inv = worldMat.Invert();

		//逆行列を使ってローカル座標に変換
		Math::Vector3 localPos = DirectX::XMVector3Transform(_rayPos, inv);
		Math::Vector3 localDir = DirectX::XMVector3TransformNormal(_rayVec, inv);

		//拡大行列を使っているメッシュ用にレイの長さを計っておく
		float rayLen = localDir.Length();

		//方向は必ず長さ１にする
		localDir.Normalize();

		//メッシュ内の全ポリゴンを回す
		for (auto&& face : node.Mesh->GetFaces())
		{
			float dist;

			bool hit = DirectX::TriangleTests::Intersects(
				localPos,
				localDir,
				node.Mesh->GetVertexPositions()[face.Idx[0]],//１頂点目の座標
				node.Mesh->GetVertexPositions()[face.Idx[1]],//２頂点目の座標
				node.Mesh->GetVertexPositions()[face.Idx[2]],//３頂点目の座標
				dist	//レイが当たったところまでの距離(参照型)
			);

			if (hit == false)continue;

			//拡大分を反映
			dist /= rayLen;

			//もっとも近い距離を残す
			if (dist < nearestDist)
			{
				nearestDist = dist;
			}
		}
	}
	if (nearestDist == FLT_MAX)
	{
		return false;
	}
	else {
		_dis = nearestDist;

		return true;
	}
}

float GetVecAngY(Math::Vector3 _vec) // 目標に指すベクトルと今向いているベクトル間の角度
{
	_vec.y = 0;
	_vec.Normalize();
	Math::Vector3 frontVec = { 0,0,1 };

	//回転角度を求める
	//内積を求める
	Math::Vector3 dot = DirectX::XMVector3Dot(frontVec, _vec);
	//クランプする
	if (dot.x > 1)
	{
		dot.x = 1.0f;
	}
	if (dot.x < -1)
	{
		dot.x = -1.0f;
	}
	float  angY;
	angY = DirectX::XMConvertToDegrees(acos(dot.x));
	if (_vec.x < 0)
	{
		angY *= -1;
	}
	return angY;
}

float TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng)// 回転
{
	float currentAng; //キャラクターの今の向き
	currentAng = GetVecAngY(_mat.Backward());
	//_mat.Backward()=>行列の前方ベクトル
	//右手座標と左手座標の違いからBackwardが前方を表す

	float lastAng;//あと何度回転すべきか
	lastAng = _targetAng - currentAng;

	//+180~-180の範囲にする
	while (lastAng > 180)
	{
		lastAng -= 360;
	}
	while (lastAng < -180)
	{
		lastAng += 360;
	}

	Math::Matrix tmpMat;
	if (lastAng > _plusAng) //目標角度を右方向
	{
		tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(_plusAng));
		lastAng -= _plusAng;
	}
	else
	{
		if (lastAng < -_plusAng) //目標角度を左方向
		{
			tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-_plusAng));
			lastAng += _plusAng;
		}
		else  //目標角度まで_plusAng度以下である
		{
			tmpMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(lastAng));
			lastAng = 0;
		}
	}
	_mat = tmpMat * _mat;

	return lastAng;
}

//==========================================================================

void GameScene::Update()
{

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (SYSTEM.GetEscapeKeyFlg() == false)
		{
			SYSTEM.SetEscapeKeyFlg(true);

			if (MessageBox(APP.m_window.GetWndHandle(), L"タイトルに戻りますか？", L"確認", MB_YESNO | MB_DEFBUTTON2) == IDYES) // 引数１は親ウインドウ
			{
				//タイトル戻り
				SCENEMANAGER.ChangeScene(new TitleScene());
			}
		}
	}
	else
	{
		SYSTEM.SetEscapeKeyFlg(false);
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		m_enemyManager->Add(new Enemy(this, Math::Vector3{ (float)(rand() % 30),170,(float)(rand() % 30 - 20) }));
	}

	m_objectManager->Update();	
		
	m_map->Update();	
	m_chest->Update();
	m_water->Update();
	//m_obstacle->Update();
	for (int i = 0; i < 2; i++)
	{
		m_wheel[i]->Update();
	}
	for (int i = 0; i < 2; i++)
	{
		m_bridge[i]->Update();
	}
	m_waterFall->Update();
	m_gear->Update();
	m_ball->Update();
	m_PFU->Update();
	m_PFI->Update();
	m_PFS->Update();
	m_WR->Update();

	m_effectManager->Update();	

	m_ui->Update();

	m_player->Update();
	m_leftArm->Update();
	m_shield->Update();
	m_rightArm->Update();
	m_hand->Update();


	m_enemyManager->Update();
	
	
}

void GameScene::DynamicDraw()
{
	// レンダーターゲットをテクスチャに変更
	m_gameTex.SetRenderTarget();

	SHADER.m_standardShader.SetToDevice();
	
	m_map->Draw();
	m_chest->Draw();
	m_water->Draw();
	for (int i = 0; i < 2; i++)
	{
		m_wheel[i]->Draw();
	}

	for (int i = 0; i < 2; i++)
	{
		m_bridge[i]->Draw();
	}


	if (m_chest->GetOpenFlg() == true)
	{
		m_waterFall->Draw();
	}

	m_gear->Draw();
	m_ball->Draw();
	//m_obstacle->Draw();
	m_PFU->Draw();
	m_PFS->Draw();
	//m_WR->Draw();

	m_player->Draw();
	m_leftArm->Draw();
	m_shield->Draw();
	m_rightArm->Draw();
	m_hand->Draw();

	m_objectManager->Draw();

	m_enemyManager->Draw();

	SHADER.m_effectShader.SetToDevice();
	m_effectManager->Draw();

	m_hand->Draw_Frie();

	//m_SP->Draw();

	m_ui->Draw();

}

void GameScene::Draw()
{
	//レンダーターゲットをバックバッファに変更
	D3D.SetBackBuffer();
	SHADER.m_spriteShader.Begin();

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_gameTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	// 出来上がったゲーム画面（テクスチャ）をシェーダーへ持っていく
	// テクスチャ(ShaderResourceView)セット	
	D3D.GetDevContext()->PSSetShaderResources(4, 1, m_gameTex.GetSRViewAddress());		

	SHADER.m_spriteShader.End();

	SHADER.m_standardShader.SetToDevice();

	m_PFI->Draw();

	
}

void GameScene::Frame()
{
	APP.m_console.ClearLog();
	// ゲーム更新処理
	Update();

	// バックバッファクリア
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBAを0-1の範囲で
	// 
	D3D.GetDevContext()->ClearRenderTargetView(m_gameTex.GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));
	// Zバッファクリア
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);


	//カメラ
	m_player->Camera();	

	//ビュー行列・射影行列をGPUに転送
	SHADER.m_cb7_Camera.Write();

	// テクスチャに描画
	DynamicDraw();

	// ゲーム描画処理
	Draw();
}

 GameScene::GameScene()
{
	// 平行ライト設定
	SHADER.m_cb8_Light.Work().DL_Dir = { 0.5f,-1,0 };
	SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
	SHADER.m_cb8_Light.Work().DL_Color = { 3,3,3 };

	// ライトのデータを書き込む
	SHADER.m_cb8_Light.Write();

	m_ui = std::make_shared<UI>(this);
	m_player = std::make_shared<Player>(this);
	m_leftArm = std::make_shared<Arm_Left>(this);
	m_shield = std::make_shared<Shield>(this);
	m_rightArm = std::make_shared<Arm_Right>(this);
	m_hand = std::make_shared<Hand>(this);
	
	

//	m_sound = new Sound(this);

	m_map = std::make_shared<Map>(this);

	m_chest = std::make_shared<Chest>(this);
	m_water = std::make_shared<Water>(this);
	{
		float plusAng = 0.16f;
		Math::Matrix transMat = DirectX::XMMatrixTranslation(-100, 45, 0);		
		
		m_wheel[0] = std::make_shared<Wheel>(this,plusAng, transMat,Math::Matrix::Identity);
	}
	{
		float plusAng = -0.16f;
		Math::Matrix transMat = DirectX::XMMatrixTranslation(-52.7f, 120, -124.8);	//115.6	
		Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(60));
		m_wheel[1] = std::make_shared<Wheel>(this,plusAng, transMat,Math::Matrix::Identity);
	}
	{
		float maxHeight = 163.0f;
		float upSpeed = 0.1f;
		m_bridge[0] = std::make_shared<Bridge>(this, "Data/Models/Map/Object/Bridge.gltf", maxHeight, upSpeed);
	}
	{
		float maxHeight = 94.0f;
		float UpSpeed = 1.0f;
		m_bridge[1] = std::make_shared<Bridge>(this, "Data/Models/Map/Object/BridgeFront.gltf", maxHeight, UpSpeed);
	}
	

	m_waterFall = std::make_shared<WaterFall>(this);
	m_gear = std::make_shared<Gear>(this);
	m_ball = std::make_shared<Ball>(this);
	m_obstacle = std::make_shared<Obstacle>(this);
	m_PFU = std::make_shared<PlatFormUpstanding>(this);
	m_PFI = std::make_shared<PlatFormInvisible>();
	m_PFS = std::make_shared<PlatFormSidewards>(this);
	m_WR = std::make_shared<Wall_Reflect>();

	Sky* sky = new Sky();
	Door_Left* doorLeft = new Door_Left(this);
	Door_Right* doorRight = new Door_Right(this);
	Hammer* hammer_1 = new Hammer(this,1.4f, 1, 50, Math::Vector3(-20, 148, 0));
	Hammer* hammer_2 = new Hammer(this,1.6f, -1, 60, Math::Vector3(50, 148, 0));
	//Ball* ball = new Ball(this);
	

	m_objectManager = std::make_shared<ObjectManager>();
	m_objectManager->Add(sky);
	m_objectManager->Add(doorLeft);
	m_objectManager->Add(doorRight);
	m_objectManager->Add(hammer_1);
	m_objectManager->Add(hammer_2);
	//m_objectManager->Add(ball);
	
	
	
	//m_SP = new SavePoint(this, Math::Vector3{ 0,10,0 });
	
	m_effectManager = std::make_shared<EffectManager>();
	
	m_enemyManager = std::make_shared<EnemyManager>();


	// sound
	m_soundBgm = std::make_shared<KdSoundEffect>();
	m_soundBgm->Load("Data/Sound/BGM.wav");
	m_soundBgmInst = m_soundBgm->CreateInstance(false);
	m_soundBgmInst->SetVolume(0.1f);
	m_soundBgmInst->Play(true);

	// ゲーム画面を描画するテクスチャを初期化
	m_gameTex.CreateRenderTarget(1280, 720);
	
}

 GameScene::~GameScene()
{
	m_soundBgmInst->Pause();
}

 void GameScene::ImGuiUpdate()
 {

	// ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	// ImGui::SetNextWindowSize(ImVec2(240, 100), ImGuiSetCond_Once);

	// if (ImGui::Begin("Player"))
	// {
	//	 ImGui::Text("[FPS]%d", APP.m_fps);
	//	 ImGui::Text("[PlayerPos]%6.2f %6.2f %6.2f", m_player->GetPos().x, m_player->GetPos().y, m_player->GetPos().z);
	//	 ImGui::Text("[Hand]%d", m_hand->GetShootState());	
	//	 ImGui::Text("[State]%d", m_player->GetState());	
	//	 
	// }
	// ImGui::End();

	// ImGui::SetNextWindowPos(ImVec2(600, 20), ImGuiSetCond_Once);
	// ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiSetCond_Once);

	///* if (ImGui::Begin("Ball"))
	// {
	//	 ImGui::Text("[NowPos]%6.2f %6.2f %6.2f", m_ball->GetNowPos().x, m_ball->GetNowPos().y, m_ball->GetNowPos().z);
	//	 ImGui::Text("[OldPos]%6.2f %6.2f %6.2f", m_ball->GetOldPos().x, m_ball->GetOldPos().y, m_ball->GetOldPos().z);
	// }
	// ImGui::End();*/
	// if (ImGui::Begin("Ball"))
	// {
	//	 Math::Vector3 pos = m_ball->GetPos();
	//	 ImGui::SliderFloat("x", &pos.x, -150, 150);
	//	 ImGui::SliderFloat("y", &pos.y, -320, 350);
	//	 ImGui::SliderFloat("z", &pos.z, -150,150);
	//	 if (ImGui::Button("Reset"))
	//	 {
	//		 pos={0,350,0};
	//	 }
	//	 m_ball->SetPos(pos);
	// }
	// ImGui::End();

	// ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiSetCond_Once);
	// ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);

	// if (ImGui::Begin("ball"))
	// {
	//	 ImGui::Text("[moveVec]%6.2f %6.2f %6.2f", m_ball->GetMoveVec().x, m_ball->GetMoveVec().y, m_ball->GetMoveVec().z);
	// }
	// ImGui::End();

 }

