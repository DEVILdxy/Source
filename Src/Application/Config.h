#pragma once

class PlayerSetting
{
public:
	static constexpr float movePow = 0.4f;// �ʏ��ԂɈړ����x
	static constexpr float movePowTPS = 0.2f;// �Ə���ԂɈړ����x
	static constexpr float toEnemyDis = 1.5f;// �ƓG�̊Ԃ̋���
	static constexpr float pushPow = 0.8f;// ������
	static constexpr float toPlatformUpDisLimit = 3.0f;// �������ꂩ��e���󂯂鋗��
	static constexpr float inMovieMoveDis = 0.3f;// ���o�Ɉړ����x
	static constexpr float height = 1.5f;// �L�����N�^�[�̍���
	static constexpr float awayDist = 1.0f; // ��������֎~�G���A�̕� 
	static constexpr float ToObjectDist = 1.0f; // �ƖڕW�̋���
};