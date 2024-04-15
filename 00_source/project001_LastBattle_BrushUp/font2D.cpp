//============================================================
//
//	�t�H���g2D���� [font2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "font2D.h"
#include "manager.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float REV_SCALE = 1.0f;	// �傫���̕␳�W��
}

//************************************************************
//	�q�N���X [CFont2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFont2D::CFont2D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject2D(label, dimension, nPriority),
	m_fScale	(0.0f)	// �g�嗦
{
	// �����o�ϐ����N���A
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// �������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFont2D::~CFont2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFont2D::Init(void)
{
	// �����o�ϐ���������
	memset(&m_infoChar, 0, sizeof(m_infoChar));	// �������
	m_fScale = 1.0f;	// �g�嗦

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// TODO
	SetPriority(7);
	SetLabel(CObject::LABEL_UI);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFont2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFont2D::Update(void)
{
	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CFont2D::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CFont2D *CFont2D::Create
(
	CFontChar *pFontChar,		// �t�H���g�������
	const UINT uChar,			// �w�蕶��
	const D3DXVECTOR3& rPos,	// �ʒu
	const float fScale,			// �g�嗦
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
)
{
	// �t�H���g2D�̐���
	CFont2D *pFont2D = new CFont2D;
	if (pFont2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�H���g2D�̏�����
		if (FAILED(pFont2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�H���g2D�̔j��
			SAFE_DELETE(pFont2D);
			return nullptr;
		}

		// �t�H���g�E������ݒ�
		pFont2D->SetFontChar(pFontChar, uChar);

		// �ʒu��ݒ�
		pFont2D->SetVec3Position(rPos);

		// ������ݒ�
		pFont2D->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pFont2D->SetScale(fScale);

		// �F��ݒ�
		pFont2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pFont2D;
	}
}

//============================================================
//	�t�H���g�E�����̐ݒ菈��
//============================================================
void CFont2D::SetFontChar
(
	CFontChar *pFontChar,	// �t�H���g�������
	const UINT uChar		// �w�蕶��
)
{
	if (uChar == L'o')
	{
		int a = 0;
	}

	// �t�H���g�����̎擾
	m_infoChar = pFontChar->Regist(uChar);

	// �e�N�X�`�����쐬�E����
	BindTexture(m_infoChar.nTexID);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CFont2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 pos = rPos;
	//float fBaseLine = (float)m_infoChar.text.tmHeight - (float)m_infoChar.text.tmAscent;
	//float fLineDown = (float)m_infoChar.glyph.gmptGlyphOrigin.y - (float)m_infoChar.glyph.gmBlackBoxY;
	//float fDown = /*fBaseLine*/ - fLineDown;

	//static int n = 0;
	//if (n < 16)
	{

		// TODO�F�ʒu�������̂ǂ��ɂ�
		//pos.x -= ((float)m_infoChar.glyph.gmptGlyphOrigin.x * REV_SCALE);
		//pos.y += ((float)m_infoChar.text.tmDescent - (float)m_infoChar.glyph.gmptGlyphOrigin.y) * REV_SCALE * 0.5f;

		// �ʒu�̐ݒ�
		CObject2D::SetVec3Position(pos);

		//n++;
	}
	//else
	//{
	//	CObject2D::SetVec3Position(pos);

	//}
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CFont2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̐ݒ�
	CObject2D::SetVec3Rotation(rRot);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CFont2D::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	// �傫���̐ݒ�֐��͔䗦���ς���Ă��܂��̂Ŏg�p�ł��Ȃ�
	// �傫���ύX�̍ۂ͓��N���X�� SetHegiht �𗘗p���Ă�������
	assert(false);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CFont2D::SetColor(const D3DXCOLOR& rCol)
{
	// �F�̐ݒ�
	CObject2D::SetColor(rCol);
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CFont2D::SetScale(const float fScale)
{
	// �e�N�X�`���X�e�[�^�X���
	D3DXIMAGE_INFO status = GET_MANAGER->GetTexture()->GetInfo(GetTextureIndex()).status;

	// �����̊g�嗦��ݒ�
	m_fScale = fScale;

	// �傫����ݒ�
	CObject2D::SetVec3Sizing(D3DXVECTOR3((float)status.Width, (float)status.Height, 0.0f) * fScale * REV_SCALE);
}
