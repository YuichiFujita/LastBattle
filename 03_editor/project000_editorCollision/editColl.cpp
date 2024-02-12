//============================================================
//
//	�G�f�B�b�g�����蔻�菈�� [editColl.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editColl.h"
#include "manager.h"
#include "scene.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{

}

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_UP_PARTS	(DIK_UP)	// �p�[�c�ύX�L�[
#define NAME_UP_PARTS	("��")		// �p�[�c�ύX�\��
#define KEY_DOWN_PARTS	(DIK_DOWN)	// �p�[�c�ύX�L�[
#define NAME_DOWN_PARTS	("��")		// �p�[�c�ύX�\��

//************************************************************
//	�q�N���X [CEditColl] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditColl::CEditColl() :
	m_nSelectParts	(0)	// �I���p�[�c
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditColl::~CEditColl()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEditColl::Init(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// �����o�ϐ���������
	m_nSelectParts = 0;	// �I���p�[�c

	// �G�f�B�b�g�̏�����
	if (FAILED(CEdit::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�[�������� (T�|�[�Y�ɂ���)
	pPlayer->GetMotion()->Reset();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEditColl::Uninit(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// �}�e���A�����Đݒ�
	pPlayer->ResetMaterial();

	// �G�f�B�b�g�̏I��
	CEdit::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEditColl::Update(void)
{
	// �p�[�c�I���̍X�V
	SelectParts();

	// �G�f�B�b�g�̍X�V
	CEdit::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEditColl::Draw(void)
{
	// �G�f�B�b�g�̕`��
	CEdit::Draw();
}

//============================================================
//	�G�f�B�b�g����\��
//============================================================
void CEditColl::DrawEditControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	// �G�f�B�b�g����\��
	CEdit::DrawEditControl();

	pDebug->Print(CDebugProc::POINT_RIGHT, "�p�[�c�ύX�F[%s/%s]\n", NAME_UP_PARTS, NAME_DOWN_PARTS);
}

//============================================================
//	�G�f�B�b�g���\��
//============================================================
void CEditColl::DrawEditData(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��

	// �G�f�B�b�g���\��
	CEdit::DrawEditData();

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�p�[�c]\n", m_nSelectParts);
}

//============================================================
//	�p�[�c�I���̍X�V����
//============================================================
void CEditColl::SelectParts(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���

	// �I���p�[�c�ύX
	if (pKey->IsTrigger(KEY_UP_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// �p�[�c�̑���
		m_nSelectParts = (m_nSelectParts + (nNumParts - 1)) % nNumParts;
	}
	else if (pKey->IsTrigger(KEY_DOWN_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// �p�[�c�̑���
		m_nSelectParts = (m_nSelectParts + 1) % nNumParts;
	}

	// �}�e���A�����Đݒ�
	pPlayer->ResetMaterial();

	// �I���p�[�c�̃}�e���A����ύX
	pPlayer->SetPartsMaterial(material::Blue(), m_nSelectParts);
}
