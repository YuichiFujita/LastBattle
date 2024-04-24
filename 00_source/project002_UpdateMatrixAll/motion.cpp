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
#include "objectChara.h"

//************************************************************
//	�e�N���X [CMotion] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMotion::CMotion() :
	m_ppModel	(nullptr),	// ���f�����
	m_pChara	(nullptr),	// �I�u�W�F�N�g�L�����N�^�[���
	m_nNumModel	(0),		// ���f���̃p�[�c��
	m_bUpdate	(true)		// �X�V��
{
	// �����o�ϐ����N���A
	memset(&m_info,  0, sizeof(m_info));	// ���[�V�������
	memset(&m_blend, 0, sizeof(m_blend)); 	// �u�����h���
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
	memset(&m_info,  0, sizeof(m_info));	// ���[�V�������
	memset(&m_blend, 0, sizeof(m_blend)); 	// �u�����h���
	m_ppModel	= nullptr;	// ���f�����
	m_pChara	= nullptr;	// �I�u�W�F�N�g�L�����N�^�[���
	m_nNumModel	= 0;		// ���f���̃p�[�c��
	m_bUpdate	= true;		// �X�V��

	// ���[�V�������I����Ԃɂ���
	m_info.bFinish = true;

	for (int nCntMotion = 0; nCntMotion < motion::MAX_MOTION; nCntMotion++)
	{ // ���[�V�����̍ő吔���J��Ԃ�

		// �L�����Z���E�R���{�t���[�����Ȃ��ɂ���
		m_info.aMotionInfo[nCntMotion].nCancelFrame = NONE_IDX;
		m_info.aMotionInfo[nCntMotion].nComboFrame  = NONE_IDX;

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
	if (!m_bUpdate) { return; }	// �X�V���Ȃ�
	if (m_info.aMotionInfo[m_info.nType].nNumKey <= 0) { return; }	// �L�[�����ݒ�

	if (m_blend.nFrame > 0)
	{ // �u�����h�t���[�����ݒ肳��Ă���ꍇ

		// �u�����h�̍X�V
		UpdateBlend();
	}
	else
	{ // �u�����h�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// ���[�V�����̍X�V
		UpdateMotion();
	}

	// �ړ��̍X�V
	UpdateMove();
}

//============================================================
//	�ړ��̍X�V����
//============================================================
void CMotion::UpdateMove(void)
{
	if (m_pChara == nullptr) { return; }	// �I�u�W�F�N�g�L�����N�^�[���ݒ�

	// �ϐ���錾
	D3DXMATRIX  mtxChara	= m_pChara->GetMtxWorld();				// �L�����}�g���b�N�X
	D3DXVECTOR3 posSetChara	= m_pChara->GetVec3Position();			// �L�����ݒ�ʒu
	D3DXVECTOR3 posOldChara	= useful::GetMatrixPosition(mtxChara);	// �L�����ߋ��ʒu
	D3DXVECTOR3 posCurChara = VEC3_ZERO;							// �L�������݈ʒu

	// �ړ��ʂ����߂�
	float fRate = 1.0f / (float)m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].nFrame;	// �L�[�t���[������
	D3DXVECTOR3 moveRate = m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].move * fRate;	// �t���[���ړ���

	if (m_info.aMotionInfo[m_info.nType].aKeyInfo[m_info.nKey].nFrame > 0)
	{ // �t���[�����ݒ肳��Ă���ꍇ

		// �ړ��ʂ��}�g���b�N�X�ɔ��f
		D3DXMATRIX mtxMove;	// �}�g���b�N�X�v�Z�p
		D3DXMatrixTranslation(&mtxMove, moveRate.x, moveRate.y, moveRate.z);
		D3DXMatrixMultiply(&mtxChara, &mtxMove, &mtxChara);

		// �ړ��ʂ�^�����}�g���b�N�X�̃��[���h���W�����߂�
		posCurChara = useful::GetMatrixPosition(mtxChara);

		// �ߋ��ƌ��݂̈ʒu����ړ��ʂ����߁A�ʒu�ɗ^����
		posSetChara += posOldChara - posCurChara;

		// �ʒu�𔽉f
		m_pChara->SetVec3Position(posSetChara);
	}
}

//============================================================
//	���[�V�����̍X�V����
//============================================================
void CMotion::UpdateMotion(void)
{
	int nType = m_info.nType;	// ���[�V�������
	int nKey  = m_info.nKey;	// ���[�V�����L�[�ԍ�

	// ���̃��[�V�����L�[�ԍ������߂�
	int nNextKey = (nKey + 1) % m_info.aMotionInfo[nType].nNumKey;

	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		D3DXVECTOR3 diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextKey].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextKey].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// ���������̐��K��

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_info.nKeyCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nKey].nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nKey].aKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_info.nKeyCounter < m_info.aMotionInfo[nType].aKeyInfo[nKey].nFrame)
	{ // ���݂̃L�[�̍Đ����I�����Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_info.nKeyCounter++;
		m_info.nWholeCounter++;
	}
	else
	{ // ���݂̃L�[�̍Đ����I�������ꍇ

		// ���̃L�[�Ɉڍs
		if (m_info.aMotionInfo[nType].bLoop)
		{ // ���[�V���������[�v����ꍇ

			// �L�[�J�E���^�[��������
			m_info.nKeyCounter = 0;

			// �L�[�J�E���g�����Z
			m_info.nKey = (m_info.nKey + 1) % m_info.aMotionInfo[nType].nNumKey;	// �ő�l��0�ɖ߂�

			if (m_info.nKey == 0)
			{ // �L�[���ŏ��ɖ߂����ꍇ

				// �S�̃J�E���^�[��������
				m_info.nWholeCounter = 0;
			}
		}
		else
		{ // ���[�V���������[�v���Ȃ��ꍇ

			if (m_info.nKey < m_info.aMotionInfo[nType].nNumKey - 2)
			{ // ���݂̃L�[���ŏI�̃L�[�ł͂Ȃ��ꍇ

				// �L�[�J�E���^�[��������
				m_info.nKeyCounter = 0;

				// �L�[�J�E���g�����Z
				m_info.nKey++;
			}
			else
			{ // ���݂̃L�[���ŏI�̃L�[�̏ꍇ

				// ���[�V�������I����Ԃɂ���
				m_info.bFinish = true;
			}
		}
	}
}

//============================================================
//	�u�����h�X�V����
//============================================================
void CMotion::UpdateBlend(void)
{
	// �p�[�c�̈ʒu�̍X�V
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �ʒu�E�����̍��������߂�
		D3DXVECTOR3 diffPos = m_info.aMotionInfo[m_info.nType].aKeyInfo[0].aKey[nCntKey].pos - m_blend.aKey[nCntKey].pos;
		D3DXVECTOR3 diffRot = m_info.aMotionInfo[m_info.nType].aKeyInfo[0].aKey[nCntKey].rot - m_blend.aKey[nCntKey].rot;
		useful::NormalizeRot(diffRot);	// ���������̐��K��

		// ���݂̃p�[�c�̈ʒu�E�������X�V
		float fRate = (float)m_blend.nWholeCounter / (float)m_blend.nFrame;	// �L�[�t���[������
		m_ppModel[nCntKey]->SetVec3Position(m_blend.aKey[nCntKey].pos + diffPos * fRate);
		m_ppModel[nCntKey]->SetVec3Rotation(m_blend.aKey[nCntKey].rot + diffRot * fRate);
	}

	// ���[�V�����̑J�ڂ̍X�V
	if (m_blend.nWholeCounter < m_blend.nFrame)
	{ // �u�����h�̍Đ����I�����Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_blend.nWholeCounter++;
	}
	else
	{ // �u�����h�̍Đ����I�������ꍇ

		// �u�����h�Đ��t���[������������
		m_blend.nFrame = 0;

		// �u�����h�S�̃J�E���^�[��������
		m_blend.nWholeCounter = 0;
	}
}

//============================================================
//	�ݒ菈��
//============================================================
void CMotion::Set(const int nType, const int nBlendFrame)
{
	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// �����̃u�����h�t���[����ݒ�
	m_blend.nFrame = nBlendFrame;

	// ���[�V��������������
	m_info.nKey			 = 0;		// ���[�V�����L�[�ԍ�
	m_info.nKeyCounter	 = 0;		// ���[�V�����L�[�J�E���^�[
	m_info.nWholeCounter = 0;		// ���[�V�����S�̃J�E���^�[
	m_info.bFinish		 = false;	// ���[�V�����I����

	// �u�����h����������
	m_blend.nWholeCounter = 0;		// �u�����h�S�̃J�E���^�[

	// �p�[�c�̈ʒu�̏�����
	if (m_blend.nFrame > 0)
	{ // �u�����h�t���[�����ݒ肳��Ă���ꍇ

		for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
		{ // ���f���̃p�[�c�����J��Ԃ�

			// ���݈ʒu�ƌ��݌�����ۑ�
			m_blend.aKey[nCntKey].pos = m_ppModel[nCntKey]->GetVec3Position();
			m_blend.aKey[nCntKey].rot = m_ppModel[nCntKey]->GetVec3Rotation();
		}
	}
	else
	{ // �u�����h�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
		{ // ���f���̃p�[�c�����J��Ԃ�

			// �����ʒu�Ə���������ݒ�
			m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nKey].aKey[nCntKey].pos);
			m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nKey].aKey[nCntKey].rot);
		}
	}
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// �����̃��[�V��������ݒ�
	m_info.aMotionInfo[m_info.nNumType] = info;

	// ���[�V�����S�̃t���[������ݒ�
	int nSubKey = (m_info.aMotionInfo[m_info.nNumType].bLoop) ? 0 : 1;	// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	int nLoop = m_info.aMotionInfo[m_info.nNumType].nNumKey - nSubKey;	// �J��Ԃ��������߂�
	for (int nCntKey = 0; nCntKey < nLoop; nCntKey++)
	{ // �L�[�̑������J��Ԃ�

		// �L�[�̃t���[���������Z
		m_info.aMotionInfo[m_info.nNumType].nWholeFrame += m_info.aMotionInfo[m_info.nNumType].aKeyInfo[nCntKey].nFrame;
	}

	// ���[�V�����̏�񐔂����Z
	m_info.nNumType++;

	// ��O����
	assert(m_info.nNumType <= motion::MAX_MOTION);	// ���[�V�������I�[�o�[
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
//	���_�ʒu�̐ݒ菈��
//============================================================
void CMotion::SetOriginPosition(const D3DXVECTOR3& rPos, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �p�[�c�����I�[�o�[���Ă��Ȃ��ꍇ

		// ���_�ʒu��ݒ�
		m_info.aOriginKey[nParts].pos = rPos;
	}
	else { assert(false); }
}

//============================================================
//	���_�����̐ݒ菈��
//============================================================
void CMotion::SetOriginRotation(const D3DXVECTOR3& rRot, const int nParts)
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �p�[�c�����I�[�o�[���Ă��Ȃ��ꍇ

		// ���_������ݒ�
		m_info.aOriginKey[nParts].rot = rRot;
	}
	else { assert(false); }
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
//	��ނ̑����擾����
//============================================================
int CMotion::GetNumType(void) const
{
	// ���[�V�����̎�ނ̑�����Ԃ�
	return m_info.nNumType;
}

//============================================================
//	�L�[�ԍ��擾����
//============================================================
int CMotion::GetKey(void) const
{
	// ���݂̃L�[�ԍ���Ԃ�
	return m_info.nKey;
}

//============================================================
//	�L�[�̑����擾����
//============================================================
int CMotion::GetNumKey(const int nType) const
{
	// �������[�V�����̃L�[�̑�����Ԃ�
	int nSubKey = (m_info.aMotionInfo[m_info.nNumType].bLoop) ? 0 : 1;	// ���[�v���Ȃ��ꍇ�Ō�̃L�[�͊܂܂Ȃ�
	return m_info.aMotionInfo[nType].nNumKey - nSubKey;
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
//	���[�V�����S�̃t���[�����擾����
//============================================================
int CMotion::GetWholeFrame(const int nType) const
{
	// �������[�V�����̑S�̃t���[������Ԃ�
	return m_info.aMotionInfo[nType].nWholeFrame;
}

//============================================================
//	���[�V�����L�����Z���t���[���擾����
//============================================================
int CMotion::GetCancelFrame(const int nType) const
{
	// �������[�V�����̃L�����Z���t���[����Ԃ�
	return m_info.aMotionInfo[nType].nCancelFrame;
}

//============================================================
//	���[�V�����R���{�t���[���擾����
//============================================================
int CMotion::GetComboFrame(const int nType) const
{
	// �������[�V�����̃R���{�t���[����Ԃ�
	return m_info.aMotionInfo[nType].nComboFrame;
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
//	�L�����Z���擾����
//============================================================
bool CMotion::IsCancel(const int nType) const
{
	if (m_info.aMotionInfo[nType].nCancelFrame != NONE_IDX)
	{ // �L�����Z���t���[�����ݒ肳��Ă���ꍇ

		// �������[�V�����̃L�����Z���󋵂�Ԃ�
		return (m_info.nWholeCounter >= m_info.aMotionInfo[nType].nCancelFrame);
	}
	else
	{ // �L�����Z���t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// �L�����Z���s��Ԃ�
		return false;
	}
}

//============================================================
//	�R���{�擾����
//============================================================
bool CMotion::IsCombo(const int nType) const
{
	if (m_info.aMotionInfo[nType].nComboFrame != NONE_IDX)
	{ // �R���{�t���[�����ݒ肳��Ă���ꍇ

		// �������[�V�����̃R���{�󋵂�Ԃ�
		return (m_info.nWholeCounter >= m_info.aMotionInfo[nType].nComboFrame);
	}
	else
	{ // �R���{�t���[�����ݒ肳��Ă��Ȃ��ꍇ

		// �R���{�s��Ԃ�
		return false;
	}
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
//	���_�ʒu�̎擾����
//============================================================
D3DXVECTOR3 CMotion::GetOriginPosition(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �p�[�c�����I�[�o�[���Ă��Ȃ��ꍇ

		// ���_�ʒu��Ԃ�
		return m_info.aOriginKey[nParts].pos;
	}
	else { assert(false); return VEC3_ZERO; }
}

//============================================================
//	���_�����̎擾����
//============================================================
D3DXVECTOR3 CMotion::GetOriginRotation(const int nParts) const
{
	if (nParts > NONE_IDX && nParts < motion::MAX_PARTS)
	{ // �p�[�c�����I�[�o�[���Ă��Ȃ��ꍇ

		// ���_������Ԃ�
		return m_info.aOriginKey[nParts].rot;
	}
	else { assert(false); return VEC3_ZERO; }
}

//============================================================
//	��������
//============================================================
CMotion *CMotion::Create(CObjectChara *pChara)
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

		// �I�u�W�F�N�g�L�����N�^�[��ݒ�
		pMotion->m_pChara = pChara;

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
