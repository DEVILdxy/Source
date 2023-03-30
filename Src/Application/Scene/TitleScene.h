#pragma once

class TitleScene :public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Draw()override;
	void Update()override;



private:
	KdTexture m_tex; //�^�C�g���摜
	KdTexture m_enterTex;


	//BGM�p
	std::shared_ptr<KdSoundEffect> m_soundBgm;
	std::shared_ptr<KdSoundInstance> m_soundBgmInst;
	
};