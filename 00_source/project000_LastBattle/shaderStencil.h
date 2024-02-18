//============================================================
//
//	�X�e���V���V�F�[�_�[�w�b�_�[ [shaderStencil.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHADER_STENCIL_H_
#define _SHADER_STENCIL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shader.h"

//************************************************************
//	�N���X��`
//************************************************************
// �X�e���V���V�F�[�_�[�N���X
class CStencilShader : public CShader
{
public:
	// ��r���Z�q��
	enum EComparison
	{
		COMPARISON_NEVER = 0,		// �K�����s
		COMPARISON_LESS,			//  <�Ő���
		COMPARISON_LESSEQUAL,		// <=�Ő���
		COMPARISON_EQUAL,			// ==�Ő���
		COMPARISON_NOTEQUAL,		// !=�Ő���
		COMPARISON_GREATER,			//  >�Ő���
		COMPARISON_GREATEREQUAL,	// >=�Ő���
		COMPARISON_ALWAYS,			// �K������
		COMPARISON_MAX				// ���̗񋓌^�̑���
	};

	// ���l�����
	enum EOperation
	{
		OPERATION_REPLACE = 0,	// �Q�ƒl�ɒu��
		OPERATION_INCRSAT,		// �C���N�������g
		OPERATION_DECRSAT,		// �f�N�������g
	};

	// �R���X�g���N�^
	CStencilShader();

	// �f�X�g���N�^
	~CStencilShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	// �����o�֐�
	void SetRefValue(const int nRefValue);				// �Q�ƒl�̐ݒ�
	void SetComparison(const EComparison comparison);	// ��r���Z�q�̐ݒ�
	void SetOperation(const EOperation operation);		// ���l����̐ݒ�

	// �ÓI�����o�֐�
	static CStencilShader *Create(void);		// ����
	static CStencilShader *GetInstance(void);	// �擾
	static void Release(void);					// �j��

private:
	// �����o�֐�
	void SetTexture(const LPDIRECT3DTEXTURE9 pTexture);	// �e�N�X�`���ݒ� (�|�C���^)
	void SetTexture(const int nTextureID);				// �e�N�X�`���ݒ� (�C���f�b�N�X)

	// �ÓI�����o�ϐ�
	static CStencilShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_pTextureStencil;	// �X�e���V���e�N�X�`��
	D3DXHANDLE m_pRefValue;			// �Q�ƒl
	D3DXHANDLE m_pComparison;		// ��r���Z�q
	D3DXHANDLE m_pOperation;		// ���l����
};

#endif	// _SHADER_H_
