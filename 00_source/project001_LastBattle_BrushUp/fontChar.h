//============================================================
//
//	�t�H���g�����w�b�_�[ [fontChar.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FONT_CHAR_H_
#define _FONT_CHAR_H_

//************************************************************
//	�N���X��`
//************************************************************
// �t�H���g�����N���X
class CFontChar
{
public:
	// �R���X�g���N�^
	CFontChar(const HFONT pFont);

	// �f�X�g���N�^
	~CFontChar();

	// �t�H���g�����\����
	struct SChar
	{
		// �R���X�g���N�^
		SChar() { memset(this, 0, sizeof(*this)); }

		GLYPHMETRICS glyph;			// �t�H���g�O���t�̏��
		OUTLINETEXTMETRIC outline;	// �t�H���g�A�E�g���C���̏��
		TEXTMETRIC text;			// �t�H���g�e�L�X�g�̏��
		int nTexID;					// �t�H���g�e�N�X�`���C���f�b�N�X
	};

	// �����o�֐�
	HRESULT Init(void);	// �t�H���g����������
	void Uninit(void);	// �t�H���g�����I��
	SChar Regist(const UINT uChar);	// �t�H���g�����o�^

	// �ÓI�����o�֐�
	static CFontChar *Create(const HFONT pFont);	// ����
	static void Release(CFontChar *&pFontChar);		// �j��

private:
	// �����o�֐�
	BYTE *CreateBitMap(SChar *pChar, HDC pDC, const UINT uChar);	// �r�b�g�}�b�v�����E�������ۑ�
	HRESULT CreateTexture(SChar *pChar, BYTE *pBitMap);				// �e�N�X�`�������E�쐬

	// �����o�ϐ�
	std::map<UINT, SChar> m_mapChar;	// �t�H���g�����A�z�z��
	const HFONT m_pFont;	// �t�H���g�ւ̃|�C���^
};

#endif	// _FONT_CHAR_H_
