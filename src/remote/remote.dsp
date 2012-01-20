# Microsoft Developer Studio Project File - Name="remote" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=remote - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "remote.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "remote.mak" CFG="remote - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "remote - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "remote - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "remote - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/Release/remote"
# PROP Intermediate_Dir "../../obj/Release/remote"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I ".." /I "..\idl" /I "$(CORBA_HOME)\Win32Release\OB\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTE_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ob.lib jtc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /debugtype:both /machine:I386 /out:"../../bin/Release/driver.dll" /pdbtype:sept /libpath:"..\lib\Release" /libpath:"$(CORBA_HOME)\Win32Release\OB\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "remote - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../bin/Debug"
# PROP Intermediate_Dir "../../obj/Debug/remote"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".." /I "..\idl" /I "$(CORBA_HOME)\Win32Debug\OB\include" /I "D:\jvater\odbc\server" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTE_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /i "D:\jvater\odbc\src\remote" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 obd.lib jtcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/Debug/driver.dll" /pdbtype:sept /libpath:"..\..\lib\Debug" /libpath:"$(CORBA_HOME)\Win32Debug\OB\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "remote - Win32 Release"
# Name "remote - Win32 Debug"
# Begin Source File

SOURCE=.\CHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\CHandle.h
# End Source File
# Begin Source File

SOURCE=.\CParam.cpp
# End Source File
# Begin Source File

SOURCE=.\CParam.h
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
# End Source File
# Begin Source File

SOURCE=.\driver.h
# End Source File
# Begin Source File

SOURCE=.\idlcpy.cpp
# End Source File
# Begin Source File

SOURCE=.\idlcpy.h
# End Source File
# Begin Source File

SOURCE=..\idl\IRemote_idl.cpp
# End Source File
# Begin Source File

SOURCE=..\idl\IRemote_idl.h
# End Source File
# Begin Source File

SOURCE=.\remote.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sql.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLDriverConnect.cpp
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

SOURCE=..\driver\SQLGetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLGetTypeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLTables.cpp
# End Source File
# Begin Source File

SOURCE=.\TLink.h
# End Source File
# End Target
# End Project
