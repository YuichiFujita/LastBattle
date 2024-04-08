//============================================================
//
//	�t�H���g2D�w�b�_�[ [font2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FONT2D_H_
#define _FONT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�H���g2D�N���X
class CFont2D : public CObject2D
{
public:
	// �R���X�g���N�^
	explicit CFont2D(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_2D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CFont2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	void SetHeight(const float fHeight) override;			// �c���ݒ�

	// �ÓI�����o�֐�
	static CFont2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize = VEC3_ONE,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

private:
	// �����o�ϐ�
	float m_fHeight;	// �c��
};

#endif	// _FONT2D_H_
