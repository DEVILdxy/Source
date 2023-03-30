#pragma once


class Arm
{
public:
	Arm();
	~Arm();
	virtual void Update();
	void Draw();

protected:
	KdModel* m_model;
	Math::Matrix m_mat;

	GameScene* m_gameScene;
};
