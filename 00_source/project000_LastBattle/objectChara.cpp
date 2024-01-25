//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	�q�N���X [CObjectChara] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pMotion	(nullptr),		// ���[�V�����̏��
	m_pos		(VEC3_ZERO),	// �ʒu
	m_rot		(VEC3_ZERO),	// ����
	m_nNumModel	(0)				// �p�[�c�̑���
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectChara::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
	m_pMotion	= nullptr;		// ���[�V�����̏��
	m_pos		= VEC3_ZERO;	// �ʒu
	m_rot		= VEC3_ZERO;	// ����
	m_nNumModel	= 0;			// �p�[�c�̑���

	// ���[�V�����I�u�W�F�N�g�̐���
	m_pMotion = CMotion::Create();
	if (m_pMotion == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectChara::Uninit(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < motion::MAX_PARTS; nCntObjectChara++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �}���`���f���̏I��
		SAFE_UNINIT(m_apMultiModel[nCntObjectChara]);
	}

	// ���[�V�����̔j��
	SAFE_REF_RELEASE(m_pMotion);

	// �I�u�W�F�N�g�L�����N�^�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectChara::Update(void)
{
	if (m_pMotion != nullptr)
	{ // ���[�V���������݂���ꍇ

		// ���[�V�����̍X�V
		m_pMotion->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectChara::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntObjectChara]->Draw();
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�}�e���A���S�ݒ菈��
//============================================================
void CObjectChara::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �����̃}�e���A����S�}�e���A���ɐݒ�
		m_apMultiModel[nCntObjectChara]->SetAllMaterial(rMat);
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CObjectChara::ResetMaterial(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �S�}�e���A���ɏ����}�e���A�����Đݒ�
		m_apMultiModel[nCntObjectChara]->ResetMaterial();
	}
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObjectChara::GetMtxWorld(void) const
{
	// �}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectChara::GetPtrMtxWorld(void)
{
	// �}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	��������
//============================================================
CObjectChara *CObjectChara::Create(const D3DXVECTOR3 &rPos, const D3DXVECTOR3 &rRot)
{
	// �I�u�W�F�N�g�L�����N�^�[�̐���
	CObjectChara *pObjectChara = new CObjectChara;
	if (pObjectChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̏�����
		if (FAILED(pObjectChara->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�L�����N�^�[�̔j��
			SAFE_DELETE(pObjectChara);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObjectChara->SetVec3Position(rPos);

		// ������ݒ�
		pObjectChara->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectChara;
	}
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CObjectChara::SetMotion(const int nType)
{
	// ���[�V�����̐ݒ�
	m_pMotion->Set(nType);
}

//============================================================
//	�p�[�c���̐ݒ菈��
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// �p�[�c�C���f�b�N�X
	const int nParentID,		// �e�C���f�b�N�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const char *pFileName		// �t�@�C����
)
{
	if (nID < motion::MAX_PARTS && nParentID < motion::MAX_PARTS && pFileName != nullptr)
	{ // �p�[�c�E�e�C���f�b�N�X���z��͈͓����A�t�@�C���������݂���ꍇ

		// ���f���̐���
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// ���f��������
		m_apMultiModel[nID]->BindModel(pFileName);

		// �e���f���̐ݒ�
		if (nParentID == NONE_IDX)
		{ // �e���Ȃ��ꍇ

			// nullptr��ݒ�
			m_apMultiModel[nID]->SetParentModel(nullptr);
		}
		else
		{ // �e������ꍇ

			// �e�̃A�h���X��ݒ�
			m_apMultiModel[nID]->SetParentModel(m_apMultiModel[nParentID]);
		}

		// �p�[�c�̑��������Z
		m_nNumModel++;
	}
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectChara::SetModelInfo(void)
{
	// ���f�����̐ݒ�
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CObjectChara::SetMotionInfo(CMotion::SMotionInfo info)
{
	// ���[�V�������̐ݒ�
	m_pMotion->SetInfo(info);
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectChara::SetMtxWorld(const D3DXMATRIX &rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	���[�V�����̍X�V�󋵂̐ݒ菈��
//============================================================
void CObjectChara::SetEnableMotionUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂����[�V�����̍X�V�󋵂ɐݒ�
	m_pMotion->SetEnableUpdate(bUpdate);
}

//============================================================
//	�p�[�c�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetPartsPosition(const int nPartsID, const D3DXVECTOR3& rPos)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̈ʒu��ݒ�
		m_apMultiModel[nPartsID]->SetVec3Position(rPos);
	}
	else { assert(false); }
}

//============================================================
//	�p�[�c�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsPosition(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̃p�[�c�̈ʒu��Ԃ�
		return m_apMultiModel[nPartsID]->GetVec3Position();
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	�p�[�c�����̐ݒ菈��
//============================================================
void CObjectChara::SetPartsRotation(const int nPartsID, const D3DXVECTOR3& rRot)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̌�����ݒ�
		m_apMultiModel[nPartsID]->SetVec3Rotation(rRot);
	}
	else { assert(false); }
}

//============================================================
//	�p�[�c�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsRotation(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̃p�[�c�̌�����Ԃ�
		return m_apMultiModel[nPartsID]->GetVec3Rotation();
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	�}���`���f���擾����
//============================================================
CMultiModel *CObjectChara::GetMultiModel(const int nPartsID) const
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �}���`���f���̏���Ԃ�
		return m_apMultiModel[nPartsID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat, const int nPartsID, const int nMatID)
{
	if (nPartsID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �����̃}�e���A����ݒ�
		m_apMultiModel[nPartsID]->SetMaterial(rMat, nMatID);
	}
	else { assert(false); }
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �����̓����x��ݒ�
		m_apMultiModel[nCntParts]->SetAlpha(fAlpha);
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CObjectChara::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		float fCurAlpha = m_apMultiModel[nCntParts]->GetAlpha();	// ���݂̃��f���̓����x
		if (fCurAlpha > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = fCurAlpha;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CObjectChara::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (m_apMultiModel[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_apMultiModel[nCntParts]->GetMaxAlpha();
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�p�[�c�����擾����
//============================================================
int CObjectChara::GetNumModel(void) const
{
	// �p�[�c�̑�����Ԃ�
	return m_nNumModel;
}

//============================================================
//	���[�V������ގ擾����
//============================================================
int CObjectChara::GetMotionType(void) const
{
	// ���[�V�����̎�ނ�Ԃ�
	return m_pMotion->GetType();
}

//============================================================
//	���[�V������ނ̑����擾����
//============================================================
int CObjectChara::GetMotionNumType(void) const
{
	// ���[�V�����̎�ނ̑�����Ԃ�
	return m_pMotion->GetNumType();
}

//============================================================
//	���[�V�����L�[�ԍ��擾����
//============================================================
int CObjectChara::GetMotionKey(void) const
{
	// ���[�V�����̃L�[�ԍ���Ԃ�
	return m_pMotion->GetKey();
}

//============================================================
//	���[�V�����L�[�̑����擾����
//============================================================
int CObjectChara::GetMotionNumKey(void) const
{
	// ���݃��[�V�����̃L�[�̑�����Ԃ�
	return m_pMotion->GetNumKey(m_pMotion->GetType());
}

//============================================================
//	���[�V�����L�[�J�E���^�[�擾����
//============================================================
int CObjectChara::GetMotionKeyCounter(void) const
{
	// ���[�V�����̃L�[�J�E���^�[��Ԃ�
	return m_pMotion->GetKeyCounter();
}

//============================================================
//	���[�V�����S�̃J�E���^�[�擾����
//============================================================
int CObjectChara::GetMotionWholeCounter(void) const
{
	// ���[�V�����̑S�̃J�E���^�[��Ԃ�
	return m_pMotion->GetWholeCounter();
}

//============================================================
//	���[�V�����S�̃t���[�����擾����
//============================================================
int CObjectChara::GetMotionWholeFrame(void) const
{
	// ���݃��[�V�����̑S�̃t���[������Ԃ�
	return m_pMotion->GetWholeFrame(m_pMotion->GetType());
}

//============================================================
//	���[�V�����L�����Z���t���[���擾����
//============================================================
int CObjectChara::GetMotionCancelFrame(void) const
{
	// ���݃��[�V�����̃L�����Z���t���[����Ԃ�
	return m_pMotion->GetCancelFrame(m_pMotion->GetType());
}

//============================================================
//	���[�V�����I���擾����
//============================================================
bool CObjectChara::IsMotionFinish(void) const
{
	// ���[�V�����̏I���󋵂�Ԃ�
	return m_pMotion->IsFinish();
}

//============================================================
//	���[�V�������[�v�擾����
//============================================================
bool CObjectChara::IsMotionLoop(void) const
{
	// ���݃��[�V�����̃��[�v�󋵂�Ԃ�
	return m_pMotion->IsLoop(m_pMotion->GetType());
}

//============================================================
//	���[�V�����L�����Z���擾����
//============================================================
bool CObjectChara::IsMotionCancel(void) const
{
	// ���݃��[�V�����̃L�����Z���󋵂�Ԃ�
	return m_pMotion->IsCancel(m_pMotion->GetType());
}

//============================================================
//	���[�V��������\���̎擾����
//============================================================
bool CObjectChara::IsWeaponDisp(void) const
{
	// ���݃��[�V�����̕���\���󋵂�Ԃ�
	return m_pMotion->IsWeaponDisp(m_pMotion->GetType());
}

//============================================================
//	���̍U������t���O�̎擾����
//============================================================
bool CObjectChara::IsLeftWeaponCollision(void)
{
	// ���[�V�����̍��̍U������󋵂�Ԃ�
	return m_pMotion->IsLeftWeaponCollision();
}

//============================================================
//	�E�̍U������t���O�̎擾����
//============================================================
bool CObjectChara::IsRightWeaponCollision(void)
{
	// ���[�V�����̉E�̍U������󋵂�Ԃ�
	return m_pMotion->IsRightWeaponCollision();
}

//============================================================
//	�j������
//============================================================
void CObjectChara::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
