@echo off
REM -- First, make map file from resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by GLVIEWCTRL.HPJ. >hlp\GLViewCtrl.hm
echo. >>hlp\GLViewCtrl.hm
echo // Commands (ID_* and IDM_*) >>hlp\GLViewCtrl.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\GLViewCtrl.hm
echo. >>hlp\GLViewCtrl.hm
echo // Prompts (IDP_*) >>hlp\GLViewCtrl.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\GLViewCtrl.hm
echo. >>hlp\GLViewCtrl.hm
echo // Resources (IDR_*) >>hlp\GLViewCtrl.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\GLViewCtrl.hm
echo. >>hlp\GLViewCtrl.hm
echo // Dialogs (IDD_*) >>hlp\GLViewCtrl.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\GLViewCtrl.hm
echo. >>hlp\GLViewCtrl.hm
echo // Frame Controls (IDW_*) >>hlp\GLViewCtrl.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\GLViewCtrl.hm
REM -- Make help for Project GLVIEWCTRL
start /wait hcrtf -x "GLViewCtrl.hpj"
echo.
