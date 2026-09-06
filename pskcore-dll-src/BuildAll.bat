cd PSKCoreDLL
nmake /NOLOGO -f pskcoredll.mak CFG="PSKCoreDLL - Win32 Debug"
nmake /NOLOGO -f pskcoredll.mak CFG="PSKCoreDLL - Win32 Release"
cd ..\PSKCoreLib
nmake /NOLOGO -f pskcorelib.mak CFG="PSKCoreLib - Win32 Debug"
nmake /NOLOGO -f pskcorelib.mak CFG="PSKCoreLib - Win32 Release"
cd ..
