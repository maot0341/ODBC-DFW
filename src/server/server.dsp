# Microsoft Developer Studio Project File - Name="server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=server - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="server - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "server - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "server - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Release\"
# PROP Intermediate_Dir "..\..\obj\Release\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I ".." /I "..\idl" /I "$(CORBA_HOME)\Win32Release\OB\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ob.lib jtc.lib sqlp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /pdbtype:sept /libpath:"..\..\lib\Release" /libpath:"$(CORBA_HOME)\Win32Release\OB\lib"

!ELSEIF  "$(CFG)" == "server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\obj\Debug\server"
# PROP Intermediate_Dir "..\..\obj\Debug\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I ".." /I "..\idl" /I "$(CORBA_HOME)\Win32Debug\OB\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 obd.lib jtcd.lib sqlp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\Debug\server.exe" /pdbtype:sept /libpath:"..\..\lib\Debug" /libpath:"$(CORBA_HOME)\Win32Debug\OB\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "server - Win32 Release"
# Name "server - Win32 Debug"
# Begin Group "catalog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SQLColumns.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLColumns.h
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

SOURCE=.\IRemote_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\IRemote_impl.h
# End Source File
# Begin Source File

SOURCE=..\idl\IRemote_skel_idl.cpp
# End Source File
# Begin Source File

SOURCE=..\stdx\regexpr.cpp
# End Source File
# Begin Source File

SOURCE=..\stdx\regexpr.h
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# End Target
# End Project
