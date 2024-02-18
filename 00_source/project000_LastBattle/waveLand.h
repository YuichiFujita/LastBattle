//============================================================
//
//	���n�g���w�b�_�[ [waveLand.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WAVE_LAND_H_
#define _WAVE_LAND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wave.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���n�g���N���X
class CWaveLand : public CWave
{
public:
	// �R���X�g���N�^
	CWaveLand();

	// �f�X�g���N�^
	~CWaveLand() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CWaveLand *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXCOLOR& rCol,		// �F
		const SGrow& rGrow,			// ������
		const SGrow& rAddGrow,		// ����������
		const float fHoleRadius,	// ���̔��a
		const float fThickness,		// ����
		const float fOuterPlusY,	// �O����Y���W���Z��
		const float fMaxGrowRadius	// ���a�̍ő听����
	);

	void SetAddGrow(const SGrow &rGrow)		{ m_addGrow = rGrow; }			// �������ݒ�
	SGrow GetAddGrow(void) const			{ return m_addGrow; }			// �������擾
	void SetMaxGrowRadius(const float fMax)	{ m_fMaxGrowRadius = fMax; }	// �������ݒ�
	float GetMaxGrowRadius(void) const		{ return m_fMaxGrowRadius; }	// �������擾

private:
	// �����o�ϐ�
	float m_fMaxGrowRadius;	// ���a�̍ő听����
	SGrow m_addGrow;		// ����������
};

#endif	// _WAVE_LAND_H_
