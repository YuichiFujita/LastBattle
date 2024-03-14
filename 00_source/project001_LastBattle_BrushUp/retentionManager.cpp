//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[���� [retentionManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"
#include "manager.h"

//************************************************************
//	�e�N���X [CRetentionManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRetentionManager::CRetentionManager()
{
	// �����o�ϐ����N���A
	memset(&m_result, 0, sizeof(m_result));	// ���U���g���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// �����o�ϐ���������
	m_result.win	= WIN_NONE;	// ������
	m_result.nTime	= 0;		// �o�߃^�C��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	��������
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// �f�[�^�ۑ��}�l�[�W���[�̐���
	CRetentionManager *pRetentionManager = new CRetentionManager;
	if (pRetentionManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �f�[�^�ۑ��}�l�[�W���[�̏�����
		if (FAILED(pRetentionManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �f�[�^�ۑ��}�l�[�W���[�̔j��
			SAFE_DELETE(pRetentionManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRetentionManager;
	}
}

//============================================================
//	�j������
//============================================================
void CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	// �f�[�^�ۑ��}�l�[�W���[�̏I��
	assert(prRetentionManager != nullptr);
	prRetentionManager->Uninit();

	// �������J��
	SAFE_DELETE(prRetentionManager);
}

//============================================================
//	���U���g���̐ݒ菈��
//============================================================
void CRetentionManager::SetResult(const EWin win, const long nTime)
{
	if (win <= WIN_NONE || win >= WIN_MAX) { assert(false); return; }	// ���������K�ł͂Ȃ�

	// �����̃N���A�󋵂�ݒ�
	m_result.win = win;

	// �����̌o�߃^�C����ݒ�
	m_result.nTime = nTime;
}

//============================================================
//	�����󋵂̎擾����
//============================================================
CRetentionManager::EWin CRetentionManager::GetWin(void) const
{
	// �����󋵂�Ԃ�
	return m_result.win;
}

//============================================================
//	�o�߃^�C���̎擾����
//============================================================
long CRetentionManager::GetTime(void) const
{
	// �o�߃^�C����Ԃ�
	return m_result.nTime;
}
