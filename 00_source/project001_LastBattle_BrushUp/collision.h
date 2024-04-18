//============================================================
//
//	����֐��w�b�_�[ [collision.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************************
//	���O��Ԑ錾
//************************************************************
// ������
namespace collision
{
	bool Box2D	// XZ���ʂ̋�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,		// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool Box3D	// 3���̋�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,		// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool Circle2D	// XZ���ʂ̉~�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRadius,	// ���蔼�a
		float fTargetRadius,	// ����ڕW���a
		float *pLength = nullptr	// ����ڕW�Ƃ̋���
	);
	bool Circle3D	// 3���̉~�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRadius,	// ���蔼�a
		float fTargetRadius		// ����ڕW���a
	);
	bool Sector	// XZ���ʂ̐�`�̓����蔻��
	( // ����
		D3DXVECTOR3 centerPos,	// ����ʒu
		D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
		float fCenterRot,		// �������
		float fRadius,			// ���E�͈�
		float fAngle,			// ����p
		float *pLength = nullptr	// ����ڕW�Ƃ̋���
	);

	bool BoxPillar	// �p���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
	);
	bool CirclePillar	// �~���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		float fCenterRadius,		// ���蔼�a
		float fTargetRadius			// ����ڕW���a
	);

	bool ResponseBox3D	// �O���̋�`�̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = nullptr,	// �ړ���
		bool *pUp = nullptr,	// �ォ��̔���
		bool *pSide = nullptr,	// ������̔���
		bool *pDown = nullptr	// ������̔���
	);
	bool ResponseCircle3D	// �O���̉~�̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		float fCenterRadius,		// ���蔼�a
		float fTargetRadius			// ����ڕW���a
	);
	bool ResponseCapsule3D	// �O���̃J�v�Z���̏Փ˔���
	( // ����
		D3DXVECTOR3 *pCenterPos,	// ����ʒu
		D3DXVECTOR3& rTargetPos,	// ����ڕW�ʒu
		float fCenterRadius,		// ���蔼�a
		float fTargetRadius,		// ����ڕW���a
		float fTargetHeight			// ����ڕW�c��
	);

	bool ResponseSingleX	// X���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = nullptr,	// �ړ���
		bool *pLeft = nullptr,			// ������̔���
		bool *pRight = nullptr			// �E����̔���
	);
	bool ResponseSingleY	// Y���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = nullptr,	// �ړ���
		bool *pDown = nullptr,			// ������̔���
		bool *pUp = nullptr			// �ォ��̔���
	);
	bool ResponseSingleZ	// Z���̏Փ˔���
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
		D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
		D3DXVECTOR3 targetSizeDown,	// ����ڕW�T�C�Y(���E���E�O)
		D3DXVECTOR3 *pMove = nullptr,	// �ړ���
		bool *pBefore = nullptr,		// �O����̔���
		bool *pAfter = nullptr			// �ォ��̔���
	);

	bool InBoxPillar	// �p���̓�������
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 originPos,		// ���茴�_�ʒu
		D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
		D3DXVECTOR3 originSizeUp,	// ���茴�_�T�C�Y(�E�E��E��)
		D3DXVECTOR3 originSizeDown	// ���茴�_�T�C�Y(���E���E�O)
	);
	bool InCirclePillar	// �~���̓�������
	( // ����
		D3DXVECTOR3& rCenterPos,	// ����ʒu
		D3DXVECTOR3 originPos,		// ���茴�_�ʒu
		float fCenterRadius,		// ���蔼�a
		float fOriginRadius			// ���茴�_���a
	);

	float LineOuterProduct	// �O�ς̍��E����
	( // ����
		D3DXVECTOR3 posLeft,	// ���E�������W
		D3DXVECTOR3 posRight,	// ���E���E���W
		D3DXVECTOR3 pos			// ����ʒu
	);
	float LineHeightOuterProduct	// �O�ς̏㉺����
	( // ����
		D3DXVECTOR3 posLeft,	// ���E�������W
		D3DXVECTOR3 posRight,	// ���E���E���W
		D3DXVECTOR3 pos			// ����ʒu
	);

	bool TriangleOuterPillar	// �O�p���̓����蔻��
	( // ����
		D3DXVECTOR3 pos0,	// �O�p�̊e���_
		D3DXVECTOR3 pos1,	// �O�p�̊e���_
		D3DXVECTOR3 pos2,	// �O�p�̊e���_
		D3DXVECTOR3 pos		// ����ʒu
	);
	bool BoxOuterPillar		// �l�p���̓����蔻��
	( // ����
		D3DXVECTOR3 pos0,	// �l�p�̊e���_
		D3DXVECTOR3 pos1,	// �l�p�̊e���_
		D3DXVECTOR3 pos2,	// �l�p�̊e���_
		D3DXVECTOR3 pos3,	// �l�p�̊e���_
		D3DXVECTOR3 pos		// ����ʒu
	);
}

#endif	// _COLLISION_H_