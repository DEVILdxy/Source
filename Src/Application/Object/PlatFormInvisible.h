#pragma once
class GameScene;
class PlatFormInvisible :public Object
{
public:
	PlatFormInvisible();
	~PlatFormInvisible();
	void Update()override;
	void Draw()override;

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

private:

};