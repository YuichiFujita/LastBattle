//============================================================
//
//	�I�u�W�F�N�g�����L�����N�^�[�w�b�_�[ [objectDivChara.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_DIVCHARA_H_
#define _OBJECT_DIVCHARA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectChara;	// �I�u�W�F�N�g�L�����N�^�[�N���X
class CMultiModel;	// �}���`���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�����L�����N�^�[�N���X
class CObjectDivChara : public CObject
{
public:
	// �g�̗�
	enum EBody
	{
		BODY_LOWER = 0,	// �����g
		BODY_UPPER,		// �㔼�g
		BODY_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CObjectDivChara(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectDivChara() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3 &rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetAllMaterial(const D3DXMATERIAL &rMat) override;	// �}�e���A���S�ݒ�
	void SetEnableUpdate(const bool bUpdate) override;		// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;			// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CObjectDivChara *Create	// ����
	( // ����
		const D3DXVECTOR3 &rPos,				// �ʒu
		const D3DXVECTOR3 &rRot = VEC3_ZERO,	// ����
		const int nUpperParentID = 0			// �㔼�g�e�C���f�b�N�X
	);

	// �����o�֐�
	void SetUpperParentID(const int nUpperParentID);		// �㔼�g�̐e�C���f�b�N�X�ݒ�
	CObjectChara *GetObjectChara(const EBody bodyID) const;	// �I�u�W�F�N�g�L�����N�^�[�擾
	CMultiModel *GetMultiModel(const EBody bodyID, const int nModelID) const;	// �}���`���f���擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	CObjectChara *m_apBody[BODY_MAX];	// �g�̂̏��
	int m_nUpperParentID;	// �㔼�g�e�C���f�b�N�X
};

#endif	// _OBJECT_DIVCHARA_H_
