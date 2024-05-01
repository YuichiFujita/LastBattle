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
	CFontChar(const HFONT hFont);

	// �f�X�g���N�^
	~CFontChar();

	// ��`�̍��W���
	struct SRectangle
	{
		POSGRID2 lu;	// ������W
		POSGRID2 rd;	// �E�����W
	};

	// �t�H���g�����\����
	struct SChar
	{
		// �R���X�g���N�^
		SChar() { memset(this, 0, sizeof(*this)); }

		GLYPHMETRICS glyph;			// �t�H���g�O���t�̏��
		OUTLINETEXTMETRIC outline;	// �t�H���g�A�E�g���C���̏��
		TEXTMETRIC text;			// �t�H���g�e�L�X�g�̏��
		SRectangle offsetBlackBox;	// �u���b�N�{�b�N�X�̒��S����̃I�t�Z�b�g
		int nTexID;					// �e�N�X�`���C���f�b�N�X
	};

	// �����o�֐�
	HRESULT Init(void);	// �t�H���g����������
	void Uninit(void);	// �t�H���g�����I��
	SChar Regist(const wchar_t wcChar);	// �t�H���g�����o�^

	// �ÓI�����o�֐�
	static CFontChar *Create(const HFONT hFont);	// ����
	static void Release(CFontChar *&pFontChar);		// �j��

private:
	// �����o�֐�
	BYTE *CreateBitMap(SChar *pChar, HDC hDC, const wchar_t wcChar);	// �r�b�g�}�b�v�����E�������ۑ�
	HRESULT CreateTexture(SChar *pChar, BYTE *pBitMap);					// �e�N�X�`�������E�쐬

	// �����o�ϐ�
	std::map<wchar_t, SChar> m_mapChar;	// �t�H���g�����A�z�z��
	const HFONT m_hFont;	// �t�H���g�ւ̃|�C���^
};

#endif	// _FONT_CHAR_H_
