# Microsoft Developer Studio Project File - Name="PSKCoreLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PSKCoreLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PSKCoreLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PSKCoreLib.mak" CFG="PSKCoreLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PSKCoreLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PSKCoreLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PSKCoreLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PSKCoreLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "PSKCoreLib - Win32 Release"
# Name "PSKCoreLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CalcIMD.cpp
# End Source File
# Begin Source File

SOURCE=..\fft.cpp
# End Source File
# Begin Source File

SOURCE=..\IOCntrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Perform.cpp
# End Source File
# Begin Source File

SOURCE=..\PSKCore.cpp
# End Source File
# Begin Source File

SOURCE=..\PSKCore.rc
# End Source File
# Begin Source File

SOURCE=..\PskDet.cpp
# End Source File
# Begin Source File

SOURCE=..\PSKMod.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Wave.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\CalcIMD.h
# End Source File
# Begin Source File

SOURCE=..\ErrorCodes.h
# End Source File
# Begin Source File

SOURCE=..\fft.h
# End Source File
# Begin Source File

SOURCE=..\IOCntrl.h
# End Source File
# Begin Source File

SOURCE=..\Perform.h
# End Source File
# Begin Source File

SOURCE=..\PSKCore.h
# End Source File
# Begin Source File

SOURCE=..\PskCoreExplicit.h
# End Source File
# Begin Source File

SOURCE=..\PskCoreImplicit.h
# End Source File
# Begin Source File

SOURCE=..\PskDet.h
# End Source File
# Begin Source File

SOURCE=..\PskDetTbl.h
# End Source File
# Begin Source File

SOURCE=..\PSKMod.h
# End Source File
# Begin Source File

SOURCE=..\PSKTables.h
# End Source File
# Begin Source File

SOURCE=..\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Sound.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Wave.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# Begin Source File

SOURCE=..\ReadMe.txt
# End Source File
# End Target
# End Project
