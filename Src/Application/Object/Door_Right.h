#pragma once
class GameScene;
class Door_Right :public Object
{
public:

	Door_Right(GameScene* _gameScene);
	~Door_Right();
	void Update()override;

private:
	float m_ang;
	GameScene* m_gameScene;
};