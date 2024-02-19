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
#include "objectMeshTube.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\magicCircle000.png",	// �ʏ�
	};

	const int	PRIORITY		= 2;		// ���@�w�̗D�揇��
	const float	CROP_LENGTH		= -1000.0f;	// �����̒���
	const POSGRID2 PART_CIRCLE	= POSGRID2(32, 2);	// ���@�w�̕�����
	const POSGRID2 PART_CROP	= POSGRID2(32, 2);	// ���@�w�̕�����
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CMagicCircle::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CMagicCircle> *CMagicCircle::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CMagicCircle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagicCircle::CMagicCircle() : CObjectMeshCircle(CObject::LABEL_MAGIC_CIRCLE, CObject::DIM_3D, PRIORITY),
	m_pCropTube	(nullptr)	// �؂蔲���p�`���[�u�̏��
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
	m_pCropTube = nullptr;	// �؂蔲���p�`���[�u�̏��

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
		pRenderState->SetZFunc(D3DCMP_ALWAYS);

		// �|���S���̗��ʂ�\����Ԃɂ���
		pRenderState->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��OFF�ɂ���
		pRenderState->SetLighting(false);
	}

	//--------------------------------------------------------
	//	�؂蔲���p�`���[�u�̏������E�ݒ�
	//--------------------------------------------------------
	{
		// �؂蔲���p�`���[�u�̐���
		m_pCropTube = CObjectMeshTube::Create
		( // ����
			VEC3_ZERO,	// �ʒu
			VEC3_ZERO,	// ����
			XCOL_WHITE,	// �F
			PART_CROP,	// ������
			GRID2_ONE,	// �e�N�X�`��������
			0.0f,		// ���a
			CROP_LENGTH	// �c��
		);
		if (m_pCropTube == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����`���OFF�ɂ���
		m_pCropTube->SetEnableDraw(false);

		// �����_�[�X�e�[�g�̏����擾
		CRenderState renderState = m_pCropTube->GetRenderState();

		// �|���S���̗��ʂ�\����Ԃɂ���
		renderState.SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��OFF�ɂ���
		renderState.SetLighting(false);

		// �����_�[�X�e�[�g�̏��𔽉f
		m_pCropTube->SetRenderState(renderState);
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CMagicCircle>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMagicCircle::Uninit(void)
{
	// �؂蔲���p�`���[�u�̏I��
	SAFE_UNINIT(m_pCropTube);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

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
void CMagicCircle::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g���b�V���T�[�N���̕`��
	CObjectMeshCircle::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// ���g�̈ʒu��ݒ�
	CObjectMeshCircle::SetVec3Position(rPos);

	// �؂蔲�����a�̈ʒu��ݒ�
	m_pCropTube->SetVec3Position(rPos);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagicCircle::SetVec3Rotation(const D3DXVECTOR3 &rRot)
{
	// ���g�̌�����ݒ�
	CObjectMeshCircle::SetVec3Rotation(rRot);

	// �؂蔲�����a�̌�����ݒ�
	m_pCropTube->SetVec3Rotation(rRot);
}

//============================================================
//	��������
//============================================================
CMagicCircle *CMagicCircle::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const float fRadius,		// ���a
	const float fCropRadius		// �؂蔲�����a
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
		pMagicCircle->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[TEXTURE_NORMAL]));

		// �ʒu��ݒ�
		pMagicCircle->SetVec3Position(rPos);

		// ������ݒ�
		pMagicCircle->SetVec3Rotation(rRot);

		// ���a��ݒ�
		pMagicCircle->SetRadius(fRadius);

		// �؂蔲�����a��ݒ�
		pMagicCircle->SetCropRadius(fCropRadius);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagicCircle;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CMagicCircle>* CMagicCircle::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�؂蔲���p�̕`�揈��
//============================================================
void CMagicCircle::DrawCrop(void)
{
	// �����`�悪OFF�̏ꍇ������
	if (!IsDraw()) { return; }

	LPDIRECT3DDEVICE9 pDevice	= GET_DEVICE;	// �f�o�C�X���
	CTexture	*pTexture		= GET_MANAGER->GetTexture();	// �e�N�X�`�����
	CMonoShader	*pMonoShader	= CMonoShader::GetInstance();	// �P�F�`��V�F�[�_�[���

	if (pDevice == nullptr || pTexture == nullptr || pMonoShader == nullptr)
	{ // ��񂪖������̂��������ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	if (pMonoShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �s�N�Z���`��F�𔒂ɐݒ�
		pMonoShader->SetColor(XCOL_WHITE);

		// �X�e���V���e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

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

		// �؂蔲���p�`���[�u�̕`��
		m_pCropTube->Draw(pMonoShader);

		// �X�e���V���e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

//============================================================
//	�؂蔲�����a�̐ݒ菈��
//============================================================
void CMagicCircle::SetCropRadius(const float fRadius)
{
	// �؂蔲���p�`���[�u�̔��a��ݒ�
	m_pCropTube->SetRadius(fRadius);
}
