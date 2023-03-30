#pragma once
class GameScene;
class Door_Left :public Object
{
public:

	Door_Left(GameScene* _gameScene);
	~Door_Left();
	void Update()override;

private:
	float m_ang;
	GameScene* m_gameScene;
};