#pragma once

class SceneManager
{
public:
	~SceneManager();

	void DoScene();
	void ChangeScene(BaseScene* _nextScene);

	static SceneManager& GetInstance()
	{
		static SceneManager sceneManager;

		return sceneManager;
	}

private:
	SceneManager()
	{
	}

	BaseScene* m_currentScene = nullptr;//現在実行中のシーン　
	BaseScene* m_nextScene = nullptr;//つきに実行したいシーン　

};

#define SCENEMANAGER SceneManager::GetInstance()