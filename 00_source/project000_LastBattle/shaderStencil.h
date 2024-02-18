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
	// �R���X�g���N�^
	CStencilShader();

	// �f�X�g���N�^
	~CStencilShader() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��

	// �����o�֐�
	void SetColor(const D3DXCOLOR& rCol);	// �s�N�Z���`��F�̐ݒ�

	// �ÓI�����o�֐�
	static CStencilShader *Create(void);		// ����
	static CStencilShader *GetInstance(void);	// �擾
	static void Release(void);					// �j��

private:
	// �ÓI�����o�ϐ�
	static CStencilShader *m_pShader;	// �V�F�[�_�[���

	// �����o�ϐ�
	D3DXHANDLE m_pColDraw;	// �s�N�Z���`��F
};

#endif	// _SHADER_H_
