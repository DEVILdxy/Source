#pragma once

class Spray;
class GameScene;
class WaterFall :public Object
{
public:
	WaterFall(GameScene* _gameScene);
	void InitSound();
	~WaterFall();
	void Update()override;
	void Draw()override;


private:

	Math::Vector2 m_uvOffset = { 0,0 };
	GameScene* m_gameScene;

	//3Dサウンド用
	std::shared_ptr<KdSoundEffect> m_sound;
	std::shared_ptr<KdSoundInstance> m_soundInst;

};