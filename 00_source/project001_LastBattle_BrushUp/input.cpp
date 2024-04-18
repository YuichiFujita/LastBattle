//============================================================
//
//	���͏��� [input.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "input.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	namespace mouse
	{
		const int MAX_KEY = 4;	// �}�E�X�L�[�̍ő吔
	}

	namespace vibration
	{
		const int TIME			= 20;	// �o�C�u�̌p������
		const int TIME_DEATH	= 100;	// ���S���̃o�C�u�p������

		const WORD LEVEL_DAMAGE		= (WORD)(USHRT_MAX * 0.6f);	// �_���[�W���̐U�����x��
		const WORD LEVEL_BIG_DAMAGE	= (WORD)(USHRT_MAX * 0.8f);	// ��_���[�W���̐U�����x��
		const WORD LEVEL_DEATH		= (WORD)(USHRT_MAX);		// ���S���̐U�����x��
		const WORD LEVEL_WALLDASH	= (WORD)(USHRT_MAX * 0.6f);	// �Ǒ���̐U�����x��
		const WORD LEVEL_FLAILCHAGE	= (WORD)(USHRT_MAX * 0.4f);	// �t���C�����߂̐U�����x��
		const WORD LEVEL_FLAILSHOT	= (WORD)(USHRT_MAX * 1.0f);	// �t���C�������̐U�����x��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInput�I�u�W�F�N�g
int CInput::m_nNumAll = 0;	// ���̓f�o�C�X�̑���

//************************************************************
//	�e�N���X [CInput] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInput::CInput() :
	m_pDevice(nullptr)	// DirectInput�f�o�C�X
{
	// ���̓f�o�C�X�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInput::~CInput()
{
	// ���̓f�o�C�X�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND /*hWnd*/)
{
	// �����o�ϐ���������
	m_pDevice = nullptr;	// DirectInput�f�o�C�X

	if (m_pInput == nullptr)
	{ // ���̓f�o�C�X���g�p����Ă��Ȃ��ꍇ

		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInput::Uninit(void)
{
	// DirectInput�f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{ // DirectInput�f�o�C�X���g�p���̏ꍇ

		// �A�N�Z�X���J��
		m_pDevice->Unacquire();

		// �������J��
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_nNumAll <= 1)
	{ // �Ō��DirectInput�f�o�C�X���j�����ꂽ�ꍇ

		// DirectInput�I�u�W�F�N�g�̔j��
		SAFE_RELEASE(m_pInput);
	}
}

//************************************************************
//	�q�N���X [CInputKeyboard] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputKeyboard::CInputKeyboard()
{
	// �����o�ϐ����N���A
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// �}�E�X�̃v���X���
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// �}�E�X�̃g���K�[���
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// �}�E�X�̃����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �����o�ϐ���������
	memset(&m_aKeyStatePress[0],   0, sizeof(m_aKeyStatePress));	// �}�E�X�̃v���X���
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));	// �}�E�X�̃g���K�[���
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));	// �}�E�X�̃����[�X���

	// ���͂̏�����
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// DirectInput�f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputKeyboard::Uninit(void)
{
	// ���͂̏I��
	CInput::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CInputKeyboard::Update(void)
{
	// �ϐ��z���錾
	BYTE aKeyState[keyboard::MAX_KEY];	// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // �擾�ɐ��������ꍇ

		for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
		{ // �L�[�̍ő吔���J��Ԃ�

			// �L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �L�[�{�[�h�ւ̃A�N�Z�X�����ēx�l��
		m_pDevice->Acquire();
	}
}

//============================================================
//	�v���X�擾����
//============================================================
bool CInputKeyboard::IsPress(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//============================================================
//	�g���K�[�擾����
//============================================================
bool CInputKeyboard::IsTrigger(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//============================================================
//	�����[�X�擾����
//============================================================
bool CInputKeyboard::IsRelease(int nKey)
{
	// ���͏���Ԃ�
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//============================================================
//	�S�v���X�擾����
//============================================================
bool CInputKeyboard::IsAnyPress(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // �L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStatePress[nCntKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�g���K�[�擾����
//============================================================
bool CInputKeyboard::IsAnyTrigger(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // �L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStateTrigger[nCntKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�����[�X�擾����
//============================================================
bool CInputKeyboard::IsAnyRelease(void)
{
	for (int nCntKey = 0; nCntKey < keyboard::MAX_KEY; nCntKey++)
	{ // �L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStateRelease[nCntKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	��������
//============================================================
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// �L�[�{�[�h�̐���
	CInputKeyboard *pKeyboard = new CInputKeyboard;
	if (pKeyboard == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �L�[�{�[�h�̏�����
		if (FAILED(pKeyboard->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �L�[�{�[�h�̔j��
			SAFE_DELETE(pKeyboard);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pKeyboard;
	}
}

//============================================================
//	�j������
//============================================================
void CInputKeyboard::Release(CInputKeyboard *&prKeyboard)
{
	// �L�[�{�[�h�̏I��
	assert(prKeyboard != nullptr);
	prKeyboard->Uninit();

	// �������J��
	SAFE_DELETE(prKeyboard);
}

//************************************************************
//	�q�N���X [CInputMouse] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputMouse::CInputMouse()
{
	// �����o�ϐ����N���A
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// �v���X���
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// �g���K�[���
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// �����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputMouse::~CInputMouse()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ���錾
	DIPROPDWORD diprop;	// �f�o�C�X�̐ݒ�p

	// �����o�ϐ���������
	memset(&m_keyStatePress,   0, sizeof(m_keyStatePress));		// �v���X���
	memset(&m_keyStateTrigger, 0, sizeof(m_keyStateTrigger));	// �g���K�[���
	memset(&m_keyStateRelease, 0, sizeof(m_keyStateRelease));	// �����[�X���

	// ���͂̏�����
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// DirectInput�f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{ // �ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{ // �f�o�C�X�̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputMouse::Uninit(void)
{
	// ���͂̏I��
	CInput::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CInputMouse::Update(void)
{
	// �ϐ���錾
	DIMOUSESTATE keyState;	// �}�E�X�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(keyState), &keyState)))
	{ // �擾�ɐ��������ꍇ

		for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
		{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

			// �}�E�X�̃����[�X����ۑ�
			m_keyStateRelease.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] | keyState.rgbButtons[nCntMouKey]) ^ keyState.rgbButtons[nCntMouKey];

			// �}�E�X�̃g���K�[����ۑ�
			m_keyStateTrigger.rgbButtons[nCntMouKey] = (m_keyStatePress.rgbButtons[nCntMouKey] ^ keyState.rgbButtons[nCntMouKey]) & keyState.rgbButtons[nCntMouKey];
		}

		// �}�E�X�̃v���X����ۑ�
		m_keyStatePress = keyState;
	}
	else
	{ // �擾�Ɏ��s�����ꍇ

		// �}�E�X�ւ̃A�N�Z�X�����ēx�l��
		m_pDevice->Acquire();
	}
}

//============================================================
//	�v���X�擾����
//============================================================
bool CInputMouse::IsPress(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStatePress.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�g���K�[�擾����
//============================================================
bool CInputMouse::IsTrigger(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStateTrigger.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�����[�X�擾����
//============================================================
bool CInputMouse::IsRelease(EKey mouKey)
{
	// ���͏���Ԃ�
	return (m_keyStateRelease.rgbButtons[mouKey] & 0x80) ? true : false;
}

//============================================================
//	�S�v���X�擾����
//============================================================
bool CInputMouse::IsAnyPress(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

		if (m_keyStatePress.rgbButtons[nCntMouKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�g���K�[�擾����
//============================================================
bool CInputMouse::IsAnyTrigger(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

		if (m_keyStateTrigger.rgbButtons[nCntMouKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�����[�X�擾����
//============================================================
bool CInputMouse::IsAnyRelease(void)
{
	for (int nCntMouKey = 0; nCntMouKey < mouse::MAX_KEY; nCntMouKey++)
	{ // �}�E�X�L�[�̍ő吔���J��Ԃ�

		if (m_keyStateRelease.rgbButtons[nCntMouKey] & 0x80)
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	// �}�E�X�̈ړ��ʂ�Ԃ�
	return D3DXVECTOR3
	(
		(float)m_keyStatePress.lX,	// ���ړ���
		(float)m_keyStatePress.lY,	// �c�ړ���
		(float)m_keyStatePress.lZ	// �z�C�[���ړ���
	);
}

//============================================================
//	��������
//============================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// �}�E�X�̐���
	CInputMouse *pMouse = new CInputMouse;
	if (pMouse == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �}�E�X�̏�����
		if (FAILED(pMouse->Init(hInstance, hWnd)))
		{ // �������Ɏ��s�����ꍇ

			// �}�E�X�̔j��
			SAFE_DELETE(pMouse);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMouse;
	}
}

//============================================================
//	�j������
//============================================================
void CInputMouse::Release(CInputMouse *&prMouse)
{
	// �}�E�X�̏I��
	assert(prMouse != nullptr);
	prMouse->Uninit();

	// �������J��
	SAFE_DELETE(prMouse);
}

//************************************************************
//	�e�N���X [CInputPad] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CInputPad::CInputPad()
{
	// �����o�ϐ����N���A
	memset(&m_aVibration[0],		0, sizeof(m_aVibration));		// �o�C�u���
	memset(&m_aKeyStatePress[0],	0, sizeof(m_aKeyStatePress));	// �v���X���
	memset(&m_aKeyStateTrigger[0],	0, sizeof(m_aKeyStateTrigger));	// �g���K�[���
	memset(&m_aKeyStateRelease[0],	0, sizeof(m_aKeyStateRelease));	// �����[�X���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CInputPad::~CInputPad()
{

}

//============================================================
//	����������
//============================================================
HRESULT CInputPad::Init(void)
{
	// �����o�ϐ���������
	memset(&m_aVibration[0],		0, sizeof(m_aVibration));		// �o�C�u���
	memset(&m_aKeyStatePress[0],	0, sizeof(m_aKeyStatePress));	// �v���X���
	memset(&m_aKeyStateTrigger[0],	0, sizeof(m_aKeyStateTrigger));	// �g���K�[���
	memset(&m_aKeyStateRelease[0],	0, sizeof(m_aKeyStateRelease));	// �����[�X���

	// XInput�̃X�e�[�g��L����
	XInputEnable(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CInputPad::Uninit(void)
{
	// XInput�̃X�e�[�g�𖳌���
	XInputEnable(false);
}

//============================================================
//	�X�V����
//============================================================
void CInputPad::Update(void)
{
	// �ϐ��z���錾
	XINPUT_STATE aKeyState[pad::MAX_NUM];	// �p�b�h�̓��͏��

	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (XInputGetState(nCntJoyKey, &aKeyState[nCntJoyKey]) == ERROR_SUCCESS)
		{ // �R���g���[���[���ڑ�����Ă���ꍇ

			// �p�b�h�̃����[�X����ۑ� (�{�^��)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons | aKeyState[nCntJoyKey].Gamepad.wButtons) ^ aKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃����[�X����ۑ� (L2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger | aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// �p�b�h�̃����[�X����ۑ� (R2)
			m_aKeyStateRelease[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger | aKeyState[nCntJoyKey].Gamepad.bRightTrigger) ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// �p�b�h�̃g���K�[����ۑ� (�{�^��)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons ^ aKeyState[nCntJoyKey].Gamepad.wButtons) & aKeyState[nCntJoyKey].Gamepad.wButtons;

			// �p�b�h�̃g���K�[����ۑ� (L2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bLeftTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bLeftTrigger ^ aKeyState[nCntJoyKey].Gamepad.bLeftTrigger) & aKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// �p�b�h�̃g���K�[����ۑ� (R2)
			m_aKeyStateTrigger[nCntJoyKey].Gamepad.bRightTrigger =
			(m_aKeyStatePress[nCntJoyKey].Gamepad.bRightTrigger ^ aKeyState[nCntJoyKey].Gamepad.bRightTrigger) & aKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// �p�b�h�̃v���X����ۑ�
			m_aKeyStatePress[nCntJoyKey] = aKeyState[nCntJoyKey];

			// �o�C�u���[�V�����̍X�V
			UpdateVibration(&m_aVibration[nCntJoyKey], nCntJoyKey);
		}
	}
}

//============================================================
//	�o�C�u���[�V�����̍X�V����
//============================================================
void CInputPad::UpdateVibration(SVibration *pVibration, int nPadID)
{
	switch (pVibration->type)
	{ // �U���̍X�V
	case TYPE_NONE:		// �������Ă��Ȃ�

		// �����𔲂���
		break;

	case TYPE_DEATH:	// ���S

		if (pVibration->nTime > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			pVibration->nTime--;

			// �U�����x���̐ݒ�
			pVibration->vibration.wLeftMotorSpeed  -= (WORD)(USHRT_MAX / vibration::TIME_DEATH);	// ��
			pVibration->vibration.wRightMotorSpeed -= (WORD)(USHRT_MAX / vibration::TIME_DEATH);	// �E
		}
		else
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �������Ȃ���Ԃɐݒ�
			pVibration->type = TYPE_NONE;

			// �������̏�����
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// �����𔲂���
		break;

	default:	// ��L�ȊO

		if (pVibration->nTime >= 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			pVibration->nTime--;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �������Ȃ���Ԃɐݒ�
			pVibration->type = TYPE_NONE;

			// �������̏�����
			memset(&pVibration->vibration, 0, sizeof(pVibration->vibration));
		}

		// �����𔲂���
		break;
	}

	// ���[�^�[�̕␳
	if (pVibration->vibration.wLeftMotorSpeed < 0)
	{ // ���̐U���� 0��菬�����ꍇ

		// ���̃��[�^�[�̕␳
		pVibration->vibration.wLeftMotorSpeed = 0;
	}
	if (pVibration->vibration.wRightMotorSpeed < 0)
	{ // �E�̐U���� 0��菬�����ꍇ

		// �E�̃��[�^�[�̕␳
		pVibration->vibration.wRightMotorSpeed = 0;
	}

	// �o�C�u����ݒ�
	XInputSetState(nPadID, &pVibration->vibration);
}

//============================================================
//	�o�C�u�ݒ菈��
//============================================================
void CInputPad::SetVibration(EType type, int nPadID)
{
	// �o�C�u�̏�Ԃ������̂��̂ɐݒ�
	m_aVibration[nPadID].type = type;

	switch (m_aVibration[nPadID].type)
	{ // �o�C�u�̎��
	case TYPE_DAMAGE:	// �_���[�W���

		// �U�����x���̐ݒ�
		m_aVibration[nPadID].vibration.wLeftMotorSpeed  = vibration::LEVEL_DAMAGE;	// ��
		m_aVibration[nPadID].vibration.wRightMotorSpeed = vibration::LEVEL_DAMAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPadID].nTime = vibration::TIME;

		// �����𔲂���
		break;

	case TYPE_BIG_DAMAGE:	// ��_���[�W���

		// �U�����x���̐ݒ�
		m_aVibration[nPadID].vibration.wLeftMotorSpeed  = vibration::LEVEL_BIG_DAMAGE;	// ��
		m_aVibration[nPadID].vibration.wRightMotorSpeed = vibration::LEVEL_BIG_DAMAGE;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPadID].nTime = vibration::TIME;

		// �����𔲂���
		break;

	case TYPE_DEATH:	// ���S���

		// �U�����x���̐ݒ�
		m_aVibration[nPadID].vibration.wLeftMotorSpeed  = vibration::LEVEL_DEATH;	// ��
		m_aVibration[nPadID].vibration.wRightMotorSpeed = vibration::LEVEL_DEATH;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPadID].nTime = vibration::TIME_DEATH;

		// �����𔲂���
		break;

	case TYPE_WALLDASH:	// �Ǒ�����

		// �U�����x���̐ݒ�
		m_aVibration[nPadID].vibration.wLeftMotorSpeed  = vibration::LEVEL_WALLDASH;	// ��
		m_aVibration[nPadID].vibration.wRightMotorSpeed = vibration::LEVEL_WALLDASH;	// �E

		// �U�����Ԃ̐ݒ�
		m_aVibration[nPadID].nTime = 0;

		// �����𔲂���
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �o�C�u����ݒ�
	XInputSetState(nPadID, &m_aVibration[nPadID].vibration);
}

//============================================================
//	�v���X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsPress(EKey joyKey, int nPadID)
{
	// ���͏���Ԃ�
	return (m_aKeyStatePress[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�g���K�[�擾���� (�{�^��)
//============================================================
bool CInputPad::IsTrigger(EKey joyKey, int nPadID)
{
	// ���͏���Ԃ�
	return (m_aKeyStateTrigger[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�����[�X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsRelease(EKey joyKey, int nPadID)
{
	// ���͏���Ԃ�
	return (m_aKeyStateRelease[nPadID].Gamepad.wButtons & (1 << joyKey)) ? true : false;
}

//============================================================
//	�S�v���X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsAnyPress(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // �p�b�h�L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStatePress[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�g���K�[�擾���� (�{�^��)
//============================================================
bool CInputPad::IsAnyTrigger(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // �p�b�h�L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStateTrigger[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�S�����[�X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsAnyRelease(int nPadID)
{
	for (int nCntPadKey = 0; nCntPadKey < KEY_MAX; nCntPadKey++)
	{ // �p�b�h�L�[�̍ő吔���J��Ԃ�

		if (m_aKeyStateRelease[nPadID].Gamepad.wButtons & (1 << nCntPadKey))
		{ // ���͂��������ꍇ

			return true;
		}
	}

	return false;
}

//============================================================
//	�v���X���������� (�{�^��)
//============================================================
void CInputPad::InitPress(int nPadID)
{
	// ���������N���A
	memset(&m_aKeyStatePress[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStatePress[nPadID].Gamepad.wButtons));
}

//============================================================
//	�g���K�[���������� (�{�^��)
//============================================================
void CInputPad::InitTrigger(int nPadID)
{
	// ���������N���A
	memset(&m_aKeyStateTrigger[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStateTrigger[nPadID].Gamepad.wButtons));
}

//============================================================
//	�����[�X���������� (�{�^��)
//============================================================
void CInputPad::InitRelease(int nPadID)
{
	// ���������N���A
	memset(&m_aKeyStateRelease[nPadID].Gamepad.wButtons, 0, sizeof(m_aKeyStateRelease[nPadID].Gamepad.wButtons));
}

//============================================================
//	�S�v���X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsPressAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStatePress[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�S�g���K�[�擾���� (�{�^��)
//============================================================
bool CInputPad::IsTriggerAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStateTrigger[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�S�����[�X�擾���� (�{�^��)
//============================================================
bool CInputPad::IsReleaseAll(EKey joyKey)
{
	for (int nCntJoyKey = 0; nCntJoyKey < pad::MAX_NUM; nCntJoyKey++)
	{ // �v���C���[�̍ő�l�������[�v

		if (m_aKeyStateRelease[nCntJoyKey].Gamepad.wButtons & (1 << joyKey))
		{ // ���݂̃p�b�h�œ��͂��m�F���ꂽ�ꍇ

			// ���͏�񂠂��Ԃ�
			return true;
		}
	}

	// ���͏�񖳂���Ԃ�
	return false;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�NX)
//============================================================
SHORT CInputPad::GetPressLStickX(int nPadID)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbLX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (���E)
	return m_aKeyStatePress[nPadID].Gamepad.sThumbLX;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�NY)
//============================================================
SHORT CInputPad::GetPressLStickY(int nPadID)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbLY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return -m_aKeyStatePress[nPadID].Gamepad.sThumbLY;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�NX)
//============================================================
SHORT CInputPad::GetPressRStickX(int nPadID)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbRX, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (���E)
	return m_aKeyStatePress[nPadID].Gamepad.sThumbRX;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�NY)
//============================================================
SHORT CInputPad::GetPressRStickY(int nPadID)
{
	// �X�e�B�b�N�̌X���̒l��␳
	useful::LimitNum(m_aKeyStatePress[nPadID].Gamepad.sThumbRY, (SHORT)-SHRT_MAX, (SHORT)SHRT_MAX);

	// �X�e�B�b�N�̌X���̒l��Ԃ� (�㉺)
	return -m_aKeyStatePress[nPadID].Gamepad.sThumbRY;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�N����)
//============================================================
float CInputPad::GetPressLStickRot(int nPadID)
{
	// �ϐ���錾
	float fRot = 0.0f;	// �X�e�B�b�N�X��

	// �X�e�b�N�̌X�����v�Z
	fRot = atan2f((float)GetPressLStickY(nPadID), (float)GetPressLStickX(nPadID));

	// �X�e�b�N�̌X����Ԃ�
	return fRot;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�N����)
//============================================================
float CInputPad::GetPressRStickRot(int nPadID)
{
	// �ϐ���錾
	float fRot = 0.0f;	// �X�e�B�b�N�X��

	// �X�e�b�N�̌X�����v�Z
	fRot = atan2f((float)GetPressRStickY(nPadID), (float)GetPressRStickX(nPadID));

	// �X�e�b�N�̌X����Ԃ�
	return fRot;
}

//============================================================
//	�v���X�擾���� (L�X�e�B�b�N�X��)
//============================================================
float CInputPad::GetPressLStickTilt(int nPadID)
{
	// �ϐ���錾
	D3DXVECTOR3	vecLTilt = D3DXVECTOR3((float)GetPressLStickX(nPadID), (float)GetPressLStickY(nPadID), 0.0f);	// L�X�e�B�b�N�e���̌X����
	float fLTilt = sqrtf(vecLTilt.x * vecLTilt.x + vecLTilt.y * vecLTilt.y) * 0.5f;	// L�X�e�B�b�N�̌X����

	// L�X�e�B�b�N�̌X����Ԃ�
	return fLTilt;
}

//============================================================
//	�v���X�擾���� (R�X�e�B�b�N�X��)
//============================================================
float CInputPad::GetPressRStickTilt(int nPadID)
{
	// �ϐ���錾
	D3DXVECTOR3	vecRTilt = D3DXVECTOR3((float)GetPressRStickX(nPadID), (float)GetPressRStickY(nPadID), 0.0f);	// R�X�e�B�b�N�e���̌X����
	float fRTilt = sqrtf(vecRTilt.x * vecRTilt.x + vecRTilt.y * vecRTilt.y) * 0.5f;	// R�X�e�B�b�N�̌X����

	// R�X�e�B�b�N�̌X����Ԃ�
	return fRTilt;
}

//============================================================
//	��������
//============================================================
CInputPad *CInputPad::Create(void)
{
	// �p�b�h�̐���
	CInputPad *pPad = new CInputPad;
	if (pPad == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �p�b�h�̏�����
		if (FAILED(pPad->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �p�b�h�̔j��
			SAFE_DELETE(pPad);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPad;
	}
}

//============================================================
//	�j������
//============================================================
void CInputPad::Release(CInputPad *&prPad)
{
	// �p�b�h�̏I��
	assert(prPad != nullptr);
	prPad->Uninit();

	// �������J��
	SAFE_DELETE(prPad);
}