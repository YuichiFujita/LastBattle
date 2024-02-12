//============================================================
//
//	�G�f�B�b�g�����蔻�菈�� [editColl.cpp]
//	Author�F���c�E��
//
//============================================================
#if _DEBUG 

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editColl.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "collSphere.h"
#include "debugCollSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SAVE_TXT = "data\\TXT\\save_collision.txt";	// ����ۑ��e�L�X�g

	const float INIT_COLL_RAD = 20.0f;	// �������̔��蔼�a
	const float MOVE_OFFSET	= 2.5f;		// �I�t�Z�b�g�ړ���
	const float MOVE_RAD	= 1.0f;		// ���a�ύX��
	const float MIN_RAD		= 1.0f;		// �ŏ����a
	const float MAX_RAD		= 1000.0f;	// �ő唼�a
}

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��

#define KEY_SAVE	(DIK_F5)	// �ۑ��L�[
#define NAME_SAVE	("F5")		// �ۑ��\��

#define KEY_ADD_COLL	(DIK_0)		// ����ǉ��L�[
#define NAME_ADD_COLL	("0")		// ����ǉ��\��
#define KEY_SUB_COLL	(DIK_9)		// ����폜�L�[
#define NAME_SUB_COLL	("9")		// ����폜�\��
#define KEY_UP_PARTS	(DIK_UP)	// �p�[�c�ύX�L�[
#define NAME_UP_PARTS	("��")		// �p�[�c�ύX�\��
#define KEY_DOWN_PARTS	(DIK_DOWN)	// �p�[�c�ύX�L�[
#define NAME_DOWN_PARTS	("��")		// �p�[�c�ύX�\��
#define KEY_UP_COLL		(DIK_LEFT)	// ����ύX�L�[
#define NAME_UP_COLL	("��")		// ����ύX�\��
#define KEY_DOWN_COLL	(DIK_RIGHT)	// ����ύX�L�[
#define NAME_DOWN_COLL	("��")		// ����ύX�\��

#define KEY_FAR		(DIK_W)	// ���ړ��L�[
#define NAME_FAR	("W")	// ���ړ��\��
#define KEY_NEAR	(DIK_S)	// ��O�ړ��L�[
#define NAME_NEAR	("S")	// ��O�ړ��\��
#define KEY_RIGHT	(DIK_D)	// �E�ړ��L�[
#define NAME_RIGHT	("D")	// �E�ړ��\��
#define KEY_LEFT	(DIK_A)	// ���ړ��L�[
#define NAME_LEFT	("A")	// ���ړ��\��
#define KEY_UP		(DIK_E)	// ��ړ��L�[
#define NAME_UP		("E")	// ��ړ��\��
#define KEY_DOWN	(DIK_Q)	// ���ړ��L�[
#define NAME_DOWN	("Q")	// ���ړ��\��

#define KEY_RAD_UP		(DIK_R)	// ����g��L�[
#define NAME_RAD_UP		("R")	// ����g��\��
#define KEY_RAD_DOWN	(DIK_F)	// ����k���L�[
#define NAME_RAD_DOWN	("F")	// ����k���\��

//************************************************************
//	�q�N���X [CEditColl] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditColl::CEditColl() :
	m_nCurSelectParts	(0),	// ���݂̑I���p�[�c
	m_nOldSelectParts	(0),	// �O��̑I���p�[�c
	m_nCurSelectColl	(0),	// ���݂̑I�𔻒�
	m_nOldSelectColl	(0)		// �O��̑I�𔻒�

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
	m_nCurSelectParts = 0;	// ���݂̑I���p�[�c
	m_nOldSelectParts = 0;	// �O��̑I���p�[�c
	m_nCurSelectColl = 0;	// ���݂̑I�𔻒�
	m_nOldSelectColl = 0;	// �O��̑I�𔻒�

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
	// �}�e���A�����Đݒ�
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	pPlayer->ResetMaterial();

	// �O��̑I�𔻒�̐F�����ɖ߂�
	CCollSphere *pOldColl = pPlayer->GetCollision(m_nOldSelectParts);	// �O��̉~������
	std::vector<CCollSphere::SInfo> oldVector = pOldColl->GetVector();	// �O��̉~����z��
	if (oldVector.size() > 0)
	{ // �O��p�[�c�̔z�񂪑��݂���ꍇ

		CCollSphere::SInfo oldInfo = pOldColl->GetInfo(m_nOldSelectColl);	// �O��̔�����
		oldInfo.pVisual->SetColor(debugCollSphere::DEFAULT_COL);	// �F��ݒ�
		pOldColl->SetInfo(oldInfo, m_nCurSelectColl);				// ������𔽉f
	}

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

	// ����ǉ��̍X�V
	AddColl();

	// ����I���̍X�V
	SelectColl();

	// ����폜�̍X�V
	SubColl();

	// ����I�t�Z�b�g�̍X�V
	UpdateCollOffset();

	// ���蔼�a�̍X�V
	UpdateCollRadius();

	// ������̍X�V
	UpdateCollInfo();

	// ����ۑ�
	SaveColl();

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

	pDebug->Print(CDebugProc::POINT_RIGHT, "�ۑ��F[%s]\n", NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�p�[�c�ύX�F[%s/%s]\n", NAME_UP_PARTS, NAME_DOWN_PARTS);
	pDebug->Print(CDebugProc::POINT_RIGHT, "����ύX�F[%s/%s]\n", NAME_UP_COLL, NAME_DOWN_COLL);
	pDebug->Print(CDebugProc::POINT_RIGHT, "����ǉ�/�폜�F[%s/%s]\n", NAME_ADD_COLL, NAME_SUB_COLL);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�I�t�Z�b�g�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "���a�g�k�F[%s/%s+%s]\n", NAME_RAD_UP, NAME_RAD_DOWN, NAME_TRIGGER);
}

//============================================================
//	�G�f�B�b�g���\��
//============================================================
void CEditColl::DrawEditData(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�̏��
	CPlayer *pPlayer = CScene::GetPlayer();							// �v���C���[���
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// �~������
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��

	// �G�f�B�b�g���\��
	CEdit::DrawEditData();

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[���p�[�c��]\n", pPlayer->GetNumModel());
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�I���p�[�c]\n", m_nCurSelectParts);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�����萔]\n", vector.size());
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�I�𔻒�]\n", m_nCurSelectColl);
	pDebug->Print(CDebugProc::POINT_RIGHT, "\n");

	int nCntColl = 0;	// �J��Ԃ���
	for (auto info : vector)
	{ // ���萔���J��Ԃ�

		nCntColl++;
		pDebug->Print(CDebugProc::POINT_RIGHT, "******************** [����ԍ��F%d] ***\n", nCntColl);
		pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[�I�t�Z�b�g]\n", info.offset.x, info.offset.y, info.offset.z);
		pDebug->Print(CDebugProc::POINT_RIGHT, "%f�F[���a]\n", info.fRadius);
	}
}

//============================================================
//	�p�[�c�I���̍X�V����
//============================================================
void CEditColl::SelectParts(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// �O��̑I���p�[�c��ݒ�
	m_nOldSelectParts = m_nCurSelectParts;

	// �I���p�[�c�ύX
	if (pKey->IsTrigger(KEY_UP_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// �p�[�c�̑���
		m_nCurSelectParts = (m_nCurSelectParts + (nNumParts - 1)) % nNumParts;
	}
	else if (pKey->IsTrigger(KEY_DOWN_PARTS))
	{
		int nNumParts = pPlayer->GetNumModel();	// �p�[�c�̑���
		m_nCurSelectParts = (m_nCurSelectParts + 1) % nNumParts;
	}

	// �}�e���A�����Đݒ�
	pPlayer->ResetMaterial();

	// �I���p�[�c�̃}�e���A����ύX
	pPlayer->SetPartsMaterial(material::Blue(), m_nCurSelectParts);
}

//============================================================
//	����ǉ��̍X�V����
//============================================================
void CEditColl::AddColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// ����ǉ�
	if (pKey->IsTrigger(KEY_ADD_COLL))
	{
		CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// ������
		pColl->AddColl(VEC3_ZERO, INIT_COLL_RAD);
	}
}

//============================================================
//	����폜�̍X�V����
//============================================================
void CEditColl::SubColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// �~������
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��

	// ����폜
	if (vector.size() > 0)
	{ // �z�񂪑��݂���ꍇ

		if (pKey->IsTrigger(KEY_SUB_COLL))
		{
			// �I�𒆂̔�����폜
			pColl->SubColl(m_nCurSelectColl);

			// �폜��̔z��̗v�f�����傫���l�̏ꍇ�␳
			int nMaxSelect = (int)pColl->GetVector().size() - 1;	// �ō��I���\�C���f�b�N�X
			useful::LimitMinNum(nMaxSelect, 0);						// 0��菬�����ꍇ�␳
			useful::LimitNum(m_nCurSelectColl, 0, nMaxSelect);		// �I�𒆂̗v�f�ԍ���␳
		}
	}
}

//============================================================
//	����I���̍X�V����
//============================================================
void CEditColl::SelectColl(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	// �O��̑I�𔻒��ݒ�
	m_nOldSelectColl = m_nCurSelectColl;

	CCollSphere *pOldColl = pPlayer->GetCollision(m_nOldSelectParts);	// �O��̉~������
	std::vector<CCollSphere::SInfo> oldVector = pOldColl->GetVector();	// �O��̉~����z��
	if (oldVector.size() > 0)
	{ // �O��p�[�c�̔z�񂪑��݂���ꍇ

		// �O��̑I�𔻒�̐F�����ɖ߂�
		CCollSphere::SInfo oldInfo = pOldColl->GetInfo(m_nOldSelectColl);	// �O��̔�����
		oldInfo.pVisual->SetColor(debugCollSphere::DEFAULT_COL);	// �F��ݒ�
		pOldColl->SetInfo(oldInfo, m_nCurSelectColl);				// ������𔽉f
	}

	CCollSphere *pCurColl = pPlayer->GetCollision(m_nCurSelectParts);	// ���݂̉~������
	std::vector<CCollSphere::SInfo> curVector = pCurColl->GetVector();	// ���݂̉~����z��
	if (curVector.size() > 0)
	{ // ���݃p�[�c�̔z�񂪑��݂���ꍇ

		// �I�𔻒�ύX
		if (pKey->IsTrigger(KEY_UP_COLL))
		{
			int nNumColl = curVector.size();	// ����̑���
			m_nCurSelectColl = (m_nCurSelectColl + (nNumColl - 1)) % nNumColl;
		}
		else if (pKey->IsTrigger(KEY_DOWN_COLL))
		{
			int nNumColl = curVector.size();	// ����̑���
			m_nCurSelectColl = (m_nCurSelectColl + 1) % nNumColl;
		}

		// ���݂̑I�𔻒�̐F��ݒ�
		CCollSphere::SInfo curInfo = pCurColl->GetInfo(m_nCurSelectColl);	// ���݂̔�����
		curInfo.pVisual->SetColor(debugCollSphere::CHOICE_COL);	// �F��ݒ�
		pCurColl->SetInfo(curInfo, m_nCurSelectColl);			// ������𔽉f
	}
}

//============================================================
//	����I�t�Z�b�g�̍X�V����
//============================================================
void CEditColl::UpdateCollOffset(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// �~������
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��

	// �I�𔻒�ύX
	if (vector.size() > 0)
	{ // �z�񂪑��݂���ꍇ

		CCollSphere::SInfo curInfo = pColl->GetInfo(m_nCurSelectColl);	// ������
		bool bTrigger = pKey->IsPress(KEY_TRIGGER);	// �g���K�[��

		if ((bTrigger) ? pKey->IsTrigger(KEY_FAR) : pKey->IsPress(KEY_FAR))
		{
			curInfo.offset.z += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_NEAR) : pKey->IsPress(KEY_NEAR))
		{
			curInfo.offset.z -= MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_RIGHT) : pKey->IsPress(KEY_RIGHT))
		{
			curInfo.offset.x += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_LEFT) : pKey->IsPress(KEY_LEFT))
		{
			curInfo.offset.x -= MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_UP) : pKey->IsPress(KEY_UP))
		{
			curInfo.offset.y += MOVE_OFFSET;
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_DOWN) : pKey->IsPress(KEY_DOWN))
		{
			curInfo.offset.y -= MOVE_OFFSET;
		}

		// ������𔽉f
		pColl->SetInfo(curInfo, m_nCurSelectColl);
	}
}

//============================================================
//	���蔼�a�̍X�V����
//============================================================
void CEditColl::UpdateCollRadius(void)
{
	CInputKeyboard *pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	CCollSphere *pColl = pPlayer->GetCollision(m_nCurSelectParts);	// �~������
	std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��

	// �I�𔻒�ύX
	if (vector.size() > 0)
	{ // �z�񂪑��݂���ꍇ

		CCollSphere::SInfo curInfo = pColl->GetInfo(m_nCurSelectColl);	// ������
		bool bTrigger = pKey->IsPress(KEY_TRIGGER);	// �g���K�[��

		if ((bTrigger) ? pKey->IsTrigger(KEY_RAD_UP) : pKey->IsPress(KEY_RAD_UP))
		{
			curInfo.fRadius += MOVE_RAD;
			useful::LimitMaxNum(curInfo.fRadius, MAX_RAD);	// ���a��␳
		}
		else if ((bTrigger) ? pKey->IsTrigger(KEY_RAD_DOWN) : pKey->IsPress(KEY_RAD_DOWN))
		{
			curInfo.fRadius -= MOVE_RAD;
			useful::LimitMinNum(curInfo.fRadius, MIN_RAD);	// ���a��␳
		}

		// ������𔽉f
		pColl->SetInfo(curInfo, m_nCurSelectColl);
	}
}

//============================================================
//	������̍X�V����
//============================================================
void CEditColl::UpdateCollInfo(void)
{
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
	for (int nCntParts = 0; nCntParts < pPlayer->GetNumModel(); nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �����蔻��̍X�V
		pPlayer->GetCollision(nCntParts)->Update();
	}
}

//============================================================
//	����ۑ�����
//============================================================
void CEditColl::SaveColl(void)
{
	if (!GET_INPUTKEY->IsTrigger(KEY_SAVE)) { return; }	// �Z�[�u���삳��Ă��Ȃ��ꍇ������

	// �t�@�C���������o���`���ŊJ��
	FILE *pFile = fopen(SAVE_TXT, "w");
	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	����Z�[�u�e�L�X�g [save_collison.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- �������牺�� �R�s�[���\��t�� ---------->--<----------\n\n");

		fprintf(pFile, "SETCOLLISION\n\n");

		CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���
		for (int nCntParts = 0; nCntParts < pPlayer->GetNumModel(); nCntParts++)
		{ // �p�[�c�̍ő吔���J��Ԃ�

			CCollSphere *pColl = pPlayer->GetCollision(nCntParts);			// �~������
			std::vector<CCollSphere::SInfo> vector = pColl->GetVector();	// �~����z��
			if (vector.size() <= 0) { continue; }	// ���肪����Ȃ��ꍇ���̃��[�v��

			fprintf(pFile, "	SETCOLL\n\n");

			fprintf(pFile, "		PARTS = %d\n", nCntParts);
			fprintf(pFile, "		NUMCOLL = %d\n\n", (int)vector.size());

			for (auto info : vector)
			{ // ���萔���J��Ԃ�

				fprintf(pFile, "		COLL\n");
				fprintf(pFile, "			OFFSET = %.2f %.2f %.2f\n", info.offset.x, info.offset.y, info.offset.z);
				fprintf(pFile, "			RADIUS = %.2f\n", info.fRadius);
				fprintf(pFile, "		END_COLL\n\n");
			}

			fprintf(pFile, "	END_SETCOLL\n\n");
		}

		fprintf(pFile, "END_SETCOLLISION\n\n");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "����ۑ��t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

#endif	// _DEBUG
