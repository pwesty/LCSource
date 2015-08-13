#ifndef SE_INCL_INPUT_H
#define SE_INCL_INPUT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Templates/StaticArray.cpp>


// number of key ids reserved (in KeyNames.h)
#define KID_TOTALCOUNT 256


#define MAX_JOYSTICKS 8

#define CT_MOUSE_AXIS    4
#define CT_MOUSE_SCROLER 2
#define CT_MOUSE_BUTTONS 5
#define CT_SECOND_MOUSE_AXIS    4
#define CT_SECOND_MOUSE_BUTTONS 3
#define CT_JOYSTICK_AXIS        12
#define CT_JOYSTICK_BUTTONS     32
#define CT_JOYSTICK_POV_BUTTONS 4 


#define CT_KEYBOARD_CONTROLS      256
#define CT_MOUSE_CONTROLS         CT_MOUSE_AXIS+CT_MOUSE_SCROLER+CT_MOUSE_BUTTONS
#define CT_SECOND_MOUSE_CONTROLS  CT_SECOND_MOUSE_AXIS+CT_SECOND_MOUSE_BUTTONS
#define CT_JOYSTICK_CONTROLS      CT_JOYSTICK_AXIS+CT_JOYSTICK_BUTTONS+CT_JOYSTICK_POV_BUTTONS


#define FIRST_MOUSE_SCROLER       CT_MOUSE_AXIS
#define FIRST_MOUSE_BUTTON        CT_MOUSE_AXIS+CT_MOUSE_SCROLER
#define FIRST_SECOND_MOUSE_BUTTON CT_SECOND_MOUSE_AXIS
#define FIRST_JOYSTICK_BUTTON     CT_JOYSTICK_AXIS
#define FIRST_POV_CONTROL         CT_JOYSTICK_CONTROLS-CT_JOYSTICK_POV_BUTTONS


#define MAX_DEVICES 12

#define DEVICE_NONE               0
#define KEYBOARD_DEVICE           1
#define MOUSE_DEVICE              2
#define SECOND_MOUSE_DEVICE       3
#define FIRST_JOYSTICK_DEVICE     4

enum DeviceType
{
  DT_NONE = 0,
  DT_KEYBOARD,
  DT_MOUSE,
  DT_SECOND_MOUSE,
  DT_JOYSTICK,
};

struct InputControl
{
  InputControl() {
    ic_bExisting = TRUE;
    ic_fValue    = 0.0f;
    ic_slMin     = 0;
    ic_slMax     = 0;
  };
  ~InputControl() {};
  BOOL     ic_bExisting;
  FLOAT    ic_fValue;
  SLONG    ic_slMin;
  SLONG    ic_slMax;
  CTString ic_strControlName;
  CTString ic_strControlNameTrans;
};

class ENGINE_API CInputDevice
{
public:
  CInputDevice();
  ~CInputDevice();
  // Init device
  BOOL InitDevice(DeviceType dtDeviceType);
  // Does joystick has pov
  BOOL HasJoystickPOV(void);
  // Get number of controls in device
  INDEX GetControlsCount(void);
  // Get control by control name
  INDEX GetControl(const CTString &strControl);
  // Get value from given control
  FLOAT GetControlValue(INDEX iControl);
  // Set all control values to 0
  void ClearControlsValue(void);
  // Enable input device
  void EnableDevice(void);
  // Disable input device
  void DisableDevice(void);
  // Enable given control
  void EnableControl(INDEX iControl);
  // Disable given control
  void DisableControl(INDEX iControl);
  // Returns device type
  inline DeviceType GetDeviceType(void) { return id_dtDeviceType;}
public:
  enum DeviceType id_dtDeviceType;  // device type
  BOOL  id_bDeviceEnabled;          // is device enabled
  INDEX id_iDeviceIndex;            // index of device
  CTString id_strDeviceName;        // device name
  CTString id_strDeviceShortName;   // short name of device (used in menus)
  CStaticArray<struct InputControl> id_aicControls; // array of input controls for this devices
};

/*
 *  Mouse speed control structure
 */
struct MouseSpeedControl
{
  int msc_iThresholdX;
  int msc_iThresholdY;
  int msc_iSpeed;
};

/*
 * Class responsible for dealing with DirectInput
 */
class ENGINE_API CInput {
public:
// Operations
  CInput();
  ~CInput();
  // Initializes all available devices and enumerates available controls
  void Initialize(void);
  // Enable input inside one viewport, or window
  void EnableInput(CViewPort *pvp);
  void EnableInput(HWND hWnd);
  // Disable input
  void DisableInput(void);
  // enable/disable joystick polling (it can be slow to poll if user doesn't realy use the joystick)
  void SetJoyPolling(BOOL bPoll);
  // Test input activity
  BOOL IsInputEnabled( void) const { return inp_bInputEnabled; };
  // Scan states of all available input sources
  void GetInput(BOOL bPreScan);

  // Add new device
  BOOL AddDevice(DeviceType dtDeviceType, INDEX iDeviceIndex);
  // Remove device
  BOOL RemoveDevice(CInputDevice &idDevice);
  // Set name for all controls in all devices
  void SetControlNames(void);
  // Get reference to device by device index
  CInputDevice *GetInputDevice(INDEX iDevice);
  // Get reference to device by device name
  CInputDevice *GetInputDevice(const CTString &strDevice);
  // Get control global id from device name and control name
  ULONG GetControlGIDFromString(const CTString &strDevice, const CTString &strControl);

  // Clear all input states (keys become not pressed, axes are reset to zero)
  void ClearInput( void);
  // Get input devices count
  INDEX GetInputDevicesCount(void) { return inp_aInputDevices.Count();}
  // Get value of control
  FLOAT GetControlValue(ULONG ulControlGID);
//0105
  void SetControlValue(ULONG ulControlGID);
//..
  // Get name of control
  CTString GetControlName(ULONG ulControlGID);
  // Get translated name of control
  CTString GetControlTransName(ULONG ulControlGID);
  // Get name of input device
  CTString GetInputDeviceName(INDEX iDevice);
  // Get translated name of input device
  CTString GetInputDeviceShortName(INDEX iDevice);

  BOOL IsQKeyPressed(){return m_bQKey;}
  BOOL IsVKeyPressed(){return m_bVKey;}
  BOOL IsIKeyPressed(){return m_bIKey;}
  BOOL IsJKeyPressed(){return m_bJKey;}
  BOOL IsKKeyPressed(){return m_bKKey;}
  BOOL IsPKeyPressed(){return m_bPKey;}
  BOOL IsRKeyPressed(){return m_bRKey;}
  BOOL IsTabKeyPressed(){return m_bTabKey;}
  BOOL IsLControlKeyPressed(){return m_bLControlKey;}
  BOOL IsNum1KeyPressed(){return m_b1Key;}
  BOOL IsNum2KeyPressed(){return m_b2Key;}
  BOOL IsNum3KeyPressed(){return m_b3Key;}
  BOOL IsNum4KeyPressed(){return m_b4Key;}
  BOOL IsNum5KeyPressed(){return m_b5Key;}
  BOOL IsNum6KeyPressed(){return m_b6Key;}
  BOOL IsNum9KeyPressed(){return m_b9Key;}
  BOOL IsNum0KeyPressed(){return m_b0Key;}
  BOOL IsTKeyPressed(){return m_bTKey;}
  BOOL IsOKeyPressed(){return m_bOKey;}
  BOOL IsUKeyPressed(){return m_bUKey;}
  BOOL IsMKeyPressed(){return m_bMKey;}
  BOOL IsWheelPressed(){return m_bWheel;}
  BOOL IsF2KeyPressed(){return m_bF2Key;}
  BOOL IsF3KeyPressed(){return m_bF3Key;}
  BOOL IsRMousePressed(){return m_bRMouse;}
  BOOL IsLMousePressed(){return m_bLMouse;}

  BOOL SetQKeyPressed(BOOL bpress){return m_bQKey = bpress;}
  BOOL SetVKeyPressed(BOOL bpress){return m_bVKey = bpress;}
  BOOL SetIKeyPressed(BOOL bpress){return m_bIKey = bpress;}
  BOOL SetJKeyPressed(BOOL bpress){return m_bJKey = bpress;}
  BOOL SetKKeyPressed(BOOL bpress){return m_bKKey = bpress;}
  BOOL SetPKeyPressed(BOOL bpress){return m_bPKey = bpress;}
  BOOL SetRKeyPressed(BOOL bpress){return m_bRKey = bpress;}
  BOOL SetTabKeyPressed(BOOL bpress){return m_bTabKey = bpress;}
  BOOL SetLControlKeyPressed(BOOL bpress){return m_bLControlKey = bpress;}
  BOOL SetNum1KeyPressed(BOOL bpress){return m_b1Key = bpress;}
  BOOL SetNum2KeyPressed(BOOL bpress){return m_b2Key = bpress;}
  BOOL SetNum3KeyPressed(BOOL bpress){return m_b3Key = bpress;}
  BOOL SetNum4KeyPressed(BOOL bpress){return m_b4Key = bpress;}
  BOOL SetNum5KeyPressed(BOOL bpress){return m_b5Key = bpress;}
  BOOL SetNum6KeyPressed(BOOL bpress){return m_b6Key = bpress;}
  BOOL SetNum9KeyPressed(BOOL bpress){return m_b9Key = bpress;}
  BOOL SetNum0KeyPressed(BOOL bpress){return m_b0Key = bpress;}
  BOOL SetTKeyPressed(BOOL bpress){return m_bTKey = bpress;}
  BOOL SetOKeyPressed(BOOL bpress){return m_bOKey = bpress;}
  BOOL SetUKeyPressed(BOOL bpress){return m_bUKey = bpress;}
  BOOL SetMKeyPressed(BOOL bpress){return m_bMKey = bpress;}
  BOOL SetWheelPressed(BOOL bpress){return m_bWheel = bpress;}
  BOOL SetF2KeyPressed(BOOL bpress){return m_bF2Key = bpress;}
  BOOL SetF3KeyPressed(BOOL bpress){return m_bF3Key = bpress;}
  BOOL SetRMousePressed(BOOL bpress){return m_bRMouse = bpress;}
  BOOL SetLMousePressed(BOOL bpress){return m_bLMouse = bpress;}
  BOOL SetHomePressed(BOOL bpress){return m_bHome = bpress;}

  BOOL IsLControlKeyPressing();
  BOOL IsRMouseButtonPressed();
  BOOL IsLMouseButtonPressed();
  BOOL IsHomePressed(){return m_bHome;}

	BOOL SendIMEKeyInput(MSG* pMsg);

public:
// Attributes
  CTCriticalSection inp_csInput;
  HWND m_vphWnd;				// canvas (child) window
  RECT m_rectClient;
  BOOL inp_bLastPrescan;
  BOOL inp_bInputEnabled;
  BOOL inp_bPollJoysticks;
  BOOL inp_bFreeMode;

  FLOAT inp_fSpeedMultiplier;
  SLONG inp_slScreenCenterX;                        // screen center X in pixels
  SLONG inp_slScreenCenterY;                        // screen center Y in pixels
  RECT inp_rectOldClip;                             // old cursor clip rectangle in pixels
  POINT inp_ptOldMousePos;                          // old mouse position ( screen coordinate )
  POINT	inp_ptMousePos;								// Current mouse position ( client coordinate )	// yjpark
  struct MouseSpeedControl inp_mscMouseSettings;    // system mouse settings

  CStaticArray<CInputDevice> inp_aInputDevices;     // array of input devices

  //0105
  FLOAT nViewDistance; 
  BOOL m_bForward;
  BOOL m_bWheelDown;
  BOOL m_bRButtonDown;
  BOOL m_bTcpIp;
  int m_WheelPos;
  BOOL m_bLControlKey;
  BOOL m_bPressedLControlKey;
  BOOL m_bTabKey;
  BOOL m_bPressedTabKey;
  BOOL m_bPKey;
  BOOL m_bPressedPKey;
  BOOL m_bRKey;
  BOOL m_bPressedRKey;
  BOOL m_bKKey;
  BOOL m_bPressedKKey;
  BOOL m_bIKey;
  BOOL m_bPressedIKey;
  BOOL m_bLKey;
  BOOL m_bPressedLKey;
  BOOL m_bJKey;
  BOOL m_bPressedJKey;
  BOOL m_b1Key;
  BOOL m_bPressed1Key;
  BOOL m_b2Key;
  BOOL m_bPressed2Key;
  BOOL m_b3Key;
  BOOL m_bPressed3Key;
  BOOL m_b4Key;
  BOOL m_bPressed4Key;
  BOOL m_b5Key;
  BOOL m_bPressed5Key;
  BOOL m_b6Key;
  BOOL m_bPressed6Key;
  BOOL m_b9Key;
  BOOL m_bPressed9Key;
  BOOL m_b0Key;
  BOOL m_bPressed0Key;
  BOOL m_bVKey;
  BOOL m_bPressedVKey;
  BOOL m_bQKey;
  BOOL m_bPressedQKey;
  BOOL m_bTKey;
  BOOL m_bPressedTKey;
  BOOL m_bOKey;
  BOOL m_bPressedOKey;
  BOOL m_bUKey;
  BOOL m_bPressedUKey;
  BOOL m_bMKey;
  BOOL m_bPressedMKey;

  BOOL m_bWheel;
  BOOL m_bPressedWheel;
  BOOL m_bF2Key;
  BOOL m_bPressedF2Key;
  BOOL m_bF3Key;
  BOOL m_bPressedF3Key;

  BOOL m_bRMouse;
  BOOL m_bPressedRMouse;

  BOOL m_bLMouse;
  BOOL m_bPressedLMouse;

  BOOL m_bHome;
  BOOL m_bPressedHome;
};

// pointer to global input object
ENGINE_API extern CInput *_pInput;

// Converts kid to char (use only with keyboard device)
ENGINE_API extern char  GetCharFromKey(ULONG ulControlGID, BOOL bShift);
// Converts char to kid (use only with keyboard device) (unoptimized)
ENGINE_API extern ULONG GetGIDFromChar(char ch);
// Converts virtual key to gid
ENGINE_API extern ULONG GetGIDFromVirtKey(INDEX iVirt);
// Get control global device id from device and control id
ENGINE_API inline ULONG GetControlGID(INDEX iInputDevice,INDEX iControl) {
  return ((iInputDevice<<16)|iControl);
}
// Get control local id from global id
ENGINE_API inline INDEX GetControlLID(ULONG ulGID) {
  return ulGID&0x0000FFFF;
}
// Get control device from global id
ENGINE_API inline INDEX GetInputDeviceID(ULONG ulGID) {
  return (ulGID>>16)&0x0000FFFF;
}

#define CONTROL_NONE GetControlGID(DEVICE_NONE,KID_NONE)

#endif  /* include-once check. */

