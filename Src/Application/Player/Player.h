#pragma once

class GameScene;
class Map;
class Shadow;
class Player
{
public:
	enum class CamState
	{
		Normal,
		Aim
	};

	enum class PlayerState
	{
		Normal,
		Aim,
		Approach,
		Stick,
		Attack,
	};

	Player(GameScene* _gameScene);
	~Player();

	void Update();

	void Update_Character();
	void Update_Normal();  // キャラクターの普通状態
	void Update_TPS();	   // キャラクターの照準状態
	void Update_Attack();  // キャラクターの攻撃状態
	void Update_Approach(); // 手へ接近
	void Update_Stick();   // 貼り付け

	void Update_Movie();   
	void Update_Auto(Math::Vector3 _pos);	

	void Draw();

	void Camera();
	void Camera_Normal();	//第三人称カメラ
	void Camera_TPS();		//照準カメラ
	void Camera_LookAt();	//ゲームを始まる時startカメラ
	void Camera_Back();		//ゲームを始まる時endカメラ	
	void Camera_Attention();//

	void SetCamState(CamState _state);
	void SetCamAnimePra(Math::Matrix _startMat,Math::Matrix _endMat); // カメラ変換用パラメーターセット関数
	void SetCamPos(Math::Vector3 _camPos) { m_camMove = _camPos; }
	void SetNowGround(float _hight) { m_nowGround = _hight; }
	void SetAimmingFlg(bool _flg) { m_isAimming = _flg; }
	
	void AnimeProc();//キーフレームアニメーション関数
	void CamProc();// カメラ位置を変わる関数

	void CheckHit(const Math::Vector3& _vec); //PLAYERとモデルの当たり判定関数
	void PushBack(bool _hit, float _hitDis,const Math::Vector3& _normal,const Math::Vector3& _vec);  // 推させる
	void CheckHit_Ground(); // 地面判定
	void CheckHit_Ray(); // 飛ばしたレイとモデルの当たり判定


	void AddPos(const Math::Vector3& _vec)
	{
		m_worldPos += _vec;
	}

	void SetPos(const Math::Vector3& _Pos)
	{
		m_worldPos = _Pos;
	}

	void SetMat(const Math::Matrix& _mat)
	{
		m_mat = _mat;
	}

	const Math::Vector3& GetPos()const { return m_worldPos; }
	const Math::Vector3& GetCamPos()const { return m_camPos; }
	const Math::Matrix& GetMat()const { return m_mat; }
	const float GetAngY()const { return m_camAngY; }
	const float GetAngX()const { return m_camAngX; }
	const bool GetAimState()const { return m_isAimming; }
	const float GetGroundDis()const { return m_groundHitDis; }
	const bool GetAimShow()const { return m_aimShow; }
	const Math::Vector3& GetRayHitPos() const { return m_hitPos; } // 
	const bool GetEnemySpotFlg()const { return m_enemySpotted; }	
	const enum class PlayerState GetState()const { return m_stateFlg; }
	const float GetPlatformUpDist()const { return m_PlatformUpHitDis; }
	const float GetHitDist()const { return m_hitDis; }

	Math::Vector3 GetLookVec(); // 照準している方向

	const float GetAimRayDis()const { return m_aimRayDis; }
	
private:
	void SavePos();

	KdModel* m_model;
	Math::Matrix m_mat;

	Math::Vector3 m_worldPos;//ワールド軸で管理
	//カメラ角度
	float m_camAngX;
	float m_camAngY;
	POINT m_basePt; //マウスの基準座標	
	float m_camAngNow;// 今の向き記録用

	//キャラクター用
	float m_ang;
	Math::Vector3 m_moveVec; // 移動量
	Math::Vector3 m_hammerVec; //hammerから受けた移動ベクトル
	Math::Vector3 m_camPos;// 照準用カメラの位置
	Math::Vector3 m_camMove;// カメラは壁の奥に入れないように用意した変数
	
	Math::Vector3 m_camLook;
	
	bool m_isAimming;//照準フラグ	
	bool m_isJumping;
	bool m_rButtonState;// マウスの右キー押すフラグ
	bool m_enemySpotted;// 敵を狙う

	PlayerState m_stateFlg;  //状態フラグ 　0:普通状態 1:照準状態　2:ツールで移動状態 3:攻撃状態
	CamState m_camState;// カメラの状態
	int m_movieState;

	// カメラ用matrix
	Math::Matrix m_camNextMat;
	Math::Matrix m_camPreviousMat;
	Math::Matrix m_camStartMat;
	Math::Matrix m_camNowMat;
	Math::Matrix m_camEndMat;
	float m_camFrame;

	int m_loadedMap;
	bool m_isMovie;// 演出フラグ

	float m_gravity;	//重力
	float m_groundHitDis; //と足元の距離
	float m_hitDis;//当たり判定距離
	float m_aimRayDis;// 照準状態にカメラから飛ばすレイ判定の距離
	Math::Vector3 m_hitPos;//レイの着弾座標

	float m_PlatformUpHitDis; // と立ち足場の距離 

	//キーフレームアニメ用
	Math::Matrix m_startMat;
	Math::Matrix m_endMat;
	//アニメのサインカーブ用角度
	float m_animeAng;

	float m_animeFrame; //(0~1)

	float m_nowGround = 0;

	GameScene* m_gameScene;
	
	bool m_aimShow; // ui側の照準を表示FLG


	bool m_hitFlg = false;

	bool m_mouseFlg = false; // Imgui用

	std::shared_ptr<Shadow> m_shadow;

	// セーブポイント
	
	Math::Vector3 m_savePos;

	//移動サウンド
	bool m_isSoundMovePlay;
	std::shared_ptr<KdSoundEffect>m_soundMove;
	std::shared_ptr<KdSoundInstance>m_soundInstMove;

	
};