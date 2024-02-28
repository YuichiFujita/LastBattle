//============================================================
//
//	�v���C���[�^�C�g���w�b�_�[ [playerTitle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_TITLE_H_
#define _PLAYER_TITLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j���[�V����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�^�C�g���N���X
class CPlayerTitle : public CPlayer
{
public:
	// ���o��
	enum EStag
	{
		STAG_NONE = 0,	// �������Ȃ�
		STAG_FALL,		// ����
		STAG_LAND,		// ���n
		STAG_IDOL,		// �ҋ@
		STAG_ATTACK,	// �U��
		STAG_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayerTitle();

	// �f�X�g���N�^
	~CPlayerTitle() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;						// �`��
	void SetStag(const int nStag) override	{ m_stag = (EStag)nStag; }	// ���o��Ԑݒ�
	int GetStag(void) const override		{ return m_stag; }			// ���o��Ԏ擾

private:
	// �����o�֐�
	void UpdateStagFall(void);		// ����
	void UpdateStagLand(void);		// ���n
	void UpdateStagIdol(void);		// �ҋ@
	void UpdateStagAttack(void);	// �U��

	// �����o�ϐ�
	CAnim2D *m_pSlash;	// �a���̏��
	EStag m_stag;		// ���o���
};

#endif	// _PLAYER_TITLE_H_
