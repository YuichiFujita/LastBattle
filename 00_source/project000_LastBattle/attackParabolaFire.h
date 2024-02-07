//============================================================
//
//	�J�[�u�U�����w�b�_�[ [attackParabolaFire.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ATTACK_PARABOLA_FIRE_H_
#define _ATTACK_PARABOLA_FIRE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fire.h"

//************************************************************
//	�N���X��`
//************************************************************
// �J�[�u�U�����N���X
class CAttackParabolaFire : public CFire
{
public:
	// �R���X�g���N�^
	CAttackParabolaFire();

	// �f�X�g���N�^
	~CAttackParabolaFire() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CAttackParabolaFire *Create	// ����
	( // ����
		const D3DXVECTOR3 &rPos,	// �ʒu
		const float fMove			// �ړ���
	);

private:
	// �����o�֐�
	void UpdateParabolaMove(void);	// ���ړ��ʂ̍X�V

	// �����o�ϐ�
	int m_nCounterTime;	// �U�����ԊǗ��J�E���^�[
	float m_fMove;		// �ړ���
};

#endif	// _ATTACK_PARABOLA_FIRE_H_
