//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"

//************************************************************
//	�O���錾
//************************************************************
class CModelFont;	// ���f�������N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_START,	// �J�n���
		STATE_NORMAL,	// �ʏ���
		STATE_STAGING,	// ���o���
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �J�n��ԗ�
	enum EStartState
	{
		START_PLAYER = 0,	// �v���C���[�X�|�[�����
		START_BOSS,			// �{�X�X�|�[�����
		START_END,			// �I�����
		START_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetState(const EState state);		// ��Ԑݒ�
	EState GetState(void) const;			// ��Ԏ擾
	void SetDrawGameUI(const bool bDraw);	// �Q�[����ʂ�UI�`��ݒ�
	void TransitionResult(const CRetentionManager::EWin win);	// ���U���g��ʑJ��

	// �ÓI�����o�֐�
	static CGameManager *Create(void);	// ����
	static void Release(CGameManager *&prGameManager);	// �j��

private:
	// �����o�֐�
	void UpdateStart(void);	// �J�n��Ԃ̍X�V
	void EndStart(void);	// �J�n���o�̏I��
	void SkipStart(void);	// �J�n���o�̃X�L�b�v

	// �����o�ϐ�
	CModelFont *m_pBossName;	// �{�X�̖��O���f�����
	EState		m_state;		// ���
	EStartState	m_startState;	// �J�n���
};

#endif	// _GAMEMANAGER_H_
