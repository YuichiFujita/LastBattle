#if 1
//============================================================
//
//	�I�u�W�F�N�g�X�L�����b�V��3D�w�b�_�[ [objectSkinMesh3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_SKINMESH3D_H_
#define _OBJECT_SKINMESH3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�X�L�����b�V��3D�N���X
class CObjectSkinMesh3D : public CObject
{
public:
	// �R���X�g���N�^
	explicit CObjectSkinMesh3D(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectSkinMesh3D() override;

	// �{�[�����\����
	struct SBone
	{
		D3DXVECTOR3 pos;	// �{�[���ʒu
		D3DXVECTOR3 rot;	// �{�[������
		int nParentID;		// �e�{�[��
	};

	// ���_�ǉ����
	struct SVtx
	{
		int aBoneRef[2];	// �{�[���Q�ƒl
		float aWeight[2];	// �{�[���e���x
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾

	// �ÓI�����o�֐�
	static CObjectSkinMesh3D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO	// ����
	);

	// �����o�֐�
	HRESULT CreateBone(const int nNumBone);		// �{�[������
	HRESULT CreateVertex(const int nNumVtx);	// �������ݒ�

	void SetBoneInfo	// �{�[�����ݒ�
	( // ����
		const int nID,				// �p�[�c�C���f�b�N�X
		const int nParentID,		// �e�C���f�b�N�X
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	void SetVertexInfo	// ���_���ݒ�
	( // ����
		const int nVtxID,	// ���_�C���f�b�N�X
		const SVtx& rVtx,	// ���_�ǉ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR2& rTex		// �e�N�X�`�����W
	);

	CRenderState *GetRenderState(void);	// �����_�[�X�e�[�g���擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void DrawNormal(void);				// �ʏ�`��
	void DrawShader(CShader *pShader);	// �V�F�[�_�[�`��
	void LoadSetup(void);				// �Z�b�g�A�b�v

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	CRenderState *m_pRenderState;		// �����_�[�X�e�[�g�̏��
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	D3DXVECTOR3	m_size;			// �傫��
	D3DXCOLOR	m_col;			// �F
	int			m_nTextureID;	// �e�N�X�`���C���f�b�N�X

	SBone *m_pBone;		// �{�[�����
	int m_nNumBone;		// �{�[����
	SVtx *m_pVtxPlus;	// ���_�ǉ����
	int m_nNumVtx;		// ���_��
};

#endif	// _OBJECT_SKINMESH3D_H_
#endif