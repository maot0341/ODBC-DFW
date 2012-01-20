# Microsoft Developer Studio Project File - Name="driver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=driver - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "driver.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "driver.mak" CFG="driver - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "driver - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "driver - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "driver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/Release/driver"
# PROP Intermediate_Dir "../../obj/Release/driver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DRIVER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /w /W0 /GR /GX /O2 /I ".." /D "BUILD_ODBC_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 sqlp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/Release/driver.dll" /implib:"../../lib/Release/driver.lib" /libpath:"../../lib/Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "driver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/Debug/driver"
# PROP Intermediate_Dir "../../obj/Debug/driver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DRIVER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I ".." /D "BUILD_ODBC_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 sqlp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /def:".\driver.def" /out:"../../bin/Debug/driver.dll" /implib:"../../lib/Debug/driver.lib" /pdbtype:sept /libpath:"../../lib/Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "driver - Win32 Release"
# Name "driver - Win32 Debug"
# Begin Group "database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\CStorage.h
# End Source File
# Begin Source File

SOURCE=.\CTableERG.cpp
# End Source File
# Begin Source File

SOURCE=.\CTableERG.h
# End Source File
# Begin Source File

SOURCE=.\CTableImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CTableImpl.h
# End Source File
# Begin Source File

SOURCE=.\CTableTST.cpp
# End Source File
# Begin Source File

SOURCE=.\CTableTST.h
# End Source File
# Begin Source File

SOURCE=.\database.cpp
# End Source File
# Begin Source File

SOURCE=.\database.h
# End Source File
# End Group
# Begin Group "catalog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SQLColumns.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLColumns.h
# End Source File
# Begin Source File

SOURCE=.\SQLError.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLError.h
# End Source File
# Begin Source File

SOURCE=.\SQLGetFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLGetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLGetTypeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLGetTypeInfo.h
# End Source File
# Begin Source File

SOURCE=.\SQLQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLQuery.h
# End Source File
# Begin Source File

SOURCE=.\SQLSpecialColumns.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLSpecialColumns.h
# End Source File
# Begin Source File

SOURCE=.\SQLStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLStatistics.h
# End Source File
# Begin Source File

SOURCE=.\SQLTables.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLTables.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\CHandle.h
# End Source File
# Begin Source File

SOURCE=.\CParamImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CParamImpl.h
# End Source File
# Begin Source File

SOURCE=.\CStmtHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\CStmtHandle.h
# End Source File
# Begin Source File

SOURCE=.\driver.cpp
# End Source File
# Begin Source File

SOURCE=.\driver.def

!IF  "$(CFG)" == "driver - Win32 Release"

!ELSEIF  "$(CFG)" == "driver - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver.h
# End Source File
# Begin Source File

SOURCE=..\stdx\regexpr.cpp
# End Source File
# Begin Source File

SOURCE=.\sql.cpp
# End Source File
# Begin Source File

SOURCE=.\sqlcpy.cpp
# End Source File
# Begin Source File

SOURCE=.\sqlcpy.h
# End Source File
# End Target
# End Project
