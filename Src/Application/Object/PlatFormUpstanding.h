#pragma once
class GameScene;
class PlatFormUpstanding :public Object
{
public:
	PlatFormUpstanding(GameScene* _gameScene);
	~PlatFormUpstanding();
	void Update()override;
	void MoveTogether();
	void SetIsOnFlg(bool _flg) { m_isOnFlg = _flg; }

	const float GetAng()const { return m_ang; }
	const float GetSpeed()const { return m_speed; }
	const bool GetDir()const { return m_dirFlg; }
	const bool GetOnFlg()const { return m_isOnFlg; }
	const Math::Matrix& GetMat()const { return m_mat; }

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const; // 法線ある

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const; // 法線なし
private:
	void Move();
	void ChangeDir(); // 回転方向を変える	
	GameScene* m_gameScene;

	float m_ang;
	float m_speed;

	bool m_dirFlg; // 回転方向を決めるフラグ
	bool m_isOnFlg; // キャラクターを乗るフラグ

	Math::Vector3 m_savePos;
};