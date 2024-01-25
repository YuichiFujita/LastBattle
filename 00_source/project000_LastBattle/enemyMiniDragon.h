//============================================================
//
//	�~�j�h���S���w�b�_�[ [enemyMiniDragon.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_MINI_DRAGON_H_
#define _ENEMY_MINI_DRAGON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"

//************************************************************
//	�N���X��`
//************************************************************
// �~�j�h���S���N���X
class CEnemyMiniDragon : public CEnemy
{
public:
	// ���f����
	enum EModel
	{
		MODEL_NORMAL = 0,	// �ʏ�
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CEnemyMiniDragon(const EType type);

	// �f�X�g���N�^
	~CEnemyMiniDragon() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

private:
	// �I�[�o�[���C�h�֐�
	const char *GetModelFileName(const int nModel) const override;	// ���f���t�@�C���擾
	void UpdateMotion(void) override;	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
};

#endif	// _ENEMY_MINI_DRAGON_H_
