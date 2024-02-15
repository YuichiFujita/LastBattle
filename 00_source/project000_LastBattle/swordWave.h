//============================================================
//
//	���g���w�b�_�[ [swordWave.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SWORD_WAVE_H_
#define _SWORD_WAVE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wave.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���g���N���X
class CSwordWave : public CWave
{
public:
	// �R���X�g���N�^
	CSwordWave();

	// �f�X�g���N�^
	~CSwordWave() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CSwordWave *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const SGrow& rGrow,			// ������
		const SGrow& rAddGrow,		// ����������
		const float fHoleRadius,	// ���̔��a
		const float fThickness		// ����
	);

	void SetAddGrow(const SGrow& rGrow)	{ m_addGrow = rGrow; }	// �������ݒ�
	SGrow GetAddGrow(void) const		{ return m_addGrow; }	// �������擾

private:
	// �����o�֐�
	void CollisionEnemy(void);	// �G�Ƃ̓����蔻��

	// �����o�ϐ�
	SGrow m_addGrow;	// ����������
};

#endif	// _SWORD_WAVE_H_
