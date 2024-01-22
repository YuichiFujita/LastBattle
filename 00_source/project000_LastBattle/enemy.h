//============================================================
//
//	�G�w�b�_�[ [enemy.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"

//************************************************************
//	�O���錾
//************************************************************
class CGauge3D;	// �Q�[�W3D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObjectModel
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_BOSS_DRAGON = 0,	// �{�X�h���S��
		TYPE_MINI_DRAGON,		// �~�j�h���S��
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_SPAWN = 0,	// �X�|�[�����
		STATE_NORMAL,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_INVULN,		// ���G���
		STATE_DEATH,		// ���S���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemy(const EType type);

	// �f�X�g���N�^
	virtual ~CEnemy();

	// �X�e�[�^�X�\����
	struct SStatusInfo
	{
		int   nMaxLife;		// �ő�̗�
		float fRadius;		// ���a
		float fHeight;		// �c��
		float fLookRev;		// ���F�̕␳�W��
		float fCollRadius;	// ���蔼�a
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetState(const int nState) override;	// ��Ԑݒ�
	int GetState(void) const override;			// ��Ԏ擾
	int GetType(void) const override;			// ��ގ擾
	float GetRadius(void) const override;		// ���a�擾
	float GetHeight(void) const override;		// �c���擾

	// ���z�֐�
	virtual void Hit(const int nDamage);	// �q�b�g
	virtual void HitKnockBack(const int nDamage, const D3DXVECTOR3 &vecKnock);	// �m�b�N�o�b�N�q�b�g

	// �ÓI�����o�֐�
	static CEnemy *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static CListManager<CEnemy> *GetList(void);			// ���X�g�擾
	static SStatusInfo GetStatusInfo(const int nType);	// �X�e�[�^�X���擾

	// �����o�֐�
	void UpdateOldPosition(void);					// �ߋ��ʒu�X�V
	D3DXVECTOR3 GetOldPosition(void) const;			// �ߋ��ʒu�擾
	void SetMovePosition(const D3DXVECTOR3& rMove);	// �ʒu�ړ��ʐݒ�
	D3DXVECTOR3 GetMovePosition(void) const;		// �ʒu�ړ��ʎ擾
	SStatusInfo GetStatusInfo(void) const;			// �X�e�[�^�X���擾
	int GetLife(void) const;						// �̗͎擾

protected:
	// ���z�֐�
	virtual void SetSpawn(void);		// �X�|�[����Ԃ̐ݒ�
	virtual void SetInvuln(void);		// ���G��Ԃ̐ݒ�
	virtual void UpdateSpawn(void);		// �X�|�[����Ԏ��̍X�V
	virtual void UpdateNormal(void);	// �ʏ��Ԏ��̍X�V
	virtual void UpdateDamage(void);	// �_���[�W��Ԏ��̍X�V
	virtual void UpdateInvuln(void);	// ���G��Ԏ��̍X�V
	virtual void UpdateDeath(void);		// ���S��Ԏ��̍X�V

	// �����o�֐�
	void UpdateLook	// �Ώێ��F
	( // ����
		const D3DXVECTOR3 &rPosLook,	// ���F�Ώۈʒu
		const D3DXVECTOR3 &rPosEnemy,	// �G�ʒu
		D3DXVECTOR3 *pRotEnemy			// �G����
	);
	void UpdateGravity(void);				// �d�͂̍X�V
	bool UpdateLanding(D3DXVECTOR3 *pPos);	// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3 *pPos);	// �ʒu�̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

private:
	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CEnemy::*AFuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy> *m_pList;			// �I�u�W�F�N�g���X�g
	static SStatusInfo m_aStatusInfo[TYPE_MAX];		// �X�e�[�^�X���
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator;	// �C�e���[�^�[
	CGauge3D	*m_pLife;		// �̗͂̏��
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_move;			// �ړ���
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[
	float	m_fSinAlpha;		// ��������
	bool	m_bJump;			// �W�����v��
	const EType m_type;			// ��ޒ萔
	const SStatusInfo m_status;	// �X�e�[�^�X�萔
};

#endif	// _ENEMY_H_
