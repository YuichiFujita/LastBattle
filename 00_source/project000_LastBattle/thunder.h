//============================================================
//
//	���w�b�_�[ [thunder.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _THUNDER_H_
#define _THUNDER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace thunder
{
	const int NUM_ORBIT = 2;	// �O�Ղ̐�����
}

//************************************************************
//	�O���錾
//************************************************************
class COrbit;	// �O�ՃN���X

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CThunder : public CObject
{
public:
	// �R���X�g���N�^
	CThunder();

	// �f�X�g���N�^
	~CThunder() override;

	// ���O�Ղ̍\����
	struct SOrbitInfo
	{
		COrbit *pOrbit;			// �O�Ղ̏��
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CThunder *Create(const D3DXVECTOR3& rPos);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��
	void SetVec3Position(const D3DXVECTOR3 &rPos) override;	// �ʒu�ݒ�

	// �����o�ϐ�
	SOrbitInfo  m_aOrbit[thunder::NUM_ORBIT];	// �O�Ղ̏��
	D3DXVECTOR3 m_posOrbit;	// �O�Ղ̐����ʒu
};

#endif	// _THUNDER_H_
