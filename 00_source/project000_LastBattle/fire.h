//============================================================
//
//	���w�b�_�[ [fire.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FIRE_H_
#define _FIRE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject3D;	// �I�u�W�F�N�g3D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CFire : public CObject
{
public:
	// �R���X�g���N�^
	CFire();

	// �f�X�g���N�^
	~CFire() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾

	// �ÓI�����o�֐�
	static CFire *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos	// �ʒu
	);
	static CListManager<CFire> *GetList(void);	// ���X�g�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static CListManager<CFire> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CFire>::AIterator m_iterator;	// �C�e���[�^�[
	D3DXVECTOR3	m_pos;	// �ʒu
	D3DXVECTOR3	m_move;	// �ړ���
};

#endif	// _FIRE_H_
