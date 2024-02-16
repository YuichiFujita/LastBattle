//============================================================
//
//	������ [fire.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fire.h"
#include "collision.h"
#include "player.h"
#include "particle3D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	DMG_FIRE	= 15;		// ���̃_���[�W��
	const float COLL_RADIUS	= 34.0f;	// ����̔��a
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CFire> *CFire::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CFire] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFire::CFire() : CObject(CObject::LABEL_FIRE, DIM_3D),
	m_pos	(VEC3_ZERO),	// �ʒu
	m_move	(VEC3_ZERO),	// �ړ���
	m_nLife	(0)				// ����

{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFire::~CFire()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFire::Init(void)
{
	// �����o�ϐ����N���A
	m_pos	= VEC3_ZERO;	// �ʒu
	m_move	= VEC3_ZERO;	// �ړ���
	m_nLife = 0;			// ����

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CFire>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFire::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DeleteList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// ���I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CFire::Update(void)
{
	// �ʒu���ړ�
	m_pos += m_move;

	// ���p�[�e�B�N���𐶐�
	CParticle3D::Create(CParticle3D::TYPE_FIRE, m_pos);

	// �v���C���[�Ƃ̓����蔻��
	CollisionPlayer();

	if (m_nLife > NONE_IDX)
	{ // �����Ǘ�������Ă���ꍇ

		// ���������炷
		m_nLife--;
		if (m_nLife <= 0)
		{ // �������Ȃ��Ȃ����ꍇ

			// ���g�̏I��
			Uninit();
			return;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CFire::Draw(void)
{

}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CFire::SetVec3Position(const D3DXVECTOR3 &rPos)
{
	// �ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CFire::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	��������
//============================================================
CFire *CFire::Create
(
	const D3DXVECTOR3& rPos,	// �����ʒu
	const int nLife				// ����
)
{
	// ���̐���
	CFire *pFire = new CFire;
	if (pFire == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���̏�����
		if (FAILED(pFire->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���̔j��
			SAFE_DELETE(pFire);
			return nullptr;
		}

		// �ʒu��ݒ�
		pFire->SetVec3Position(rPos);

		// ������ݒ�
		pFire->SetLife(nLife);

		// �m�ۂ����A�h���X��Ԃ�
		return pFire;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CFire> *CFire::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�j������
//============================================================
void CFire::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
void CFire::CollisionPlayer(void)
{
	CListManager<CPlayer> *pList = CPlayer::GetList();		// �v���C���[���X�g
	if (pList == nullptr)		 { assert(false); return; }	// ���X�g���g�p
	if (pList->GetNumAll() != 1) { assert(false); return; }	// �v���C���[��1�l����Ȃ�
	auto player = pList->GetList().front();					// �v���C���[���

	D3DXVECTOR3 posPlayer	 = player->GetVec3Position();	// �v���C���[�̒��S�ʒu
				posPlayer.y += player->GetHeight() * 0.5f;	// �����̔�����ɂ�����
	float fRadiusPlayer		 = player->GetRadius();			// �v���C���[�̔��a

	if (collision::Circle3D(m_pos, posPlayer, COLL_RADIUS, fRadiusPlayer))
	{ // �Ռ��g�ɓ������Ă���ꍇ

		// �v���C���[�̃q�b�g����
		player->Hit(DMG_FIRE);
	}
}
