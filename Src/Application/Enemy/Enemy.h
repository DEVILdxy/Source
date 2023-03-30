#pragma once

class GameScene;
class MagicCircle;
class BaseState;
class AimCone;
class Enemy
{
public:
	Enemy(GameScene* _gameScene,Math::Vector3 _pos);
	~Enemy();
	void Update();
	void Draw();

	Math::Matrix& GetMat() { return m_mat; }

	const bool GetAliveFlg() { return m_aliveFlg; }

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;
private:
	void Check_Ground();

	void CheckHit(Math::Vector3 _vec);

	void PushBack(bool _hit,Math::Vector3& _normal, Math::Vector3 _vec);

	void AddPos(Math::Vector3 _addPos);

	GameScene* m_gameScene;

	AimCone* m_aimCone;

	Math::Matrix m_mat;
	KdModel* m_modelDraw;
	KdModel* m_modelUpdate;

	bool m_aliveFlg;

	BaseState* m_currentState = nullptr; // 現在のステート
	
	float m_nowGround; // 今の地面

	float m_hitDis; // 当たり判定距離

	

};