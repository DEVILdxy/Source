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

	BaseScene* m_currentScene = nullptr;//���ݎ��s���̃V�[���@
	BaseScene* m_nextScene = nullptr;//���Ɏ��s�������V�[���@

};

#define SCENEMANAGER SceneManager::GetInstance()