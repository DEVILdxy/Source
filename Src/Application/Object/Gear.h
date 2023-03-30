#pragma once

class GameScene;
class Gear
{
public:
	Gear(GameScene* _gameScene);
	~Gear();
	virtual void Update();
	void Draw();

	void SetOnGearFlg(bool _flg);
	void MoveTogether();// キャラクターと今のプロジェクト一緒に回転する
	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec,  float& _dis) const;
private:
	GameScene* m_gameScene;

	KdModel* m_model;
	Math::Matrix m_mat;
	float m_ang;

	bool m_isOnFlg; // キャラクターを乗るフラグ
	Math::Vector3 m_saveVec;
	float m_saveAng;
};