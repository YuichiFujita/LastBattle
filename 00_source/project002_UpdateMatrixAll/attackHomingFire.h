//============================================================
//
//	�z�[�~���O�U�����w�b�_�[ [attackHomingFire.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ATTACK_HOMING_FIRE_H_
#define _ATTACK_HOMING_FIRE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fire.h"

//************************************************************
//	�N���X��`
//************************************************************
// �z�[�~���O�U�����N���X
class CAttackHomingFire : public CFire
{
public:
	// �R���X�g���N�^
	CAttackHomingFire();

	// �f�X�g���N�^
	~CAttackHomingFire() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;		// ������
	void Uninit(void) override;			// �I��
	void Update(void) override;			// �X�V
	void UpdateMatrix(void) override;	// �}�g���b�N�X�X�V
	void Draw(void) override;			// �`��

	// �ÓI�����o�֐�
	static CAttackHomingFire *Create(const D3DXVECTOR3& rPos);	// ����

private:
	// �����o�֐�
	void UpdateHomingMove(void);	// ���ړ��ʂ̍X�V
	void CalcHoming(float *pCurRot, float fDestRot);	// �z�[�~���O�̌v�Z
};

#endif	// _ATTACK_HOMING_FIRE_H_
