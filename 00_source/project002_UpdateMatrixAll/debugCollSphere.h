//============================================================
//
//	���̔���f�o�b�O�\���w�b�_�[ [debugCollSphere.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _DEBUG_COLLSPHERE_H_
#define _DEBUG_COLLSPHERE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshSphere.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace debugCollSphere
{
	const D3DXVECTOR3 ROT		= VEC3_ZERO;	// ����
	const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f);	// �F
	const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f);	// �F

	const POSGRID2	PART	= POSGRID2(8, 4);	// ������
	const POSGRID2	TEXPART	= POSGRID2(1, 1);	// �e�N�X�`��������
}

//************************************************************
//	�N���X��`
//************************************************************
// ���̔���f�o�b�O�\���N���X
class CDebugCollSphere : public CObjectMeshSphere
{
public:
	// �R���X�g���N�^
	CDebugCollSphere();

	// �f�X�g���N�^
	~CDebugCollSphere() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;		// ������
	void Uninit(void) override;			// �I��
	void Update(void) override;			// �X�V
	void UpdateMatrix(void) override;	// �}�g���b�N�X�X�V
	void Draw(void) override;			// �`��

	// �ÓI�����o�֐�
	static CDebugCollSphere *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const float fRadius,		// ���a
		const D3DXCOLOR& rCol		= debugCollSphere::DEFAULT_COL,	// �F
		const D3DXVECTOR3& rRot		= debugCollSphere::ROT,			// ����
		const POSGRID2& rPart		= debugCollSphere::PART,		// ������
		const POSGRID2& rTexPart	= debugCollSphere::TEXPART		// �e�N�X�`��������
	);
};

#endif	// _DEBUG_COLLSPHERE_H_
