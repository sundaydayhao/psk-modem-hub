# Microsoft Developer Studio Generated NMAKE File, Based on PSKCoreDLL.dsp
!IF "$(CFG)" == ""
CFG=PSKCoreDLL - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PSKCoreDLL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PSKCoreDLL - Win32 Release" && "$(CFG)" != "PSKCoreDLL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PSKCoreDLL.mak" CFG="PSKCoreDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PSKCoreDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PSKCoreDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PSKCore.dll"


CLEAN :
	-@erase "$(INTDIR)\CalcIMD.obj"
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\IOCntrl.obj"
	-@erase "$(INTDIR)\Perform.obj"
	-@erase "$(INTDIR)\PSKCore.obj"
	-@erase "$(INTDIR)\PSKCore.res"
	-@erase "$(INTDIR)\PSKCoreDLL.pch"
	-@erase "$(INTDIR)\PskDet.obj"
	-@erase "$(INTDIR)\PSKMod.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Wave.obj"
	-@erase "$(OUTDIR)\PSKCore.dll"
	-@erase "$(OUTDIR)\PSKCore.exp"
	-@erase "$(OUTDIR)\PSKCore.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COREDLL_EXPORTS" /D "_WINDLL" /D "DLL_VERSION" /Fp"$(INTDIR)\PSKCoreDLL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\PSKCore.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PSKCoreDLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\PSKCore.pdb" /machine:I386 /def:"..\PSKCore.def" /out:"$(OUTDIR)\PSKCore.dll" /implib:"$(OUTDIR)\PSKCore.lib" 
DEF_FILE= \
	"..\PSKCore.def"
LINK32_OBJS= \
	"$(INTDIR)\CalcIMD.obj" \
	"$(INTDIR)\fft.obj" \
	"$(INTDIR)\IOCntrl.obj" \
	"$(INTDIR)\Perform.obj" \
	"$(INTDIR)\PSKCore.obj" \
	"$(INTDIR)\PskDet.obj" \
	"$(INTDIR)\PSKMod.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Wave.obj" \
	"$(INTDIR)\PSKCore.res"

"$(OUTDIR)\PSKCore.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PSKCore.dll" "$(OUTDIR)\PSKCoreDLL.bsc"


CLEAN :
	-@erase "$(INTDIR)\CalcIMD.obj"
	-@erase "$(INTDIR)\CalcIMD.sbr"
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\fft.sbr"
	-@erase "$(INTDIR)\IOCntrl.obj"
	-@erase "$(INTDIR)\IOCntrl.sbr"
	-@erase "$(INTDIR)\Perform.obj"
	-@erase "$(INTDIR)\Perform.sbr"
	-@erase "$(INTDIR)\PSKCore.obj"
	-@erase "$(INTDIR)\PSKCore.res"
	-@erase "$(INTDIR)\PSKCore.sbr"
	-@erase "$(INTDIR)\PSKCoreDLL.pch"
	-@erase "$(INTDIR)\PskDet.obj"
	-@erase "$(INTDIR)\PskDet.sbr"
	-@erase "$(INTDIR)\PSKMod.obj"
	-@erase "$(INTDIR)\PSKMod.sbr"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Sound.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wave.obj"
	-@erase "$(INTDIR)\Wave.sbr"
	-@erase "$(OUTDIR)\PSKCore.dll"
	-@erase "$(OUTDIR)\PSKCore.exp"
	-@erase "$(OUTDIR)\PSKCore.ilk"
	-@erase "$(OUTDIR)\PSKCore.lib"
	-@erase "$(OUTDIR)\PSKCore.pdb"
	-@erase "$(OUTDIR)\PSKCoreDLL.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "COREDLL_EXPORTS" /D "DLL_VERSION" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PSKCoreDLL.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\PSKCore.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PSKCoreDLL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CalcIMD.sbr" \
	"$(INTDIR)\fft.sbr" \
	"$(INTDIR)\IOCntrl.sbr" \
	"$(INTDIR)\Perform.sbr" \
	"$(INTDIR)\PSKCore.sbr" \
	"$(INTDIR)\PskDet.sbr" \
	"$(INTDIR)\PSKMod.sbr" \
	"$(INTDIR)\Sound.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Wave.sbr"

"$(OUTDIR)\PSKCoreDLL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\PSKCore.pdb" /debug /machine:I386 /def:"..\PSKCore.def" /out:"$(OUTDIR)\PSKCore.dll" /implib:"$(OUTDIR)\PSKCore.lib" /pdbtype:sept 
DEF_FILE= \
	"..\PSKCore.def"
LINK32_OBJS= \
	"$(INTDIR)\CalcIMD.obj" \
	"$(INTDIR)\fft.obj" \
	"$(INTDIR)\IOCntrl.obj" \
	"$(INTDIR)\Perform.obj" \
	"$(INTDIR)\PSKCore.obj" \
	"$(INTDIR)\PskDet.obj" \
	"$(INTDIR)\PSKMod.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Wave.obj" \
	"$(INTDIR)\PSKCore.res"

"$(OUTDIR)\PSKCore.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PSKCoreDLL.dep")
!INCLUDE "PSKCoreDLL.dep"
!ELSE 
!MESSAGE Warning: cannot find "PSKCoreDLL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PSKCoreDLL - Win32 Release" || "$(CFG)" == "PSKCoreDLL - Win32 Debug"
SOURCE=..\CalcIMD.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\CalcIMD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\CalcIMD.obj"	"$(INTDIR)\CalcIMD.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fft.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\fft.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\fft.obj"	"$(INTDIR)\fft.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\IOCntrl.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\IOCntrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\IOCntrl.obj"	"$(INTDIR)\IOCntrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Perform.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\Perform.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\Perform.obj"	"$(INTDIR)\Perform.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\PSKCore.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\PSKCore.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\PSKCore.obj"	"$(INTDIR)\PSKCore.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\PSKCore.rc

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\PSKCore.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\PSKCore.res" /i "\Amateur Radio\AE4JY\PSKCore\V1.14\PSK31Core" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\PSKCore.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\PSKCore.res" /i "\Amateur Radio\AE4JY\PSKCore\V1.14\PSK31Core" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\PskDet.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\PskDet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\PskDet.obj"	"$(INTDIR)\PskDet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\PSKMod.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\PSKMod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\PSKMod.obj"	"$(INTDIR)\PSKMod.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Sound.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\Sound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\Sound.obj"	"$(INTDIR)\Sound.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COREDLL_EXPORTS" /D "_WINDLL" /D "DLL_VERSION" /Fp"$(INTDIR)\PSKCoreDLL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\PSKCoreDLL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /D "COREDLL_EXPORTS" /D "DLL_VERSION" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PSKCoreDLL.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\PSKCoreDLL.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Wave.cpp

!IF  "$(CFG)" == "PSKCoreDLL - Win32 Release"


"$(INTDIR)\Wave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreDLL - Win32 Debug"


"$(INTDIR)\Wave.obj"	"$(INTDIR)\Wave.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreDLL.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

