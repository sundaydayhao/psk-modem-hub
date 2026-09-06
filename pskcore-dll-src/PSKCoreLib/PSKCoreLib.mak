# Microsoft Developer Studio Generated NMAKE File, Based on PSKCoreLib.dsp
!IF "$(CFG)" == ""
CFG=PSKCoreLib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PSKCoreLib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PSKCoreLib - Win32 Release" && "$(CFG)" != "PSKCoreLib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PSKCoreLib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PSKCoreLib.lib"


CLEAN :
	-@erase "$(INTDIR)\CalcIMD.obj"
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\IOCntrl.obj"
	-@erase "$(INTDIR)\Perform.obj"
	-@erase "$(INTDIR)\PSKCore.obj"
	-@erase "$(INTDIR)\PSKCore.res"
	-@erase "$(INTDIR)\PSKCoreLib.pch"
	-@erase "$(INTDIR)\PskDet.obj"
	-@erase "$(INTDIR)\PSKMod.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Wave.obj"
	-@erase "$(OUTDIR)\PSKCoreLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PSKCoreLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\PSKCore.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PSKCoreLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\PSKCoreLib.lib" 
LIB32_OBJS= \
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

"$(OUTDIR)\PSKCoreLib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PSKCoreLib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PSKCoreLib.lib"


CLEAN :
	-@erase "$(INTDIR)\CalcIMD.obj"
	-@erase "$(INTDIR)\fft.obj"
	-@erase "$(INTDIR)\IOCntrl.obj"
	-@erase "$(INTDIR)\Perform.obj"
	-@erase "$(INTDIR)\PSKCore.obj"
	-@erase "$(INTDIR)\PSKCore.res"
	-@erase "$(INTDIR)\PSKCoreLib.pch"
	-@erase "$(INTDIR)\PskDet.obj"
	-@erase "$(INTDIR)\PSKMod.obj"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wave.obj"
	-@erase "$(OUTDIR)\PSKCoreLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PSKCoreLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\PSKCore.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PSKCoreLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\PSKCoreLib.lib" 
LIB32_OBJS= \
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

"$(OUTDIR)\PSKCoreLib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PSKCoreLib.dep")
!INCLUDE "PSKCoreLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "PSKCoreLib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PSKCoreLib - Win32 Release" || "$(CFG)" == "PSKCoreLib - Win32 Debug"
SOURCE=..\CalcIMD.cpp

"$(INTDIR)\CalcIMD.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\fft.cpp

"$(INTDIR)\fft.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\IOCntrl.cpp

"$(INTDIR)\IOCntrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Perform.cpp

"$(INTDIR)\Perform.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PSKCore.cpp

"$(INTDIR)\PSKCore.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PSKCore.rc

!IF  "$(CFG)" == "PSKCoreLib - Win32 Release"


"$(INTDIR)\PSKCore.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\PSKCore.res" /i "\Amateur Radio\AE4JY\PSKCore\V1.14\PSK31Core" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "PSKCoreLib - Win32 Debug"


"$(INTDIR)\PSKCore.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\PSKCore.res" /i "\Amateur Radio\AE4JY\PSKCore\V1.14\PSK31Core" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\PskDet.cpp

"$(INTDIR)\PskDet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\PSKMod.cpp

"$(INTDIR)\PSKMod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Sound.cpp

"$(INTDIR)\Sound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "PSKCoreLib - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PSKCoreLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\PSKCoreLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "PSKCoreLib - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\PSKCoreLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\PSKCoreLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Wave.cpp

"$(INTDIR)\Wave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PSKCoreLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

