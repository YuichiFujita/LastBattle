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
	explicit CAttackParabolaFire(const D3DXVECTOR3& rPosDest);

	// �f�X�g���N�^
	~CAttackParabolaFire() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CAttackParabolaFire *Create	// ����
	( // ����
		const D3DXVECTOR3& rCurPos,		// �����ʒu
		const D3DXVECTOR3& rDestPos,	// �ڕW�ʒu
		const float fMove				// �ړ���
	);

private:
	// �����o�֐�
	void InitParabolaParam(const D3DXVECTOR3& rCurPos);	// ���ړ��p�����[�^�[�̏�����
	void UpdateParabolaMove(void);	// ���ړ��ʂ̍X�V

	// �����o�ϐ�
	const D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	D3DXVECTOR3 m_posOrigin;		// ���_�ʒu
	float m_fLength;	// �ړ�����
	float m_fPhi;		// ���ʊp
	float m_fTheta;		// �p
	float m_fMove;		// �ړ���
	int m_nCounterTime;	// �U�����ԊǗ��J�E���^�[
};

#endif	// _ATTACK_PARABOLA_FIRE_H_
