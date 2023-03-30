#include "../Headers.h"

Appear::Appear(GameScene* _gameScene, Enemy* _enemy, Math::Matrix& _mat)
{
	m_appearHeight = _mat.Translation().y - 1.5f;	
	_gameScene->GetEffectManager().Add(new MagicCircle(_enemy,_mat.Translation()));
}


BaseState* Appear::Action(GameScene* _gameScene, Math::Matrix& _mat)
{	
	const float speed = 0.04f; // “GŒ»‚·‘¬“x
	const float heightMax = _mat.Translation().y + 1.5f; // “GŒ»‚·‚‚³‚ð§ŒÀ

	m_appearHeight += speed;
	
	if (m_appearHeight >= heightMax)
	{
		m_appearHeight = heightMax;
		return new Track();
	}

	SHADER.m_standardShader.SetAppearHeight(m_appearHeight);

	return nullptr;
}


