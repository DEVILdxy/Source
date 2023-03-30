#pragma once

class BaseScene
{
public:
	virtual ~BaseScene(); //�ݒu���Ȃ��Ɣh���N���X���s���Ȃ�����

	virtual void Frame();
	
	virtual void SetCamera();
	virtual void Update();
	virtual void Draw();

	// imgui�ǉ�
	virtual void ImGuiUpdate();

private:

	

};