//============================================================
//
//	�~���菈�� [collSphere.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collSphere.h"
#include "debugCollSphere.h"
#include "object.h"

//************************************************************
//	�e�N���X [CCollSphere] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CCollSphere::CCollSphere()
{
	// �����o�ϐ����N���A
	m_coll.clear();	// �v�f���N���A
}

//============================================================
//	�f�X�g���N�^
//============================================================
CCollSphere::~CCollSphere()
{

}

//============================================================
//	����������
//============================================================
HRESULT CCollSphere::Init(void)
{
	// �����o�ϐ���������
	m_coll.clear();	// �v�f���N���A

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CCollSphere::Uninit(void)
{
#if _DEBUG

	for (auto info : m_coll)
	{ // �v�f�����J��Ԃ�

		// ����\���̏I��
		SAFE_UNINIT(info.pVisual);
	}

#endif	// _DEBUG

	// �v�f���N���A
	m_coll.clear();
}

//============================================================
//	�X�V����
//============================================================
void CCollSphere::Update(void)
{
#if _DEBUG

	for (auto info : m_coll)
	{ // �S�v�f���J��Ԃ�

		// �I�t�Z�b�g�}�g���b�N�X�����߂�
		D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();	// �e�}�g���b�N�X
		D3DXMATRIX mtxOffset;	// �I�t�Z�b�g�v�Z�}�g���b�N�X
		D3DXMatrixTranslation(&mtxOffset, info.offset.x, info.offset.y, info.offset.z);
		D3DXMatrixMultiply(&mtxOffset, &mtxOffset, &mtxParent);

		// ���������ڂɔ��f
		D3DXVECTOR3 posColl = useful::GetMtxWorldPosition(mtxOffset);
		info.pVisual->SetVec3Position(posColl);	// �ʒu
		info.pVisual->SetRadius(info.fRadius);	// ���a
	}

#endif	// _DEBUG
}

//============================================================
//	������̐ݒ菈��
//============================================================
void CCollSphere::SetInfo(const SInfo& rInfo, const int nID)
{
	auto info = m_coll.begin();	// �z��̐擪�C�e���[�^�[
	info += nID;	// �C�e���[�^�[���C���f�b�N�X��������

	// �C�e���[�^�[�̒��g��ݒ�
	*info = rInfo;
}

//============================================================
//	������̎擾����
//============================================================
CCollSphere::SInfo CCollSphere::GetInfo(const int nID) const
{
	auto info = m_coll.begin();	// �z��̐擪�C�e���[�^�[
	info += nID;	// �C�e���[�^�[���C���f�b�N�X��������

	// �C�e���[�^�[�̒��g��Ԃ�
	return *info;
}

//============================================================
//	�z��̎擾����
//============================================================
std::vector<CCollSphere::SInfo> CCollSphere::GetVector(void) const
{
	// �z���Ԃ�
	return m_coll;
}

//============================================================
//	����̒ǉ�����
//============================================================
void CCollSphere::AddColl
(
	const D3DXVECTOR3& rOffset,	// ����ʒu�I�t�Z�b�g
	const float fRadius			// ���蔼�a
)
{
	// ����ݒ�
	SInfo tempInfo;	// �ݒ�p���
	tempInfo.offset  = rOffset;	// �I�t�Z�b�g
	tempInfo.fRadius = fRadius;	// ���a

#if _DEBUG

	// �I�t�Z�b�g�}�g���b�N�X�����߂�
	D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();	// �e�}�g���b�N�X
	D3DXMATRIX mtxOffset;	// �I�t�Z�b�g�v�Z�}�g���b�N�X
	D3DXMatrixTranslation(&mtxOffset, rOffset.x, rOffset.y, rOffset.z);
	D3DXMatrixMultiply(&mtxOffset, &mtxOffset, &mtxParent);

	// �I�t�Z�b�g���[���h���W�������ڂɐݒ�
	D3DXVECTOR3 posColl = useful::GetMtxWorldPosition(mtxOffset);
	tempInfo.pVisual = CDebugCollSphere::Create(posColl, fRadius);

#endif	// _DEBUG

	// �z��Ō���ɒǉ�����ݒ�
	m_coll.push_back(tempInfo);
}

//============================================================
//	����̍폜����
//============================================================
void CCollSphere::SubColl(const int nID)
{
	auto info = m_coll.begin();	// �z��̐擪�C�e���[�^�[
	info += nID;	// �C�e���[�^�[���C���f�b�N�X��������

#if _DEBUG

	// �f�o�b�O�p�̌����ڂ��I��
	SAFE_UNINIT(info->pVisual);

#endif	// _DEBUG

	// �z��̗v�f����폜
	m_coll.erase(info);
}

//============================================================
//	��������
//============================================================
CCollSphere *CCollSphere::Create(CObject *pParent)
{
	// �~����̐���
	CCollSphere *pCollSphere = new CCollSphere;
	if (pCollSphere == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �~����̏�����
		if (FAILED(pCollSphere->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �~����̔j��
			SAFE_DELETE(pCollSphere);
			return nullptr;
		}

		// �e�I�u�W�F�N�g��ݒ�
		pCollSphere->m_pParent = pParent;

		// �m�ۂ����A�h���X��Ԃ�
		return pCollSphere;
	}
}

//============================================================
//	�j������
//============================================================
void CCollSphere::Release(CCollSphere *&prCollSphere)
{
	// �~����̏I��
	assert(prCollSphere != nullptr);
	prCollSphere->Uninit();

	// �������J��
	SAFE_DELETE(prCollSphere);
}
