//============================================================
//
//	�I�u�W�F�N�g2D���� [object2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int MAX_VERTEX = 4;	// ���_��
}

//************************************************************
//	�q�N���X [CObject2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject2D::CObject2D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),		// ���_�o�b�t�@�ւ̃|�C���^
	m_pRenderState	(nullptr),		// �����_�[�X�e�[�g�̏��
	m_pos			(VEC3_ZERO),	// �ʒu
	m_rot			(VEC3_ZERO),	// ����
	m_size			(VEC3_ZERO),	// �傫��
	m_col			(XCOL_WHITE),	// �F
	m_fAngle		(0.0f),			// �Ίp���̊p�x
	m_fLength		(0.0f),			// �Ίp���̒���
	m_nTextureID	(0)				// �e�N�X�`���C���f�b�N�X
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject2D::~CObject2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObject2D::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	m_pVtxBuff		= nullptr;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pRenderState	= nullptr;		// �����_�[�X�e�[�g�̏��
	m_pos			= VEC3_ZERO;	// �ʒu
	m_rot			= VEC3_ZERO;	// ����
	m_size			= VEC3_ZERO;	// �傫��
	m_col			= XCOL_WHITE;	// �F
	m_fAngle		= 0.0f;			// �Ίp���̊p�x
	m_fLength		= 0.0f;			// �Ίp���̒���
	m_nTextureID	= NONE_IDX;		// �e�N�X�`���C���f�b�N�X

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * MAX_VERTEX,	// �K�v���_��
		D3DUSAGE_WRITEONLY,				// �g�p���@
		object::FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// �������̎w��
		&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
		nullptr
	)))
	{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�̐���
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObject2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pVtxBuff);

	// �����_�[�X�e�[�g�̔j��
	SAFE_REF_RELEASE(m_pRenderState);

	// 2D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObject2D::Update(void)
{
	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CObject2D::Draw(CShader * /*pShader*/)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	// �����_�[�X�e�[�g��ݒ�
	m_pRenderState->Set();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(object::FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �����_�[�X�e�[�g���Đݒ�
	m_pRenderState->Reset();
}

//============================================================
//	��������
//============================================================
CObject2D *CObject2D::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �I�u�W�F�N�g2D�̐���
	CObject2D *pObject2D = new CObject2D;
	if (pObject2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g2D�̏�����
		if (FAILED(pObject2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g2D�̔j��
			SAFE_DELETE(pObject2D);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObject2D->SetVec3Position(rPos);

		// ������ݒ�
		pObject2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObject2D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pObject2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pObject2D;
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObject2D::BindTexture(const int nTextureID)
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
void CObject2D::BindTexture(const char *pTexturePass)
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
//	�e�N�X�`���C���f�b�N�X�擾����
//============================================================
int CObject2D::GetTextureIndex(void) const
{
	// �e�N�X�`���C���f�b�N�X��Ԃ�
	return m_nTextureID;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObject2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObject2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_size = rSize;

	// �Ίp���̊p�x�����߂�
	m_fAngle = atan2f(m_size.x, m_size.y);

	// �Ίp���̒��������߂�
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObject2D::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject2D::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObject2D::GetVec3Sizing(void) const
{
	// �傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObject2D::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	�����_�[�X�e�[�g���̎擾����
//============================================================
CRenderState *CObject2D::GetRenderState(void)
{
	// �C���X�^���X���g�p
	assert(m_pRenderState != nullptr);

	// �����_�[�X�e�[�g�̏���Ԃ�
	return m_pRenderState;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObject2D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W��ݒ�
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.z = 0.0f;

		// rhw �̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�A�j���[�V�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObject2D::SetAnimTex(const int nPattern, const int nWidthPtrn, const int nHeightPtrn)
{
	// �ϐ���錾
	float fWidthRate	= 1.0f / nWidthPtrn;	// ���̕������̊���
	float fHeightRate	= 1.0f / nHeightPtrn;	// �c�̕������̊���
	int nWidthCurrent	= nPattern % nWidthPtrn;					// ���݂̉��̃p�^�[��
	int nHeightCurrent	= (nPattern / nWidthPtrn) % nHeightPtrn;	// ���݂̏c�̃p�^�[��

	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate *  nHeightCurrent);
		pVtx[1].tex = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate *  nHeightCurrent);
		pVtx[2].tex = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate * (nHeightCurrent + 1));
		pVtx[3].tex = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate * (nHeightCurrent + 1));

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�X�N���[���̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObject2D::SetScrollTex(const float fTexU, const float fTexV)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != nullptr)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(fTexU,		fTexV);
		pVtx[1].tex = D3DXVECTOR2(fTexU + 1.0f,	fTexV);
		pVtx[2].tex = D3DXVECTOR2(fTexU,		fTexV + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fTexU + 1.0f,	fTexV + 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�j������
//============================================================
void CObject2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}