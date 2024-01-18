//============================================================
//
//	���[�V�������� [motion.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "motion.h"
#include "multiModel.h"

//************************************************************
//	�e�N���X [CMotion] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMotion::CMotion() :
	m_ppModel	(nullptr),	// ���f�����
	m_nNumModel	(0),		// ���f���̃p�[�c��
	m_bUpdate	(true)		// �X�V��
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMotion::Init(void)
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
	m_ppModel	= nullptr;	// ���f�����
	m_nNumModel	= 0;		// ���f���̃p�[�c��
	m_bUpdate	= true;		// �X�V��

	// ���[�V�������I����Ԃɂ���
	m_info.bFinish = true;

	for (int nCntMotion = 0; nCntMotion < motion::MAX_MOTION; nCntMotion++)
	{ // ���[�V�����̍ő吔���J��Ԃ�

		// �U���������������
		m_info.aMotionInfo[nCntMotion].collLeft.nMin  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collLeft.nMax  = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMin = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].collRight.nMax = NONE_IDX;

		// ����\����OFF�ɂ���
		m_info.aMotionInfo[nCntMotion].bWeaponDisp = false;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMotion::Update(void)
{
	if (!m_bUpdate)										{ return; }	// �X�V���Ȃ�
	if (m_info.aMotionInfo[m_info.nType].nNumKey <= 0)	{ return; }	// �L�[�����ݒ�

	// �ϐ���錾
	D3DXVECTOR3 diffPos;		// ���|�[�Y�܂ł̍����ʒu
	D3DXVECTOR3 diffRot;		// ���|�[�Y�܂ł̍�������
	D3DXVECTOR3 currentPos;		// ���݃t���[���̈ʒu
	D3DXVECTOR3 currentRot;		// ���݃t���[���̌���
	int nType = m_info.nType;	// ���[�V�������
	int nPose = m_info.nPose;	// ���[�V�����|�[�Y�ԍ�
	int nNextPose;				// ���̃��[�V�����|�[�Y�ԍ�

	// ���̃��[�V�����|�[�Y�ԍ������߂�
	nNextPose = (nPose + 1) % m_info.aMotionInfo[nType].nNumKey;

	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos;
		diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot;

		// ���������̐��K��
		useful::Vec3NormalizeRot(diffRot);

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_info.nKeyCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_info.nKeyCounter < m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame)
	{ // ���݂̃|�[�Y�̍Đ����I�����Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // ���݂̃|�[�Y�̍Đ����I�������ꍇ

		// ���̃|�[�Y�Ɉڍs
		if (m_info.aMotionInfo[nType].bLoop)
		{ // ���[�V���������[�v����ꍇ

			// �L�[�J�E���^�[��������
			m_info.nKeyCounter = 0;

			// �|�[�Y�J�E���g�����Z
			m_info.nPose = (m_info.nPose + 1) % m_info.aMotionInfo[nType].nNumKey;	// �ő�l��0�ɖ߂�

			if (m_info.nPose == 0)
			{ // �|�[�Y���ŏ��ɖ߂����ꍇ

				// �S�̃J�E���^�[��������
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // ���[�V���������[�v���Ȃ��ꍇ

			if (m_info.nPose < m_info.aMotionInfo[nType].nNumKey - 2)
			{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�ł͂Ȃ��ꍇ

				// �L�[�J�E���^�[��������
				m_info.nKeyCounter = 0;

				// �|�[�Y�J�E���g�����Z
				m_info.nPose++;
			}
			else
			{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�̏ꍇ

				// ���[�V�������I����Ԃɂ���
				m_info.bFinish = true;
			}
		}
	}
}

//============================================================
//	�ݒ菈��
//============================================================
void CMotion::Set(const int nType)
{
	// ���[�V��������������
	m_info.nPose		 = 0;		// ���[�V�����|�[�Y�ԍ�
	m_info.nKeyCounter	 = 0;		// ���[�V�����L�[�J�E���^�[
	m_info.nWholeCounter = 0;		// ���[�V�����S�̃J�E���^�[
	m_info.bFinish		 = false;	// ���[�V�����I����

	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// ���[�V�������Đ���Ԃɂ���
	m_info.bFinish = false;

	// �p�[�c�̈ʒu�̏�����
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �����ʒu�Ə���������ݒ�
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].rot);
	}
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// �����̃��[�V��������ݒ�
	m_info.aMotionInfo[m_info.nNumMotion] = info;

	// ���[�V�����̏�񐔂����Z
	m_info.nNumMotion++;

	// ��O����
	assert(m_info.nNumMotion <= motion::MAX_MOTION);	// ���[�V�������I�[�o�[
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	m_bUpdate = bUpdate;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CMotion::SetModel(CMultiModel **ppModel, const int nNum)
{
	// �����̃��f������ݒ�
	m_ppModel = ppModel;

	// �����̃��f���̃p�[�c����ݒ�
	m_nNumModel = nNum;

	// ��O����
	assert(m_nNumModel <= motion::MAX_PARTS);	// �p�[�c���I�[�o�[
}

//============================================================
//	��ގ擾����
//============================================================
int CMotion::GetType(void) const
{
	// ���݂̃��[�V�����̎�ނ�Ԃ�
	return m_info.nType;
}

//============================================================
//	�|�[�Y�ԍ��擾����
//============================================================
int CMotion::GetPose(void) const
{
	// ���݂̃|�[�Y�ԍ���Ԃ�
	return m_info.nPose;
}

//============================================================
//	���[�V�����L�[�J�E���^�[�擾����
//============================================================
int CMotion::GetKeyCounter(void) const
{
	// ���݂̃��[�V�����L�[�J�E���^�[��Ԃ�
	return m_info.nKeyCounter;
}

//============================================================
//	���[�V�����S�̃J�E���^�[�擾����
//============================================================
int CMotion::GetWholeCounter(void) const
{
	// ���݂̃��[�V�����S�̃J�E���^�[��Ԃ�
	return m_info.nWholeCounter;
}

//============================================================
//	�I���擾����
//============================================================
bool CMotion::IsFinish(void) const
{
	// ���݂̃��[�V�����̏I���󋵂�Ԃ�
	return m_info.bFinish;
}

//============================================================
//	���[�v�擾����
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// �������[�V�����̃��[�v��ON/OFF�󋵂�Ԃ�
	return m_info.aMotionInfo[nType].bLoop;
}

//============================================================
//	����\���̎擾����
//============================================================
bool CMotion::IsWeaponDisp(const int nType) const
{
	// �������[�V�����̕���\����ON/OFF�󋵂�Ԃ�
	return m_info.aMotionInfo[nType].bWeaponDisp;
}

//============================================================
//	���̍U������t���O�̎擾����
//============================================================
bool CMotion::IsLeftWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// ���݂̃��[�V�������
	if (pMotionInfo->collLeft.nMin == NONE_IDX) { return false; }	// �J�n�J�E���g���ݒ�
	if (pMotionInfo->collLeft.nMax == NONE_IDX) { return false; }	// �I���J�E���g���ݒ�

	if (m_info.nWholeCounter >= pMotionInfo->collLeft.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collLeft.nMax)
	{ // �J�E���^�[���J�n�ƏI���͈͓̔��̏ꍇ

		return true;
	}

	return false;
}

//============================================================
//	�E�̍U������t���O�̎擾����
//============================================================
bool CMotion::IsRightWeaponCollision(void)
{
	SMotionInfo *pMotionInfo = &m_info.aMotionInfo[m_info.nType];	// ���݂̃��[�V�������
	if (pMotionInfo->collRight.nMin == NONE_IDX) { return false; }	// �J�n�J�E���g���ݒ�
	if (pMotionInfo->collRight.nMax == NONE_IDX) { return false; }	// �I���J�E���g���ݒ�

	if (m_info.nWholeCounter >= pMotionInfo->collRight.nMin
	&&  m_info.nWholeCounter <= pMotionInfo->collRight.nMax)
	{ // �J�E���^�[���J�n�ƏI���͈͓̔��̏ꍇ

		return true;
	}

	return false;
}

//============================================================
//	��������
//============================================================
CMotion *CMotion::Create(void)
{
	// ���[�V�����̐���
	CMotion *pMotion = new CMotion;
	if (pMotion == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���[�V�����̏�����
		if (FAILED(pMotion->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���[�V�����̔j��
			SAFE_DELETE(pMotion);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMotion;
	}
}

//============================================================
//	�j������
//============================================================
void CMotion::Release(CMotion *&prMotion)
{
	// ���[�V�����̏I��
	assert(prMotion != nullptr);
	prMotion->Uninit();

	// �������J��
	SAFE_DELETE(prMotion);
}
