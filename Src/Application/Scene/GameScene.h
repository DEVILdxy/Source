#pragma once

class GameScene:public BaseScene
{
public:

	// 初期化
	GameScene();

	void Frame()override;

	// 更新処理
	void Update()override;
	// 描画処理
	void DynamicDraw();
	void Draw()override;
	// 終了処理
	~GameScene();

	const UI& GetUI()const {return *m_ui; }

	Player& GetPlayer() { return *m_player; }
	Hand& GetHand() const { return *m_hand; }
	const Shield& GetSword()const { return *m_shield; }
	const Arm_Left& GetLeftArm()const { return *m_leftArm; }
	const Arm_Right& GetRightArm()const { return *m_rightArm; }

	Wheel& GetWhellDown()const{return *m_wheel[0]; }
	Wheel& GetWhellUp()const{return *m_wheel[1]; }
	const Bridge& GetBridgeBack() const { return *m_bridge[0]; }
	const Bridge& GetBridgeFront() const { return *m_bridge[1]; }
	Gear& GetGear()const{return *m_gear;}	
	Map& GetMap()const { return *m_map; }
	const Water& GetWater()const { return *m_water; }	
	const Chest& GetChest()const { return *m_chest; }
	Obstacle& GetObstacle()const { return *m_obstacle; }
	PlatFormUpstanding& GetPlatFormMove()const { return *m_PFU; }
	const PlatFormInvisible& GetPFI() const{ return *m_PFI; }
	PlatFormSidewards& GetPFS() const{ return *m_PFS; }
	const Wall_Reflect& GetWallReflect()const { return *m_WR; }

	EffectManager& GetEffectManager()const  { return *m_effectManager; }
	ObjectManager& GetObjectManager()const { return *m_objectManager; }
	EnemyManager& GetEnemyManager()const { return *m_enemyManager; }




	// imgui導入で追加==========
	void ImGuiUpdate()override;
	
private:
	std::shared_ptr<UI> m_ui;
	std::shared_ptr<Shield> m_shield;
	std::shared_ptr<Hand> m_hand;
	std::shared_ptr<Arm_Left> m_leftArm;
	std::shared_ptr<Arm_Right> m_rightArm;
	std::shared_ptr<Player> m_player;

	std::shared_ptr<Map> m_map;
	std::shared_ptr<Chest> m_chest;
	std::shared_ptr<Water> m_water;
	std::shared_ptr<Wheel> m_wheel[2];
	std::shared_ptr<WaterFall> m_waterFall;
	std::shared_ptr<Bridge> m_bridge[2];
	std::shared_ptr<Gear> m_gear;
	std::shared_ptr<Ball> m_ball;              // 転がる岩
	std::shared_ptr<Obstacle> m_obstacle;      // 障害
	std::shared_ptr<PlatFormUpstanding> m_PFU; // 立ち板
	std::shared_ptr<PlatFormInvisible> m_PFI; // 隠す足場
	std::shared_ptr<PlatFormSidewards> m_PFS; // 寝る板
	std::shared_ptr<Wall_Reflect> m_WR; // ボール反射用

	//SavePoint* m_SP;

	std::shared_ptr<ObjectManager> m_objectManager;
	std::shared_ptr<EffectManager> m_effectManager;
	std::shared_ptr<EnemyManager> m_enemyManager;

	//BGM用
	std::shared_ptr<KdSoundEffect> m_soundBgm;
	std::shared_ptr<KdSoundInstance> m_soundBgmInst;
	
	// ゲーム画面テクスチャ
	KdTexture m_gameTex;

};
