# Microsoft Developer Studio Project File - Name="sqlplib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=sqlplib - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "sqlplib.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "sqlplib.mak" CFG="sqlplib - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "sqlplib - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "sqlplib - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sqlplib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release\sqlplib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I ".." /I "$(GNUWIN_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\Release\sqlp.lib"

!ELSEIF  "$(CFG)" == "sqlplib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug\sqlplib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I ".." /I "$(GNUWIN_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "YYDEBUG" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\Debug\sqlp.lib"

!ENDIF 

# Begin Target

# Name "sqlplib - Win32 Release"
# Name "sqlplib - Win32 Debug"
# Begin Source File

SOURCE=.\desc.cpp
# End Source File
# Begin Source File

SOURCE=.\desc.h
# End Source File
# Begin Source File

SOURCE=.\function.cpp
# End Source File
# Begin Source File

SOURCE=.\function.h
# End Source File
# Begin Source File

SOURCE=.\index.cpp
# End Source File
# Begin Source File

SOURCE=.\index.h
# End Source File
# Begin Source File

SOURCE=.\lex.cpp
# End Source File
# Begin Source File

SOURCE=.\lex.l

!IF  "$(CFG)" == "sqlplib - Win32 Release"

# Begin Custom Build - FLEX wird für $(InputPath) ausgeführt
InputPath=.\lex.l

"lex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	run flex -i -olex.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "sqlplib - Win32 Debug"

# Begin Custom Build - FLEX wird für $(InputPath) ausgeführt
InputPath=.\lex.l

"lex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	run flex -i -olex.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\match.cpp
# End Source File
# Begin Source File

SOURCE=.\resultset.cpp
# End Source File
# Begin Source File

SOURCE=.\resultset.h
# End Source File
# Begin Source File

SOURCE=.\sqlp.cpp
# End Source File
# Begin Source File

SOURCE=.\sqlp.h
# End Source File
# Begin Source File

SOURCE=.\svector.h
# End Source File
# Begin Source File

SOURCE=.\tableset.cpp
# End Source File
# Begin Source File

SOURCE=.\tableset.h
# End Source File
# Begin Source File

SOURCE=.\unary.h
# End Source File
# Begin Source File

SOURCE=.\yac.cpp
# End Source File
# Begin Source File

SOURCE=.\yac.h
# End Source File
# Begin Source File

SOURCE=.\yac.y

!IF  "$(CFG)" == "sqlplib - Win32 Release"

# Begin Custom Build - YACC wird für $(InputPath) ausgeführt
InputPath=.\yac.y

"yac.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	run bison  -d -oyac.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "sqlplib - Win32 Debug"

# Begin Custom Build - YACC wird für $(InputPath) ausgeführt
InputPath=.\yac.y

"yac.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	run bison -d -oyac.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
