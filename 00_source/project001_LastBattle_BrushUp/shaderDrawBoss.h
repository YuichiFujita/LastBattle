//============================================================
//
//	�{�X�`��V�F�[�_�[�w�b�_�[ [shaderDrawBoss.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADER_DRAWBOSS_H_
#define _SHADER_DRAWBOSS_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"

//************************************************************
//	�N���X��`
//************************************************************
// �{�X�`��V�F�[�_�[�N���X
class CDrawBossShader : public CShader
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_TOON = 0,	// �g�D�[���}�b�v�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CDrawBossShader();

	// �f�X�g���N�^
	~CDrawBossShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	void SetLightDirect(D3DXMATRIX *pMtxWorld, const int nLightID) override;	// ���C�g�����x�N�g���ݒ�
	void SetTexture(const LPDIRECT3DTEXTURE9 *pTexture) override;	// �e�N�X�`���ݒ� (�|�C���^)
	void SetTexture(const int nTextureID) override;					// �e�N�X�`���ݒ� (�C���f�b�N�X)
	void SetMaterial(const D3DMATERIAL9& rMaterial) override;		// �}�e���A���ݒ�
	void SetDiffuse(const D3DXCOLOR& rDiffuse) override;			// �g�U���ݒ�
	void SetAmbient(const D3DXCOLOR& rAmbient) override;			// �����ݒ�
	void SetEmissive(const D3DXCOLOR& rEmissive) override;			// ���ˌ��ݒ�
	void SetOnlyDiffuse(const D3DXCOLOR& rDiffuse) override;		// �g�U���̂ݐݒ�

	// �ÓI�����o�֐�
	static CDrawBossShader *Create(void);		// ����
	static CDrawBossShader *GetInstance(void);	// �擾
	static void Release(void);					// �j��

private:
	// �����o�֐�
	void SetToonMapTexture(const ETexture texture);	// �g�D�[���}�b�v�e�N�X�`���ݒ�
	void SetCropTexture(const int nTextureID);		// �؂蔲���e�N�X�`���ݒ�

	// �ÓI�����o�ϐ�
	static CDrawBossShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_pTextureToon;	// �g�D�[���}�b�v�e�N�X�`��
	D3DXHANDLE m_pTextureCrop;	// �؂蔲���e�N�X�`��
	D3DXHANDLE m_pDirectLight;	// ���s�����̕����x�N�g��
	D3DXHANDLE m_pDiffuse;		// �g�U��
	D3DXHANDLE m_pAmbient;		// ����
	D3DXHANDLE m_pEmissive;		// ���ˌ�
	D3DXHANDLE m_pUseTexture;	// �e�N�X�`���̎g�p��
};

#endif	// _SHADER_DRAWBOSS_H_
