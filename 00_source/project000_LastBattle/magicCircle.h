//============================================================
//
//	���@�w�w�b�_�[ [magicCircle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MAGIC_CIRCLE_H_
#define _MAGIC_CIRCLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCircle.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectMeshCylinder;	// �I�u�W�F�N�g���b�V���V�����_�[�N���X
class CObjectMeshRing;		// �I�u�W�F�N�g���b�V�������O�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���@�w�N���X
class CMagicCircle : public CObjectMeshCircle
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// �ʏ�
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CMagicCircle();

	// �f�X�g���N�^
	~CMagicCircle() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3 &rRot) override;	// �����ݒ�
	void SetRadius(const float fRadius) override;			// ���a�ݒ�

	// �ÓI�����o�֐�
	static CMagicCircle *Create		// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const float fRadius,		// ���a
		const float fAlphaRadius	// �������a
	);
	static CListManager<CMagicCircle> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void DrawCrop(void);	// �؂蔲���p�̕`��
	void SetAlphaRadius(const float fRadius);	// �������a�ݒ�

private:
	// �ÓI�����o�ϐ�
	static CListManager<CMagicCircle> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CMagicCircle>::AIterator m_iterator;	// �C�e���[�^�[
	CObjectMeshCylinder *m_pAlphaCylinder;	// ���@�w�̐�̓������
	CObjectMeshRing *m_pAlphaRing;			// ���@�w�̋󔒂̓������
};

#endif	// _MAGIC_CIRCLE_H_
