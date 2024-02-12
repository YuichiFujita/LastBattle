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
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEdit::~CEdit()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEdit::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_mode = MODE_PLAY;	// �G�f�B�b�g���[�h

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEdit::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEdit::Update(void)
{
#if _DEBUG

	if (GET_INPUTKEY->IsTrigger(KEY_MODE_CHANGE))
	{
		CEdit *pEdit = CSceneGame::GetEdit();	// �G�f�B�b�g�擾
		CEdit::NextMode(pEdit);					// �G�f�B�b�g�ύX
		CSceneGame::SetEdit(pEdit);				// �G�f�B�b�g�ݒ�
	}

#endif	// _DEBUG
}

//============================================================
//	�`�揈��
//============================================================
void CEdit::Draw(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g����\��
//============================================================
void CEdit::DrawEditControl(void)
{
#if _DEBUG

	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g����]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "�G�f�B�b�g���[�h�ύX�F[%s]\n", NAME_MODE_CHANGE);

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g���\��
//============================================================
void CEdit::DrawEditData(void)
{
#if _DEBUG

	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g���]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g���[�h]\n", MODE_NAME[m_mode]);

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g���[�h�擾����
//============================================================
CEdit::EMode CEdit::GetMode(void)
{
#if _DEBUG

	// �G�f�B�b�g���[�h��Ԃ�
	return m_mode;

#else	// NDEBUG

	return (EMode)0;

#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
#if _DEBUG

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

#else	// NDEBUG

CEdit *CEdit::Create(const EMode /*mode*/)
{
	return nullptr;
}

#endif	// _DEBUG

//============================================================
//	�j������
//============================================================
#if _DEBUG

void CEdit::Release(CEdit *&prEdit)
{
	// �G�f�B�b�g�̏I��
	assert(prEdit != nullptr);
	prEdit->Uninit();

	// �������J��
	SAFE_DELETE(prEdit);
}

#else	// NDEBUG

void CEdit::Release(CEdit *& /*prEdit*/)
{

}

#endif	// _DEBUG

//============================================================
//	���̃��[�h�ݒ菈��
//============================================================
#if _DEBUG

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

#else	// NDEBUG

void CEdit::NextMode(CEdit *& /*prEdit*/)
{

}

#endif	// _DEBUG
