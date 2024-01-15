//============================================================
//
//	�I�u�W�F�N�g�����L�����N�^�[���� [objectDivChara.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectDivChara.h"
#include "manager.h"
#include "renderer.h"
#include "objectChara.h"
#include "multiModel.h"

//************************************************************
//	�q�N���X [CObjectDivChara] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectDivChara::CObjectDivChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority), m_nUpperParentID(0)
{
	// �����o�ϐ����N���A
	memset(&m_apBody[0], 0, sizeof(m_apBody));	// �g�̂̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectDivChara::~CObjectDivChara()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectDivChara::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apBody[0], 0, sizeof(m_apBody));	// �g�̂̏��
	m_nUpperParentID = 0;	// �㔼�g�e�C���f�b�N�X

	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		// �I�u�W�F�N�g�L�����N�^�[�̐���
		m_apBody[i] = CObjectChara::Create(VEC3_ZERO);
		if (m_apBody[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_apBody[i]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectDivChara::Uninit(void)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		// �I�u�W�F�N�g�L�����N�^�[�̏I��
		m_apBody[i]->Uninit();
	}

	// �I�u�W�F�N�g�����L�����N�^�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectDivChara::Update(void)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		// �I�u�W�F�N�g�L�����N�^�[�̍X�V
		m_apBody[i]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectDivChara::Draw(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetVec3Position();	// �����g�̈ʒu
	D3DXVECTOR3 rot = GetVec3Rotation();	// �����g�̌���
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

	// �ϐ��z���錾
	D3DXMATRIX *apMtxWorld[] = { &mtxWorld, GetMultiModel(BODY_LOWER, m_nUpperParentID)->GetPtrMtxWorld() };
	static_assert(NUM_ARRAY(apMtxWorld) == BODY_MAX, "ERROR : Body Count Mismatch");

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �����g�̃��[���h�}�g���b�N�X�̔��f
	m_apBody[BODY_LOWER]->SetMtxWorld(mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCntChara = 0; nCntChara < BODY_MAX; nCntChara++)
	{ // ���������g�̂̐����J��Ԃ�

		for (int nCntParts = 0; nCntParts < m_apBody[nCntChara]->GetNumModel(); nCntParts++)
		{ // �p�[�c�̑������J��Ԃ�

			// �p�[�c�̕`��
			GetMultiModel((EBody)nCntChara, nCntParts)->Draw();
		}
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectDivChara::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// �����̈ʒu��ݒ�
	m_apBody[BODY_LOWER]->SetVec3Position(rPos);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectDivChara::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_apBody[BODY_LOWER]->GetVec3Position();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectDivChara::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// �����̌�����ݒ�
	m_apBody[BODY_LOWER]->SetVec3Rotation(rRot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectDivChara::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_apBody[BODY_LOWER]->GetVec3Rotation();
}

//============================================================
//	�}�e���A���S�ݒ菈��
//============================================================
void CObjectDivChara::SetAllMaterial(const D3DXMATERIAL &rMat)
{
	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		// �����̃}�e���A����S�}�e���A���ɐݒ�
		m_apBody[i]->SetAllMaterial(rMat);
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CObjectDivChara::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g

	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		m_apBody[i]->SetEnableUpdate(bUpdate);	// ��E�����g
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CObjectDivChara::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g

	for (int i = 0; i < BODY_MAX; i++)
	{ // ���������g�̂̐����J��Ԃ�

		m_apBody[i]->SetEnableDraw(bDraw);	// ��E�����g
	}
}

//============================================================
//	��������
//============================================================
CObjectDivChara *CObjectDivChara::Create
(
	const D3DXVECTOR3 &rPos,	// �ʒu
	const D3DXVECTOR3 &rRot,	// ����
	const int nUpperParentID	// �㔼�g�e�C���f�b�N�X
)
{
	// �I�u�W�F�N�g�����L�����N�^�[�̐���
	CObjectDivChara *pObjectDivChara = new CObjectDivChara;
	if (pObjectDivChara == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�����L�����N�^�[�̏�����
		if (FAILED(pObjectDivChara->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�����L�����N�^�[�̔j��
			SAFE_DELETE(pObjectDivChara);
			return nullptr;
		}

		// �㔼�g�̐e�C���f�b�N�X��ݒ�
		pObjectDivChara->SetUpperParentID(nUpperParentID);

		// �ʒu��ݒ�
		pObjectDivChara->SetVec3Position(rPos);

		// ������ݒ�
		pObjectDivChara->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectDivChara;
	}
}

//============================================================
//	�㔼�g�̐e�C���f�b�N�X�̐ݒ菈��
//============================================================
void CObjectDivChara::SetUpperParentID(const int nUpperParentID)
{
	if (nUpperParentID > NONE_IDX && nUpperParentID < m_apBody[BODY_LOWER]->GetNumModel())
	{ // ���K�C���f�b�N�X�̏ꍇ

		// �㔼�g�̐e�C���f�b�N�X��ݒ�
		m_nUpperParentID = nUpperParentID;
	}
	else { assert(false); }	// �C���f�b�N�X�G���[
}

//============================================================
//	�I�u�W�F�N�g�L�����N�^�[�擾����
//============================================================
CObjectChara *CObjectDivChara::GetObjectChara(const EBody bodyID) const
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{ // ���K�C���f�b�N�X�̏ꍇ

		// �����C���f�b�N�X�̔��g��Ԃ�
		return m_apBody[bodyID];
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return nullptr;
}

//============================================================
//	�}���`���f���擾����
//============================================================
CMultiModel *CObjectDivChara::GetMultiModel(const EBody bodyID, const int nModelID) const
{
	if (bodyID > NONE_IDX && bodyID < BODY_MAX)
	{
		if (nModelID > NONE_IDX && nModelID < m_apBody[bodyID]->GetNumModel())
		{ // ���K�C���f�b�N�X�̏ꍇ

			// �����C���f�b�N�X�̃p�[�c��Ԃ�
			return m_apBody[bodyID]->GetMultiModel(nModelID);
		}
	}

	// �C���f�b�N�X�G���[
	assert(false);
	return nullptr;
}

//============================================================
//	�j������
//============================================================
void CObjectDivChara::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
