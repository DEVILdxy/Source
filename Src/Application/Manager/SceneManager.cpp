#include"../Headers.h"

void SceneManager::DoScene()
{
	//現在のシーンとは異なるシーンが登録されていたら
	if (m_nextScene != m_currentScene)
	{
		if (m_currentScene != nullptr)
		{
			delete m_currentScene;
		}

		m_currentScene = m_nextScene;
	}

	if (m_currentScene != nullptr)
	{
		//現在のシーンを実行する
		m_currentScene->Frame();

		// imgui導入で追加============================
		// ImGui開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		// ImGui処理
		m_currentScene->ImGuiUpdate();

		APP.m_console.Draw("Log", nullptr);

		// GUI描画実行
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		// ===============================================
		
	}
}

void SceneManager::ChangeScene(BaseScene* _nextScene)
{
	//移行したい次のシーンを登録
	m_nextScene = _nextScene;
}

SceneManager::~SceneManager()
{
	//何かしらのシーンが実行中だったら
	if (m_currentScene != nullptr)
	{
		//現在実行していたシーンを削除
		delete m_currentScene;
	}
}