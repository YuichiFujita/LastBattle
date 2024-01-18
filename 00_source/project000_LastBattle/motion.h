//============================================================
//
//	���[�V�����w�b�_�[ [motion.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	�萔�錾
//************************************************************
namespace motion
{
	const int MAX_PARTS		= 24;	// �p�[�c�̍ő吔
	const int MAX_MOTION	= 16;	// ���[�V�����̍ő吔
	const int MAX_KEY		= 16;	// �L�[�̍ő吔
}

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���[�V�����N���X
class CMotion
{
public:
	// �R���X�g���N�^
	CMotion();

	// �f�X�g���N�^
	~CMotion();

	// �p�[�c�Ǘ��\����
	struct SKey
	{
		D3DXVECTOR3 pos;	// ���f���ʒu
		D3DXVECTOR3 rot;	// ���f������
	};

	// �|�[�Y�Ǘ��\����
	struct SKeyInfo
	{
		SKey aKey[motion::MAX_PARTS];	// �L�[���f�����
		int  nFrame;	// �L�[�Đ��t���[����
	};

	// �L�[�Ǘ��\����
	struct SMotionInfo
	{
		SKeyInfo aKeyInfo[motion::MAX_KEY];	// �L�[���
		int  nLeftMinColl;	// ���U������̊J�n�J�E���g
		int  nLeftMaxColl;	// ���U������̏I���J�E���g
		int  nRightMinColl;	// �E�U������̊J�n�J�E���g
		int  nRightMaxColl;	// �E�U������̏I���J�E���g
		int  nNumKey;		// �L�[����
		bool bLoop;			// ���[�v ON/OFF
		bool bWeaponDisp;	// ����\�� ON/OFF
	};

	// ���[�V�����Ǘ��\����
	struct SInfo
	{
		SMotionInfo aMotionInfo[motion::MAX_MOTION];	// ���[�V�������
		int  nNumMotion;	// ���[�V��������
		int  nType;			// ���[�V�������
		int  nPose;			// ���[�V�����|�[�Y�ԍ�
		int  nKeyCounter;	// ���[�V�����L�[�J�E���^�[
		int  nWholeCounter;	// ���[�V�����S�̃J�E���^�[
		bool bFinish;		// ���[�V�����I����
	};

	// �����o�֐�
	HRESULT Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Set(const int nType);	// �ݒ�
	void SetInfo(const SMotionInfo info);					// ���[�V�������ݒ�
	void SetEnableUpdate(const bool bUpdate);				// �X�V�󋵐ݒ�
	void SetModel(CMultiModel **ppModel, const int nNum);	// ���f�����ݒ�
	int  GetType(void) const;					// ��ގ擾
	int  GetPose(void) const;					// �|�[�Y�ԍ��擾
	int  GetKeyCounter(void) const;				// ���[�V�����L�[�J�E���^�[�擾
	int  GetWholeCounter(void) const;			// ���[�V�����S�̃J�E���^�[�擾
	bool IsFinish(void) const;					// �I���擾
	bool IsLoop(const int nType) const;			// ���[�v�擾
	bool IsWeaponDisp(const int nType) const;	// ����\���擾
	bool IsLeftWeaponCollision(void);			// ���̍U������t���O�擾
	bool IsRightWeaponCollision(void);			// �E�̍U������t���O�擾

	// �ÓI�����o�֐�
	static CMotion *Create(void);				// ����
	static void Release(CMotion *&prMotion);	// �j��

private:
	// �����o�ϐ�
	SInfo m_info;				// ���[�V�������
	CMultiModel **m_ppModel;	// ���f�����
	int  m_nNumModel;			// ���f���̃p�[�c��
	bool m_bUpdate;				// �X�V��
};

#endif	// _MOTION_H_
