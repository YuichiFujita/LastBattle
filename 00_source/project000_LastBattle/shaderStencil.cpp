//============================================================
//
//	�X�e���V���V�F�[�_�[�w�b�_�[ [shaderStencil.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderStencil.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderStencil.fx";	// �X�e���V���V�F�[�_�[�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStencilShader *CStencilShader::m_pShader = nullptr;	// �V�F�[�_�[���

//************************************************************
//	�e�N���X [CStencilShader] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStencilShader::CStencilShader() :
	m_pRefValue		(nullptr),	// �Q�ƒl
	m_pComparison	(nullptr),	// ��r���Z�q
	m_pOperation	(nullptr)	// ���l����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CStencilShader::~CStencilShader()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStencilShader::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	LPD3DXBUFFER pError  = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_pRefValue		= nullptr;	// �Q�ƒl
	m_pComparison	= nullptr;	// ��r���Z�q
	m_pOperation	= nullptr;	// ���l����

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
			m_pTextureStencil	= pEffect->GetParameterByName(nullptr, "g_textureStencil");	// �X�e���V���e�N�X�`��
			m_pRefValue			= pEffect->GetParameterByName(nullptr, "g_nRefValue");		// �Q�ƒl
			m_pComparison		= pEffect->GetParameterByName(nullptr, "g_nComparison");	// ��r���Z�q
			m_pOperation		= pEffect->GetParameterByName(nullptr, "g_nOperation");		// ���l����

			// �X�e���V���e�N�X�`����ݒ�
			SetTexture(GET_RENDERER->m_nStencilTexID);
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
void CStencilShader::Uninit(void)
{
	// �V�F�[�_�[�̏I��
	CShader::Uninit();
}

//============================================================
//	�Q�ƒl�̐ݒ菈��
//============================================================
void CStencilShader::SetRefValue(const int nRefValue)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�ɎQ�ƒl��ݒ�
	GetEffect()->SetInt(m_pRefValue, nRefValue);
}

//============================================================
//	��r���Z�q�̐ݒ菈��
//============================================================
void CStencilShader::SetComparison(const EComparison comparison)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�ɔ�r���Z�q��ݒ�
	GetEffect()->SetInt(m_pComparison, comparison);
}

//============================================================
//	���l����̐ݒ菈��
//============================================================
void CStencilShader::SetOperation(const EOperation operation)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�ɐ��l�����ݒ�
	GetEffect()->SetInt(m_pOperation, operation);
}

//============================================================
//	��������
//============================================================
CStencilShader *CStencilShader::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �X�e���V���V�F�[�_�[�̐���
	m_pShader = new CStencilShader;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�e���V���V�F�[�_�[�̏�����
		if (FAILED(m_pShader->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�e���V���V�F�[�_�[�̔j��
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
CStencilShader *CStencilShader::GetInstance(void)
{
	// �C���X�^���X���g�p
	assert(m_pShader != nullptr);

	// �X�e���V���V�F�[�_�[�̃|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CStencilShader::Release(void)
{
	// �X�e���V���V�F�[�_�[�̏I��
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// �������J��
	SAFE_DELETE(m_pShader);
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�|�C���^)
//============================================================
void CStencilShader::SetTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`����ݒ�
	if (pTexture != nullptr)
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

		// �G�t�F�N�g�ɃX�e���V���e�N�X�`����ݒ�
		GetEffect()->SetTexture(m_pTextureStencil, pTexture);
	}
	else { assert(false); }	// �w��Ȃ�
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�C���f�b�N�X)
//============================================================
void CStencilShader::SetTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`����ݒ�
	if (nTextureID != NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă���ꍇ

		if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p
		CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`�����

		// �G�t�F�N�g�ɃX�e���V���e�N�X�`����ݒ�
		GetEffect()->SetTexture(m_pTextureStencil, pTexture->GetTexture(nTextureID));
	}
	else { assert(false); }	// �w��Ȃ�
}
