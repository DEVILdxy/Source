#pragma once

class GameScene;
class Platform :public Object
{
public:
	Platform(GameScene* _gameScene);
	~Platform();
	void Update()override;
	void Draw()override;
	void SetPos(Math::Vector3 _pos);

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

private:

	GameScene* m_gameScene;

	KdModel* m_modelUpdate;

	Math::Vector3 m_pos;

	float m_dis;
	
};