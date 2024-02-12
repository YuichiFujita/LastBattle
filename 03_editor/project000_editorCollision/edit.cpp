//============================================================
//
//	�G�f�B�b�g���� [edit.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "edit.h"
#include "editPlay.h"
#include "editColl.h"

#include "manager.h"
#include "camera.h"
#include "sceneGame.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *MODE_NAME[] =	// ���[�h��
	{
		"�v���C",	// �v���C���[�h
		"����쐬",	// ����ݒ胂�[�h
	};
}

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_MODE_CHANGE		(DIK_F4)	// �G�f�B�b�g���[�h�ύX�L�[
#define NAME_MODE_CHANGE	("F4")		// �G�f�B�b�g���[�h�ύX�\��

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(MODE_NAME) == CEdit::MODE_MAX, "ERROR : Mode Count Mismatch");

//************************************************************
//	�q�N���X [CEdit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEdit::CEdit() : m_mode(MODE_PLAY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEdit::~CEdit()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEdit::Init(void)
{
	// �����o�ϐ���������
	m_mode = MODE_PLAY;	// �G�f�B�b�g���[�h

	// ����J������ݒ�
	CCamera *pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_CONTROL);		// ����J�����ݒ�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEdit::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CEdit::Update(void)
{
	if (GET_INPUTKEY->IsTrigger(KEY_MODE_CHANGE))
	{
		CEdit *pEdit = CSceneGame::GetEdit();	// �G�f�B�b�g�擾
		CEdit::NextMode(pEdit);					// �G�f�B�b�g�ύX
		CSceneGame::SetEdit(pEdit);				// �G�f�B�b�g�ݒ�
	}
}

//============================================================
//	�`�揈��
//============================================================
void CEdit::Draw(void)
{

}

//============================================================
//	�G�f�B�b�g����\��
//============================================================
void CEdit::DrawEditControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g����]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "�G�f�B�b�g���[�h�ύX�F[%s]\n", NAME_MODE_CHANGE);
}

//============================================================
//	�G�f�B�b�g���\��
//============================================================
void CEdit::DrawEditData(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g���]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g���[�h]\n", MODE_NAME[m_mode]);
}

//============================================================
//	�G�f�B�b�g���[�h�擾����
//============================================================
CEdit::EMode CEdit::GetMode(void)
{
	// �G�f�B�b�g���[�h��Ԃ�
	return m_mode;
}

//============================================================
//	��������
//============================================================
CEdit *CEdit::Create(const EMode mode)
{
	// �G�f�B�b�g�̐���
	CEdit *pEdit = nullptr;
	switch (mode)
	{ // ���[�h���Ƃ̏���
	case MODE_PLAY:
		pEdit = new CEditPlay;
		break;

	case MODE_COLL_SET:
		pEdit = new CEditColl;
		break;

	default:
		assert(false);
		break;
	}

	if (pEdit == nullptr)
	{ // �������Ă��Ȃ��ꍇ

		return nullptr;
	}
	else
	{ // �������Ă���ꍇ

		// �G�f�B�b�g�̏�����
		if (FAILED(pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�b�g�̔j��
			SAFE_DELETE(pEdit);
			return nullptr;
		}

		// ���݂̃��[�h��ۑ�
		pEdit->m_mode = mode;

		// �m�ۂ����A�h���X��Ԃ�
		return pEdit;
	}
}

//============================================================
//	�j������
//============================================================
void CEdit::Release(CEdit *&prEdit)
{
	// �G�f�B�b�g�̏I��
	assert(prEdit != nullptr);
	prEdit->Uninit();

	// �������J��
	SAFE_DELETE(prEdit);
}

//============================================================
//	���̃��[�h�ݒ菈��
//============================================================
void CEdit::NextMode(CEdit *&prEdit)
{
	EMode nextMode = (EMode)((prEdit->m_mode + 1) % MODE_MAX);	// ���̃��[�h

	// �G�f�B�b�g�̔j��
	assert(prEdit != nullptr);
	SAFE_REF_RELEASE(prEdit);

	// �G�f�B�b�g�̐���
	assert(prEdit == nullptr);
	prEdit = CEdit::Create(nextMode);
}
