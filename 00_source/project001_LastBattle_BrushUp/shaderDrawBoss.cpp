//============================================================
//
//	�{�X�`��V�F�[�_�[�w�b�_�[ [shaderDrawBoss.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderDrawBoss.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���萔
	{
		"data\\TEXTURE\\SHADER\\toon003.png",	// �g�D�[���}�b�v�e�N�X�`��
	};

	const char* EFFECT_FX = "data\\SHADER\\shaderDrawBoss.fx";	// �{�X�`��V�F�[�_�[�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CDrawBossShader::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CDrawBossShader *CDrawBossShader::m_pShader = nullptr;	// �V�F�[�_�[���

//************************************************************
//	�e�N���X [CDrawBossShader] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CDrawBossShader::CDrawBossShader() :
	m_pTextureToon	(nullptr),	// �g�D�[���}�b�v�e�N�X�`��
	m_pTextureCrop	(nullptr),	// �؂蔲���e�N�X�`��
	m_pDirectLight	(nullptr),	// ���s�����̕����x�N�g��
	m_pDiffuse		(nullptr),	// �g�U��
	m_pAmbient		(nullptr),	// ����
	m_pEmissive		(nullptr),	// ���ˌ�
	m_pUseTexture	(nullptr)	// �e�N�X�`���̎g�p��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CDrawBossShader::~CDrawBossShader()
{

}

//============================================================
//	����������
//============================================================
HRESULT CDrawBossShader::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	LPD3DXBUFFER pError  = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_pTextureToon	= nullptr;	// �g�D�[���}�b�v�e�N�X�`��
	m_pTextureCrop	= nullptr;	// �؂蔲���e�N�X�`��
	m_pDirectLight	= nullptr;	// ���s�����̕����x�N�g��
	m_pDiffuse		= nullptr;	// �g�U��
	m_pAmbient		= nullptr;	// ����
	m_pEmissive		= nullptr;	// ���ˌ�
	m_pUseTexture	= nullptr;	// �e�N�X�`���̎g�p��

	// �V�F�[�_�[�̏�����
	if (FAILED(CShader::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�[�h�E�F�A�@�\���擾
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // ���_�E�s�N�Z���V�F�[�_�̃o�[�W�������g�p�\�ȏꍇ

		// �G�t�F�N�g�t�@�C���̓Ǎ�
		hr = D3DXCreateEffectFromFile
		( // ����
			pDevice,	// �f�o�C�X�ւ̃|�C���^
			EFFECT_FX,	// �G�t�F�N�g�t�@�C��
			nullptr,	// �v���v���Z�b�T��`
			nullptr,	// �C���N���[�h����
			0,			// �Ǎ��I�v�V�����t���O
			nullptr,	// �O���[�o���ϐ��C���^�[�t�F�[�X
			&pEffect,	// �G�t�F�N�g�C���^�[�t�F�[�X
			&pError		// �R���p�C���G���[���
		);
		if (SUCCEEDED(hr))
		{ // �Ǎ��ɐ��������ꍇ

			// �G�t�F�N�g����ݒ�
			SetEffect(pEffect);

			// �e�N�j�b�N�֐����擾�E�ݒ�
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// �e�}�g���b�N�X�̃O���[�o���ϐ����擾�E�ݒ�
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "g_mtxWorld"));		// ���[���h�}�g���b�N�X
			SetMatrixView(pEffect->GetParameterByName(nullptr, "g_mtxView"));		// �r���[�}�g���b�N�X
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "g_mtxProj"));	// �v���W�F�N�V�����}�g���b�N�X

			// �O���[�o���ϐ����擾
			m_pTextureToon	= pEffect->GetParameterByName(nullptr, "g_textureToon");	// �g�D�[���}�b�v�e�N�X�`��
			m_pTextureCrop	= pEffect->GetParameterByName(nullptr, "g_textureCrop");	// �؂蔲���e�N�X�`��
			m_pDirectLight	= pEffect->GetParameterByName(nullptr, "g_dirLight");		// ���s�����̕����x�N�g��
			m_pDiffuse		= pEffect->GetParameterByName(nullptr, "g_diffuse");		// �g�U��
			m_pAmbient		= pEffect->GetParameterByName(nullptr, "g_ambient");		// ����
			m_pEmissive		= pEffect->GetParameterByName(nullptr, "g_emissive");		// ���ˌ�
			m_pUseTexture	= pEffect->GetParameterByName(nullptr, "g_bUseTex");		// �e�N�X�`���̎g�p��

			// �g�D�[���}�b�v�e�N�X�`����ݒ�
			SetToonMapTexture(TEXTURE_TOON);

			// �؂蔲���e�N�X�`����ݒ�
			SetCropTexture(GET_RENDERER->GetCropTextureID());
		}
		else
		{ // �Ǎ��Ɏ��s�����ꍇ

			// �ǂݍ��ݎ��s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // �o�[�W�������g�p�s�ȏꍇ

		// �Â��o�[�W�����ɂ�鎸�s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CDrawBossShader::Uninit(void)
{
	// �V�F�[�_�[�̏I��
	CShader::Uninit();
}

//============================================================
//	���C�g�����x�N�g���̐ݒ菈��
//============================================================
void CDrawBossShader::SetLightDirect(D3DXMATRIX *pMtxWorld, const int nLightID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR3 dirLight = GET_MANAGER->GetLight()->GetLight(nLightID).Direction;	// ���C�g�����v�Z�p
	D3DXVECTOR4 setLight = D3DXVECTOR4(dirLight.x, dirLight.y, dirLight.z, 0.0f);	// ���C�g�����ݒ�p
	D3DXMATRIX  mtxInvWorld;	// ���[���h�}�g���b�N�X�t�s��

	// ���s�����̕����x�N�g�����v�Z
	D3DXMatrixInverse(&mtxInvWorld, nullptr, pMtxWorld);	// ���[���h�}�g���b�N�X�̋t�s����v�Z
	D3DXVec4Transform(&setLight, &setLight, &mtxInvWorld);	// �}�g���b�N�X���x�N�g���ɕϊ�

	// ���C�g�̕����x�N�g���𐳋K��
	dirLight = D3DXVECTOR3(setLight.x, setLight.y, setLight.z);			// �v�Z�����x�N�g������
	D3DXVec3Normalize(&dirLight, &dirLight);							// �x�N�g���𐳋K��
	setLight = D3DXVECTOR4(dirLight.x, dirLight.y, dirLight.z, 0.0f);	// ���K�������x�N�g����ݒ�

	// �G�t�F�N�g�ɕ��s�����̕����x�N�g����ݒ�
	GetEffect()->SetVector(m_pDirectLight, &setLight);
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�|�C���^)
//============================================================
void CDrawBossShader::SetTexture(const CTexture::STexture& rTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (rTexture.pTexture == nullptr)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		GetEffect()->SetBool(m_pUseTexture, false);
	}
	else
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		GetEffect()->SetBool(m_pUseTexture, true);
	}
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�C���f�b�N�X)
//============================================================
void CDrawBossShader::SetTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (nTextureID == NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		GetEffect()->SetBool(m_pUseTexture, false);
	}
	else if (nTextureID > NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		GetEffect()->SetBool(m_pUseTexture, true);
	}
	else { assert(false); }	// �g�p�s�C���f�b�N�X
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CDrawBossShader::SetMaterial(const D3DMATERIAL9& rMaterial)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �|�C���^��錾
	LPD3DXEFFECT pEffect = GetEffect();	// �G�t�F�N�g���

	// �ϐ���錾
	D3DXVECTOR4 diffuse  = D3DXVECTOR4(rMaterial.Diffuse.r,  rMaterial.Diffuse.g,  rMaterial.Diffuse.b,  rMaterial.Diffuse.a);	// �g�U��
	D3DXVECTOR4 ambient  = D3DXVECTOR4(rMaterial.Ambient.r,  rMaterial.Ambient.g,  rMaterial.Ambient.b,  rMaterial.Ambient.a);	// ����
	D3DXVECTOR4 emissive = D3DXVECTOR4(rMaterial.Emissive.r, rMaterial.Emissive.g, rMaterial.Emissive.b, rMaterial.Emissive.a);	// ���ˌ�

	// �G�t�F�N�g�Ƀ}�e���A���̊g�U���E�����E���ˌ���ݒ�
	pEffect->SetVector(m_pDiffuse,  &diffuse);
	pEffect->SetVector(m_pAmbient,  &ambient);
	pEffect->SetVector(m_pEmissive, &emissive);
}

//============================================================
//	�g�U���̐ݒ菈��
//============================================================
void CDrawBossShader::SetDiffuse(const D3DXCOLOR& rDiffuse)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rDiffuse.r, rDiffuse.g, rDiffuse.b, rDiffuse.a);	// �g�U��

	// �G�t�F�N�g�Ɋg�U����ݒ�
	GetEffect()->SetVector(m_pDiffuse, &diffuse);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CDrawBossShader::SetAmbient(const D3DXCOLOR& rAmbient)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 ambient = D3DXVECTOR4(rAmbient.r, rAmbient.g, rAmbient.b, rAmbient.a);	// ����

	// �G�t�F�N�g�Ɋ�����ݒ�
	GetEffect()->SetVector(m_pAmbient, &ambient);
}

//============================================================
//	���ˌ��̐ݒ菈��
//============================================================
void CDrawBossShader::SetEmissive(const D3DXCOLOR& rEmissive)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 emissive = D3DXVECTOR4(rEmissive.r, rEmissive.g, rEmissive.b, rEmissive.a);	// ���ˌ�

	// �G�t�F�N�g�ɕ��ˌ���ݒ�
	GetEffect()->SetVector(m_pEmissive, &emissive);
}

//============================================================
//	�g�U���̂ݐݒ菈��
//============================================================
void CDrawBossShader::SetOnlyDiffuse(const D3DXCOLOR& rDiffuse)
{
	// �g�U���̐ݒ�
	SetDiffuse(rDiffuse);

	// �����̐ݒ�
	SetAmbient(XCOL_ABLACK);

	// ���ˌ��̐ݒ�
	SetEmissive(XCOL_ABLACK);
}

//============================================================
//	��������
//============================================================
CDrawBossShader *CDrawBossShader::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �{�X�`��V�F�[�_�[�̐���
	m_pShader = new CDrawBossShader;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �{�X�`��V�F�[�_�[�̏�����
		if (FAILED(m_pShader->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �{�X�`��V�F�[�_�[�̔j��
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return m_pShader;
	}
}

//============================================================
//	�擾����
//============================================================
CDrawBossShader *CDrawBossShader::GetInstance(void)
{
	// �C���X�^���X���g�p
	assert(m_pShader != nullptr);

	// �{�X�`��V�F�[�_�[�̃|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CDrawBossShader::Release(void)
{
	// �{�X�`��V�F�[�_�[�̏I��
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// �������J��
	SAFE_DELETE(m_pShader);
}

//============================================================
//	�{�X�`��}�b�v�e�N�X�`���̐ݒ菈��
//============================================================
void CDrawBossShader::SetToonMapTexture(const ETexture texture)
{
	if (texture > NONE_IDX && texture < TEXTURE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p
		CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`�����

		// �G�t�F�N�g�Ƀg�D�[���}�b�v�p�e�N�X�`����ݒ�
		GetEffect()->SetTexture(m_pTextureToon, pTexture->GetPtr(pTexture->Regist(TEXTURE_FILE[texture])));
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�؂蔲���e�N�X�`���̐ݒ菈��
//============================================================
void CDrawBossShader::SetCropTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`�����

	// �G�t�F�N�g�ɐ؂蔲���p�e�N�X�`����ݒ�
	GetEffect()->SetTexture(m_pTextureCrop, pTexture->GetPtr(nTextureID));
}