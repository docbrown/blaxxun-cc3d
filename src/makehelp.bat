@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by GLView.HPJ. >hlp\GLView.hm
echo. >>hlp\GLView.hm
echo // Commands (ID_* and IDM_*) >>hlp\GLView.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\GLView.hm
echo. >>hlp\GLView.hm
echo // Prompts (IDP_*) >>hlp\GLView.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\GLView.hm
echo. >>hlp\GLView.hm
echo // Resources (IDR_*) >>hlp\GLView.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\GLView.hm
echo. >>hlp\GLView.hm
echo // Dialogs (IDD_*) >>hlp\GLView.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\GLView.hm
echo. >>hlp\GLView.hm
echo // Frame Controls (IDW_*) >>hlp\GLView.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\GLView.hm

REM -- Make help for Project GLView

echo Building Win32 Help files
REM 2.2 call hc31 GLView.hpj
REM D:\MSVC20\BIN\hc31 GLView.hpj

echo 4.0 Building Win32 Help files
start /wait hcrtf -x "glview.hpj"
echo.

if exist WinDebug copy GLView.hlp windebug
if exist WinRel copy GLView.hlp winrel
if exist W32_Eval copy GLView.hlp win32_Eval
EndLocal
whelp32 glview.hlp

