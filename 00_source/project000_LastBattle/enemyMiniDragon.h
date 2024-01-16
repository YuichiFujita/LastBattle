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
	// ��ޗ�
	enum EModel
	{
		MODEL_NORMAL = 0,	// �ʏ�
		MODEL_MAX			// ���̗񋓌^�̑���
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
};

#endif	// _ENEMY_MINI_DRAGON_H_
