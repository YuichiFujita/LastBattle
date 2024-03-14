//============================================================
//
//	���i�U�����w�b�_�[ [attackMoveFire.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ATTACK_MOVE_FIRE_H_
#define _ATTACK_MOVE_FIRE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fire.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���i�U�����N���X
class CAttackMoveFire : public CFire
{
public:
	// �R���X�g���N�^
	CAttackMoveFire();

	// �f�X�g���N�^
	~CAttackMoveFire() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CAttackMoveFire *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �����ʒu
		const D3DXVECTOR3& rVecMove,	// �ړ�����
		const float fMove,				// �ړ���
		const float fAddMove = 0.0f,	// ������
		const int nLife = NONE_IDX		// ����
	);

	// �����o�֐�
	void SetMoveDir(const D3DXVECTOR3& rMove);	// �ړ������̐ݒ�
	D3DXVECTOR3 GetMoveDir(void) const			{ return m_dirMove; }		// �ړ������̎擾
	void SetMoveValue(const float fMove)		{ m_fMove = fMove; }		// �ړ��ʂ̐ݒ�
	float GetMoveValue(void) const				{ return m_fMove; }			// �ړ��ʂ̎擾
	void SetAddMoveValue(const float fAddMove)	{ m_fAddMove = fAddMove; }	// �����ʂ̐ݒ�
	float GetAddMoveValue(void) const			{ return m_fAddMove; }		// �����ʂ̎擾

private:
	// �����o�֐�
	void UpdateMove(void);	// �ړ��̍X�V

	// �����o�֐�
	D3DXVECTOR3 m_dirMove;	// �ړ������x�N�g��
	float m_fMove;		// �ړ���
	float m_fAddMove;	// ������
};

#endif	// _ATTACK_MOVE_FIRE_H_
