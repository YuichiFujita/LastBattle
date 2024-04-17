#if 1
//============================================================
//
//	�I�u�W�F�N�g�X�L�����b�V��3D���� [objectSkinMesh3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectSkinMesh3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\TXT\\skinmesh.txt";		// �X�L�����b�V���Z�b�g�A�b�v�e�L�X�g
}

//************************************************************
//	�q�N���X [CObjectSkinMesh3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectSkinMesh3D::CObjectSkinMesh3D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),			// ���_�o�b�t�@�ւ̃|�C���^
	m_pBone			(nullptr),			// �{�[�����
	m_pVtxPlus		(nullptr),			// ���_�ǉ����
	m_pRenderState	(nullptr),			// �����_�[�X�e�[�g�̏��
	m_pos			(VEC3_ZERO),		// �ʒu
	m_rot			(VEC3_ZERO),		// ����
	m_size			(VEC3_ZERO),		// �傫��
	m_col			(XCOL_WHITE),		// �F
	m_nTextureID	(0),				// �e�N�X�`���C���f�b�N�X
	m_nNumBone		(0),				// �{�[����
	m_nNumVtx		(0)					// ���_��
{
	// �����o�ϐ����N���A
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectSkinMesh3D::~CObjectSkinMesh3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectSkinMesh3D::Init(void)
{
	// �����o�ϐ���������
	D3DXMatrixIdentity(&m_mtxWorld);	// ���[���h�}�g���b�N�X
	m_pVtxBuff		= nullptr;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pBone			= nullptr;			// �{�[�����
	m_pVtxPlus		= nullptr;			// ���_�ǉ����
	m_pRenderState	= nullptr;			// �����_�[�X�e�[�g�̏��
	m_pos			= VEC3_ZERO;		// �ʒu
	m_rot			= VEC3_ZERO;		// ����
	m_size			= VEC3_ZERO;		// �傫��
	m_col			= XCOL_WHITE;		// �F
	m_nTextureID	= NONE_IDX;			// �e�N�X�`���C���f�b�N�X
	m_nNumBone		= 0;				// �{�[����
	m_nNumVtx		= 0;				// ���_��

	// �����_�[�X�e�[�g�̐���
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �J�����O��OFF�ɂ���
	m_pRenderState->SetCulling(D3DCULL_NONE);

	// ���C�e�B���O��OFF�ɂ���
	m_pRenderState->SetLighting(false);

	// TODO
	// �e�N�X�`����ݒ�
	m_nTextureID = GET_MANAGER->GetTexture()->Regist("data\\TEXTURE\\player000.jpg");

	// �Z�b�g�A�b�v����
	LoadSetup();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectSkinMesh3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// ���_�ǉ����̔j��
	SAFE_DEL_ARRAY(m_pVtxPlus);

	// �{�[���̔j��
	SAFE_DEL_ARRAY(m_pBone);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// �I�u�W�F�N�g�X�L�����b�V��3D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectSkinMesh3D::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectSkinMesh3D::Draw(CShader *pShader)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_3D);

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

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	��������
//============================================================
CObjectSkinMesh3D *CObjectSkinMesh3D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �I�u�W�F�N�g�X�L�����b�V��3D�̐���
	CObjectSkinMesh3D *pObjectSkinMesh3D = new CObjectSkinMesh3D;
	if (pObjectSkinMesh3D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�X�L�����b�V��3D�̏�����
		if (FAILED(pObjectSkinMesh3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�X�L�����b�V��3D�̔j��
			SAFE_DELETE(pObjectSkinMesh3D);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObjectSkinMesh3D->SetVec3Position(rPos);

		// ������ݒ�
		pObjectSkinMesh3D->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectSkinMesh3D;
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectSkinMesh3D::BindTexture(const int nTextureID)
{
	if (nTextureID >= NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObjectSkinMesh3D::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	if (pTexturePass != nullptr)
	{ // ���蓖�Ă�e�N�X�`���p�X������ꍇ

		// �e�N�X�`���C���f�b�N�X��ݒ�
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else
	{ // ���蓖�Ă�e�N�X�`���p�X���Ȃ��ꍇ

		// �e�N�X�`���Ȃ��C���f�b�N�X��ݒ�
		m_nTextureID = NONE_IDX;
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectSkinMesh3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectSkinMesh3D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectSkinMesh3D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectSkinMesh3D::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�{�[���̐�������
//============================================================
HRESULT CObjectSkinMesh3D::CreateBone(const int nNumBone)
{
	// �{�[���������R���ł͂Ȃ��ꍇ������
	if (nNumBone <= 0) { assert(false); return E_FAIL; }

	// �{�[���̔j��
	SAFE_DEL_ARRAY(m_pBone);
	if (m_pBone != nullptr)
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �{�[���̐���
	m_pBone = new SBone[nNumBone];
	if (m_pBone == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���_�̐�������
//============================================================
HRESULT CObjectSkinMesh3D::CreateVertex(const int nNumVtx)
{
	// ���_�������R���ł͂Ȃ��ꍇ������
	if (nNumVtx <= 0) { assert(false); return E_FAIL; }

	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);
	if (m_pVtxBuff != nullptr)
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(GET_DEVICE->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * nNumVtx,	// �K�v���_��
		D3DUSAGE_WRITEONLY,		// �g�p���@
		object::FVF_VERTEX_3D,	// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,		// �������̎w��
		&m_pVtxBuff,			// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_�ǉ����̔j��
	SAFE_DEL_ARRAY(m_pVtxPlus);
	if (m_pVtxPlus != nullptr)
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_�ǉ����̐���
	m_pVtxPlus = new SVtx[nNumVtx];
	if (m_pVtxPlus == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�{�[�����̐ݒ菈��
//============================================================
void CObjectSkinMesh3D::SetBoneInfo
(
	const int nID,				// �p�[�c�C���f�b�N�X
	const int nParentID,		// �e�C���f�b�N�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	if (nID < m_nNumBone && nParentID < m_nNumBone)
	{ // �C���f�b�N�X���{�[�������͈͓̔��̏ꍇ

		// �I�t�Z�b�g�ʒu�E�����̐ݒ�
		m_pBone[nID].pos = rPos;
		m_pBone[nID].rot = rRot;

#if 0
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
#else
		// �e�C���f�b�N�X�̐ݒ�
		m_pBone[nID].nParentID = nParentID;
#endif
	}
	else { assert(false); }
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectSkinMesh3D::SetVertexInfo
(
	const int nVtxID,	// ���_�C���f�b�N�X
	const SVtx& rVtx,	// ���_�ǉ����
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR2& rTex		// �e�N�X�`�����W
)
{
	if (m_pVtxBuff != nullptr && nVtxID < m_nNumVtx)
	{ // ���_�������ς݊��A���_�����͈͓̔��̏ꍇ

		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�f�[�^�������C���f�b�N�X�܂Ői�߂�
		pVtx += nVtxID;

		// ���_����ݒ�
		pVtx->pos = rPos;		// ���_���W
		pVtx->nor = VEC3_ZERO;	// �@���x�N�g��
		pVtx->col = XCOL_WHITE;	// ���_�J���[
		pVtx->tex = rTex;		// �e�N�X�`�����W

		// ���_�ǉ�����ݒ�
		m_pVtxPlus[nVtxID] = rVtx;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObjectSkinMesh3D::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	�j������
//============================================================
void CObjectSkinMesh3D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�ʏ�`�揈��
//============================================================
void CObjectSkinMesh3D::DrawNormal(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);
}

//============================================================
//	�V�F�[�_�[�`�揈��
//============================================================
void CObjectSkinMesh3D::DrawShader(CShader *pShader)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �`��J�n
	pShader->Begin();
	pShader->BeginPass(0);

	// �}�g���b�N�X����ݒ�
	pShader->SetMatrix(&m_mtxWorld);

	// ���C�g������ݒ�
	pShader->SetLightDirect(&m_mtxWorld, 0);

	// �g�U����ݒ�
	pShader->SetOnlyDiffuse(m_col);

	// �e�N�X�`����ݒ�
	pShader->SetTexture(m_nTextureID);

	// ��ԕύX�̓`�B
	pShader->CommitChanges();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �`��I��
	pShader->EndPass();
	pShader->End();
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CObjectSkinMesh3D::LoadSetup(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu
	D3DXVECTOR3 rot = VEC3_ZERO;	// ����
	D3DXVECTOR2 tex = VEC2_ZERO;	// �e�N�X�`�����W
	SVtx vtx;	// ���_���

	int nBoneID		= 0;	// �{�[���C���f�b�N�X
	int nVtxID		= 0;	// ���_�C���f�b�N�X
	int nParentID	= 0;	// �e�C���f�b�N�X
	int nEnd		= 0;	// �Ǎ��I���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �{�[���̐ݒ�
			if (strcmp(&aString[0], "BONESET") == 0)
			{ // �ǂݍ��񂾕����� BONESET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_BONESET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_BONE") == 0)
					{ // �ǂݍ��񂾕����� NUM_BONE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_nNumBone);	// �{�[������ǂݍ���

						// �{�[���̐���
						CreateBone(m_nNumBone);
					}
					else if (strcmp(&aString[0], "BONE") == 0)
					{ // �ǂݍ��񂾕����� BONE �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_BONE �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// �e�C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_BONE") != 0);	// �ǂݍ��񂾕����� END_BONE �ł͂Ȃ��ꍇ���[�v

						// �{�[�����̐ݒ�
						SetBoneInfo(nBoneID, nParentID, pos, rot);

						// ���̃C���f�b�N�X�Ɉڍs
						nBoneID++;
					}
				} while (strcmp(&aString[0], "END_BONESET") != 0);	// �ǂݍ��񂾕����� END_BONESET �ł͂Ȃ��ꍇ���[�v
			}

			// ���_�̐ݒ�
			if (strcmp(&aString[0], "VERTEXSET") == 0)
			{ // �ǂݍ��񂾕����� VERTEXSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_VERTEXSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_VERTEX") == 0)
					{ // �ǂݍ��񂾕����� NUM_VERTEX �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &m_nNumVtx);	// ���_����ǂݍ���

						// ���_�̐���
						CreateVertex(m_nNumVtx);
					}
					else if (strcmp(&aString[0], "VERTEX") == 0)
					{ // �ǂݍ��񂾕����� VERTEX �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_VERTEX �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "BONE_REF") == 0)
							{ // �ǂݍ��񂾕����� BONE_REF �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &vtx.aBoneRef[0]);	// �{�[���Q�ƒl0��ǂݍ���
								fscanf(pFile, "%d", &vtx.aBoneRef[1]);	// �{�[���Q�ƒl1��ǂݍ���
							}
							else if (strcmp(&aString[0], "WEIGHT_VALUE") == 0)
							{ // �ǂݍ��񂾕����� WEIGHT_VALUE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &vtx.aWeight[0]);	// �{�[���e���x0��ǂݍ���
								fscanf(pFile, "%f", &vtx.aWeight[1]);	// �{�[���e���x1��ǂݍ���
							}
							else if (strcmp(&aString[0], "TEX") == 0)
							{ // �ǂݍ��񂾕����� TEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &tex.x);		// �e�N�X�`��X���W��ǂݍ���
								fscanf(pFile, "%f", &tex.y);		// �e�N�X�`��Y���W��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_VERTEX") != 0);	// �ǂݍ��񂾕����� END_VERTEX �ł͂Ȃ��ꍇ���[�v

						// ���_���̐ݒ�
						SetVertexInfo(nVtxID, vtx, pos, tex);

						// ���̃C���f�b�N�X�Ɉڍs
						nVtxID++;
					}
				} while (strcmp(&aString[0], "END_VERTEXSET") != 0);	// �ǂݍ��񂾕����� END_VERTEXSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�L�����b�V���Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
#endif
