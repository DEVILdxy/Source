#pragma once

class BaseScene
{
public:
	virtual ~BaseScene(); //設置しないと派生クラス実行しないかも

	virtual void Frame();
	
	virtual void SetCamera();
	virtual void Update();
	virtual void Draw();

	// imgui追加
	virtual void ImGuiUpdate();

private:

	

};