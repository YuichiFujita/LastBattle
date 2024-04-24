//============================================================
//
//	���g���}�l�[�W���[�w�b�_�[ [swordWaveManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SWORDWAVE_MANAGER_H_
#define _SWORDWAVE_MANAGER_H_

//************************************************************
//	�N���X��`
//************************************************************
// ���g���}�l�[�W���[�N���X
class CSwordWaveManager
{
public:
	// �R���X�g���N�^
	CSwordWaveManager();

	// �f�X�g���N�^
	~CSwordWaveManager();

	// �g�������^�C�~���O�\����
	struct STiming
	{
		D3DXVECTOR3 posOffset;	// �����ʒu�I�t�Z�b�g
		D3DXVECTOR3 rotOffset;	// ���������I�t�Z�b�g
		int nMotion;	// �������[�V����
		int nFrame;		// �����t���[��
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	void AddTiming	// �����^�C�~���O�ǉ�
	( // ����
		const D3DXVECTOR3& rPosOffset,	// �����ʒu�I�t�Z�b�g
		const D3DXVECTOR3& rRotOffset,	// ���������I�t�Z�b�g
		const int nMotion,	// �������[�V����
		const int nFrame	// �����t���[��
	);
	void AddTiming(const STiming& rTiming);	// �����^�C�~���O�ǉ�

	// �ÓI�����o�֐�
	static CSwordWaveManager *Create(void);	// ����
	static void Release(CSwordWaveManager *&prSwordWaveManager);	// �j��

private:
	// �����o�ϐ�
	std::vector<STiming> m_spawnTiming;	// �����^�C�~���O���
};

#endif	// _SWORDWAVE_MANAGER_H_
