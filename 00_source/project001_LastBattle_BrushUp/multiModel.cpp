//============================================================
//
//	�}���`���f������ [multiModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "renderState.h"

//************************************************************
//	�q�N���X [CMultiModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMultiModel::CMultiModel(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObjectModel(label, dimension, nPriority), m_pParent(nullptr)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CMultiModel::~CMultiModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMultiModel::Init(void)
{
	// �����o�ϐ���������
	m_pParent = nullptr;	// �e���f���ւ̃|�C���^

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����`���OFF�ɂ���
	SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMultiModel::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CMultiModel::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CMultiModel::Draw(CShader *pShader)
{
	// �ϐ���錾
	CModel::SModel modelData = GetModelData();	// ���f���̏��
	D3DXVECTOR3 pos = GetVec3Position();		// ���f���̈ʒu
	D3DXVECTOR3 rot = GetVec3Rotation();		// ���f���̌���
	D3DXVECTOR3 scale = GetVec3Scaling();		// ���f���̊g�嗦
	D3DXMATRIX  mtxScale, mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	D3DXMATRIX   mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXMATRIX   mtxParent;	// �e�̃}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;			// �f�o�C�X�̃|�C���^
	CRenderState *pRenderState = GetRenderState();	// �����_�[�X�e�[�g�̏��

	// �����_�[�X�e�[�g��ݒ�
	pRenderState->Set();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �e�}�g���b�N�X��ݒ�
	if (m_pParent == nullptr)
	{ // �e�����݂��Ȃ��ꍇ

		// ���݂̃}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// �ݒ肳�ꂽ�ŐV�̃}�g���b�N�X (���̂̃}�g���b�N�X)
	}
	else
	{ // �e�����݂���ꍇ

		// �e�̃}�g���b�N�X��ݒ�
		mtxParent = *m_pParent->GetPtrMtxWorld();
	}

	// ���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̔��f
	SetMtxWorld(mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (pShader == nullptr)
	{ // �V�F�[�_�[���g�p����Ă��Ȃ��ꍇ

		// �ʏ�`��
		DrawNormal();
	}
	else
	{ // �V�F�[�_�[���g�p����Ă���ꍇ

		// �V�F�[�_�[�`��
		DrawShader(pShader);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �����_�[�X�e�[�g���Đݒ�
	pRenderState->Reset();
}

//============================================================
//	��������
//============================================================
CMultiModel *CMultiModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �}���`���f���̐���
	CMultiModel *pMultiModel = new CMultiModel;
	if (pMultiModel == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �}���`���f���̏�����
		if (FAILED(pMultiModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �}���`���f���̔j��
			SAFE_DELETE(pMultiModel);
			return nullptr;
		}

		// �ʒu��ݒ�
		pMultiModel->SetVec3Position(rPos);

		// ������ݒ�
		pMultiModel->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pMultiModel->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pMultiModel;
	}
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CMultiModel::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g�|�C���^��e�ɐݒ�
	m_pParent = pObject;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CMultiModel::SetParentModel(CMultiModel *pModel)
{
	// �����̃��f���|�C���^��e�ɐݒ�
	m_pParent = (CObject*)pModel;
}

//============================================================
//	�e�I�u�W�F�N�g�폜����
//============================================================
void CMultiModel::DeleteParentObject(void)
{
	// �e�I�u�W�F�N�g��nullptr�ɂ���
	m_pParent = nullptr;
}

//============================================================
//	�ʏ�`�揈��
//============================================================
void CMultiModel::DrawNormal(void)
{
	// �ϐ���錾
	CModel::SModel modelData = GetModelData();	// ���f���̏��
	D3DXVECTOR3	scale = GetVec3Scaling();		// ���f���̊g�嗦
	D3DXMATRIX	mtxWorld = GetMtxWorld();		// ���[���h�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	for (int nCntMat = 0; nCntMat < (int)modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&GetPtrMaterial(nCntMat)->MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(modelData.pTextureID[nCntMat]));

		if (scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		modelData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}
}

//============================================================
//	�V�F�[�_�[�`�揈��
//============================================================
void CMultiModel::DrawShader(CShader *pShader)
{
	// �ϐ���錾
	CModel::SModel modelData = GetModelData();	// ���f���̏��
	D3DXVECTOR3	scale = GetVec3Scaling();		// ���f���̊g�嗦
	D3DXMATRIX	mtxWorld = GetMtxWorld();		// ���[���h�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �`��J�n
	pShader->Begin();
	pShader->BeginPass(0);

	// �}�g���b�N�X����ݒ�
	pShader->SetMatrix(&mtxWorld);

	// ���C�g������ݒ�
	pShader->SetLightDirect(&mtxWorld, 0);

	for (int nCntMat = 0; nCntMat < (int)modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&GetPtrMaterial(nCntMat)->MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(modelData.pTextureID[nCntMat]));

		// �}�e���A����ݒ�
		pShader->SetMaterial(GetPtrMaterial(nCntMat)->MatD3D);

		// �e�N�X�`����ݒ�
		pShader->SetTexture(modelData.pTextureID[nCntMat]);

		// ��ԕύX�̓`�B
		pShader->CommitChanges();

		if (scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		modelData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �`��I��
	pShader->EndPass();
	pShader->End();
}