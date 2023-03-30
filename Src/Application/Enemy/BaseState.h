#pragma once

class GameScene;
class BaseState
{
public:
	virtual BaseState* Action(GameScene* _gameScene, Math::Matrix& _mat) = 0;
	//ƒˆ‰¼‘zŠÖ”
	
};