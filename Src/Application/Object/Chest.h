#pragma once
class GameScene;
class Chest
{
public:
	Chest(GameScene *_gameScene);
	~Chest();
	void Update();
	void Draw();
	const bool GetOpenFlg()const { return m_isOpen; }
	const Math::Vector3 GetPos()const { return m_mat.Translation(); }
	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const;
private:



	KdModel* m_model;
	KdModel* m_modelTop;
	Math::Matrix m_mat;
	Math::Matrix m_matTop;

	// “–‚½‚è”»’è—pƒ‚ƒfƒ‹
	KdModel* m_modelUpdate;
	Math::Matrix m_matUpdate;

	float m_ang;
	GameScene* m_gameScene;

	bool m_isOpen;
};