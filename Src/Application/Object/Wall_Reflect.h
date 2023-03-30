#pragma once
class Wall_Reflect :public Object
{
public:
	Wall_Reflect();
	~Wall_Reflect();

	void Update()override;

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const;// ñ@ê¸Ç†ÇË

private:



};