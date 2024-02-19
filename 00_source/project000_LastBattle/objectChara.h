//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CCollSphere;	// �~����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�L�����N�^�[�N���X
class CObjectChara : public CObject
{
public:
	// �R���X�g���N�^
	explicit CObjectChara(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = object::DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectChara() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;			// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetAllMaterial(const D3DXMATERIAL& rMat) override;	// �}�e���A���S�ݒ�
	void ResetMaterial(void) override;						// �}�e���A���Đݒ�
	D3DXMATRIX GetMtxWorld(void) const override;			// �}�g���b�N�X�擾
	D3DXMATRIX *GetPtrMtxWorld(void) override;				// �}�g���b�N�X�|�C���^�擾

	// �ÓI�����o�֐�
	static CObjectChara *Create(const D3DXVECTOR3 &rPos, const D3DXVECTOR3 &rRot = VEC3_ZERO);	// ����

	// ���z�֐�
	virtual void SetMotion(const int nType, const int nBlendFrame = 0);	// ���[�V�����ݒ�

	// �����o�֐�
	void SetPartsInfo	// �p�[�c���ݒ�
	( // ����
		const int nID,				// �p�[�c�C���f�b�N�X
		const int nParentID,		// �e�C���f�b�N�X
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const char *pFileName		// �t�@�C����
	);

	void SetModelInfo(void);						// ���f�����ݒ�
	void SetMotionInfo(CMotion::SMotionInfo info);	// ���[�V�������ݒ�
	void SetMtxWorld(const D3DXMATRIX &rMtxWorld);	// �}�g���b�N�X�ݒ�
	void SetEnableMotionUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�

	void SetPartsPosition(const int nPartsID, const D3DXVECTOR3& rPos);	// �p�[�c�ʒu�ݒ�
	D3DXVECTOR3 GetPartsPosition(const int nPartsID) const;				// �p�[�c�ʒu�擾
	void SetPartsRotation(const int nPartsID, const D3DXVECTOR3& rRot);	// �p�[�c�����ݒ�
	D3DXVECTOR3 GetPartsRotation(const int nPartsID) const;				// �p�[�c�����擾

	CMultiModel *GetMultiModel(const int nPartsID) const;	// �}���`���f���擾
	CCollSphere *GetCollision(const int nPartsID) const;	// �����蔻��擾
	CMotion *GetMotion(void) const;							// ���[�V�����擾

	void SetMaterial(const D3DXMATERIAL& rMat, const int nPartsID, const int nMatID);	// �}�e���A���ݒ�
	void SetAlpha(const float fAlpha);		// �����x�ݒ�
	float GetAlpha(void) const;				// �����x�擾
	float GetMaxAlpha(void) const;			// �ő哧���x�擾
	int GetNumModel(void) const;			// �p�[�c�����擾
	int GetMotionType(void) const;			// ���[�V������ގ擾
	int GetMotionNumType(void) const;		// ���[�V������ޑ����擾
	int GetMotionKey(void) const;			// ���[�V�����L�[�ԍ��擾
	int GetMotionNumKey(void) const;		// ���[�V�����L�[�����擾
	int GetMotionKeyCounter(void) const;	// ���[�V�����L�[�J�E���^�[�擾
	int GetMotionWholeCounter(void) const;	// ���[�V�����S�̃J�E���^�[�擾
	int GetMotionWholeFrame(void) const;	// ���[�V�����S�̃t���[�����擾
	int GetMotionCancelFrame(void) const;	// ���[�V�����L�����Z���t���[���擾
	int GetMotionComboFrame(void) const;	// ���[�V�����R���{�t���[���擾
	bool IsMotionFinish(void) const;		// ���[�V�����I���擾
	bool IsMotionLoop(void) const;			// ���[�V�������[�v�擾
	bool IsMotionCancel(void) const;		// ���[�V�����L�����Z���擾
	bool IsMotionCombo(void) const;			// ���[�V�����R���{�擾
	bool IsWeaponDisp(void) const;			// ���[�V��������\���擾
	bool IsLeftWeaponCollision(void);		// ���̍U������t���O�擾
	bool IsRightWeaponCollision(void);		// �E�̍U������t���O�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[motion::MAX_PARTS];	// ���f���̏��
	CCollSphere	*m_apCollision[motion::MAX_PARTS];	// �~����N���X
	CMotion		*m_pMotion;		// ���[�V�����̏��
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	int			m_nNumModel;	// �p�[�c�̑���
};

#endif	// _OBJECTCHARA_H_
