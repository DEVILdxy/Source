#pragma once

class GameScene;
class Obstacle :public Object
{
public:
	Obstacle(GameScene* _gameScene);
	~Obstacle();
	void Update()override;
	void Draw()override;

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const;

	void SetAliveFlg(bool _flg) {m_aliveFlg = _flg; }
	void Hit(); // “G‚Æ•Ç‚ð“–‚½‚é
	const bool GetAliveFlg()const { return m_aliveFlg; }
	const Math::Matrix& GetMat()const { return m_mat; }
	const bool GetHitFlg()const { return m_hitFlg; }
	void SetHitFlg(bool _hitFlg) { m_hitFlg = _hitFlg; }

private:
	GameScene* m_gameScene;
	bool m_aliveFlg;
	bool m_hitFlg;
	int m_hp;


};