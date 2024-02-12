//============================================================
//
//	�G�f�B�b�g�����蔻��w�b�_�[ [editColl.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_COLL_H_
#define _EDIT_COLL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "edit.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�����蔻��N���X
class CEditColl : public CEdit
{
public:
	// �R���X�g���N�^
	CEditColl();

	// �f�X�g���N�^
	~CEditColl();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void DrawEditControl(void) override;	// �G�f�B�b�g����\��
	void DrawEditData(void) override;		// �G�f�B�b�g���\��

private:
	// �����o�֐�
	void SelectParts(void);			// �p�[�c�I���X�V
	void AddColl(void);				// ����ǉ��X�V
	void SubColl(void);				// ����폜�X�V
	void SelectColl(void);			// ����I���X�V
	void UpdateCollOffset(void);	// ����I�t�Z�b�g�X�V
	void UpdateCollRadius(void);	// ���蔼�a�X�V
	void UpdateCollInfo(void);		// ������X�V

	// �����o�ϐ�
	int m_nCurSelectParts;	// ���݂̑I���p�[�c
	int m_nOldSelectParts;	// �O��̑I���p�[�c
	int m_nCurSelectColl;	// ���݂̑I�𔻒�
	int m_nOldSelectColl;	// �O��̑I�𔻒�
};

#endif	// _EDIT_COLL_H_
