//============================================================
//
//	���@�w���� [magicCircle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magicCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectMeshCylinder.h"
#include "objectMeshRing.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\magicCircle000.png",	// �ʏ�
	};

	const int	PRIORITY		= 5;		// ���@�w�̗D�揇��
	const float	ALPHA_LENGTH	= -1000.0f;	// �����̒���
	const POSGRID2 PART_CIRCLE	= POSGRID2(32, 2);	// ���@�w�̕�����
	const POSGRID2 PART_ALPHA	= POSGRID2(32, 1);	// �����̕�����
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMagicCircle::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�q�N���X [CMagicCircle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagicCircle::CMagicCircle() : CObjectMeshCircle(CObject::LABEL_MAGIC_CIRCLE, CObject::DIM_3D, PRIORITY),
	m_pAlphaCylinder	(nullptr),	// ���@�w�̐�̓������
	m_pAlphaRing		(nullptr)	// ���@�w�̋󔒂̓������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagicCircle::~CMagicCircle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagicCircle::Init(void)
{
	// �����o�ϐ���������
	m_pAlphaCylinder = nullptr;	// ���@�w�̐�̓������
	m_pAlphaRing = nullptr;		// ���@�w�̋󔒂̓������

	//--------------------------------------------------------
	//	���N���X�̏������E�ݒ�
	//--------------------------------------------------------
	{
		// �I�u�W�F�N�g���b�V���T�[�N���̏�����
		if (FAILED(CObjectMeshCircle::Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������̐ݒ�
		if (FAILED(SetPattern(PART_CIRCLE)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����_�[�X�e�[�g�̏����擾
		CRenderState *pRenderState = GetRenderState();

		// Z�e�X�g���ΐ����ɂ���
		//pRenderState->SetZFunc(D3DCMP_ALWAYS);

		// �|���S���̗��ʂ�\����Ԃɂ���
		pRenderState->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��OFF�ɂ���
		pRenderState->SetLighting(false);
	}

	//--------------------------------------------------------
	//	���@�w�̓����V�����_�[�̏������E�ݒ�
	//--------------------------------------------------------
	{
		// ���@�w�̐�̓����\���̐���
		m_pAlphaCylinder = CObjectMeshCylinder::Create
		( // ����
			VEC3_ZERO,		// �ʒu
			VEC3_ZERO,		// ����
			XCOL_WHITE,	// �F
			PART_ALPHA,		// ������
			GRID2_ONE,		// �e�N�X�`��������
			0.0f,			// ���a
			ALPHA_LENGTH	// �c��
		);
		if (m_pAlphaCylinder == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_pAlphaCylinder->SetEnableDraw(false);

		// Z�e�X�g���ΐ����ɂ���
		//m_pAlphaCylinder->GetRenderState()->SetZFunc(D3DCMP_ALWAYS);

		// �|���S���̗��ʂ�\����Ԃɂ���
		m_pAlphaCylinder->GetRenderState()->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��OFF�ɂ���
		m_pAlphaCylinder->GetRenderState()->SetLighting(false);
	}

	//--------------------------------------------------------
	//	���@�w�̓��������O�̏������E�ݒ�
	//--------------------------------------------------------
	{
		// ���@�w�̋󔒂̓����\���̐���
		m_pAlphaRing = CObjectMeshRing::Create
		( // ����
			VEC3_ZERO,		// �ʒu
			VEC3_ZERO,		// ����
			XCOL_WHITE,	// �F
			PART_ALPHA,		// ������
			GRID2_ONE,		// �e�N�X�`��������
			0.0f,			// ���̔��a
			0.0f,			// ����
			0.0f			// �O����Y���W���Z��
		);
		if (m_pAlphaRing == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_pAlphaRing->SetEnableDraw(false);

		// Z�e�X�g���ΐ����ɂ���
		//m_pAlphaRing->GetRenderState()->SetZFunc(D3DCMP_ALWAYS);

		// �|���S���̗��ʂ�\����Ԃɂ���
		m_pAlphaRing->GetRenderState()->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��OFF�ɂ���
		m_pAlphaRing->GetRenderState()->SetLighting(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMagicCircle::Uninit(void)
{
	// ���@�w�̐�̓������̏I��
	SAFE_UNINIT(m_pAlphaCylinder);

	// ���@�w�̋󔒂̓������̏I��
	SAFE_UNINIT(m_pAlphaRing);

	// �I�u�W�F�N�g���b�V���T�[�N���̏I��
	CObjectMeshCircle::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CMagicCircle::Update(void)
{
	// �I�u�W�F�N�g���b�V���T�[�N���̍X�V
	CObjectMeshCircle::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CMagicCircle::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X�̃|�C���^

	if (GET_INPUTKEY->IsPress(DIK_0))
	{
		// �X�e���V���e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	}

	// ��r�Q�ƒl��ݒ肷��
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	// �X�e���V���}�X�N���w�肷�� 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// �X�e���V����r�֐����w�肷��
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �I�u�W�F�N�g���b�V���T�[�N���̕`��
	CObjectMeshCircle::Draw();

	// ���@�w�̐�̓������̕`��
	m_pAlphaCylinder->Draw();

	// ���@�w�̋󔒂̓������̕`��
	m_pAlphaRing->Draw();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// ���g�̈ʒu��ݒ�
	CObjectMeshCircle::SetVec3Position(rPos);

	// ���@�w�̐�̓����\���̈ʒu��ݒ�
	m_pAlphaCylinder->SetVec3Position(rPos);

	// ���@�w�̋󔒂̓����\���̈ʒu��ݒ�
	m_pAlphaRing->SetVec3Position(rPos);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// ���g�̌�����ݒ�
	CObjectMeshCircle::SetVec3Rotation(rRot);

	// ���@�w�̐�̓����\���̌�����ݒ�
	m_pAlphaCylinder->SetVec3Rotation(rRot);

	// ���@�w�̋󔒂̓����\���̌�����ݒ�
	m_pAlphaRing->SetVec3Rotation(rRot);
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CMagicCircle::SetRadius(const float fRadius)
{
	// ���g�̔��a��ݒ�
	CObjectMeshCircle::SetRadius(fRadius);

	// ���@�w�̋󔒂����Ԃ𖄂߂�傫���ɐݒ�
	m_pAlphaRing->SetHoleRadius(fRadius);
	m_pAlphaRing->SetThickness(m_pAlphaCylinder->GetRadius() - fRadius);
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CMagicCircle::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);			// ���g
	//m_pAlphaCylinder->SetEnableDraw(bDraw);	// ���@�w�̐�̓������
	//m_pAlphaRing->SetEnableDraw(bDraw);		// ���@�w�̋󔒂̓������
}

//============================================================
//	��������
//============================================================
CMagicCircle *CMagicCircle::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const float fRadius,		// ���a
	const float fAlphaRadius	// �������a
)
{
	// ���@�w�̐���
	CMagicCircle *pMagicCircle = new CMagicCircle;
	if (pMagicCircle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���@�w�̏�����
		if (FAILED(pMagicCircle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���@�w�̔j��
			SAFE_DELETE(pMagicCircle);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		//pMagicCircle->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[TEXTURE_NORMAL]));

		// �ʒu��ݒ�
		pMagicCircle->SetVec3Position(rPos);

		// ������ݒ�
		pMagicCircle->SetVec3Rotation(rRot);

		// ���a��ݒ�
		pMagicCircle->SetRadius(fRadius);

		// �������a��ݒ�
		pMagicCircle->SetAlphaRadius(fAlphaRadius);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagicCircle;
	}
}

//============================================================
//	�������a�̐ݒ菈��
//============================================================
void CMagicCircle::SetAlphaRadius(const float fRadius)
{
	// ���@�w�̐�̓����\���̔��a��ݒ�
	m_pAlphaCylinder->SetRadius(fRadius);

	// ���@�w�̋󔒂����Ԃ𖄂߂�傫���ɐݒ�
	float fThisRadius = GetRadius();	// ���@�w���g�̔��a
	m_pAlphaRing->SetHoleRadius(fThisRadius);
	m_pAlphaRing->SetThickness(fRadius - fThisRadius);
}
