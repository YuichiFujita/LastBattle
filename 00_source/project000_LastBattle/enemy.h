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
//	�N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObjectModel
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_MINI_DRAGON = 0,	// �~�j�h���S��
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_SPAWN = 0,	// �X�|�[�����
		STATE_NORMAL,		// �ʏ���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemy(const EType type);

	// �f�X�g���N�^
	virtual ~CEnemy();

	// �X�e�[�^�X�\����
	struct SStatusInfo
	{
		float fRadius;	// ���a
		float fHeight;	// �c��
		float fLookRev;	// ���F�̕␳�W��
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

	// �ÓI�����o�֐�
	static CEnemy *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static SStatusInfo GetStatusInfo(const int nType);	// �X�e�[�^�X���擾

	// �����o�֐�
	void UpdateOldPosition(void);					// �ߋ��ʒu�X�V
	D3DXVECTOR3 GetOldPosition(void) const;			// �ߋ��ʒu�擾
	void SetMovePosition(const D3DXVECTOR3& rMove);	// �ʒu�ړ��ʐݒ�
	D3DXVECTOR3 GetMovePosition(void) const;		// �ʒu�ړ��ʎ擾
	void SetMoveRotation(const D3DXVECTOR3& rMove);	// �����ύX�ʐݒ�
	D3DXVECTOR3 GetMoveRotation(void) const;		// �����ύX�ʎ擾
	SStatusInfo GetStatusInfo(void) const;			// �X�e�[�^�X���擾

protected:
	// ���z�֐�
	virtual void UpdateSpawn(void);		// �X�|�[����Ԏ��̍X�V
	virtual void UpdateNormal(void);	// �ʏ��Ԏ��̍X�V

	// �����o�֐�
	void UpdateLook	// �Ώێ��F
	( // ����
		const D3DXVECTOR3 &rPosLook,	// ���F�Ώۈʒu
		const D3DXVECTOR3 &rPosEnemy,	// �G�ʒu
		D3DXVECTOR3 *pRotEnemy			// �G����
	);

private:
	// �����o�֐�
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	// �ÓI�����o�ϐ�
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���

	// �����o�ϐ�
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_movePos;		// �ʒu�ړ���
	D3DXVECTOR3	m_moveRot;		// �����ړ���
	EState	m_state;			// ���
	int		m_nCounterState;	// ��ԊǗ��J�E���^�[
	bool	m_bJump;			// �W�����v��
	const EType m_type;			// ��ޒ萔
	const SStatusInfo m_status;	// �X�e�[�^�X�萔
};

#endif	// _ENEMY_H_
