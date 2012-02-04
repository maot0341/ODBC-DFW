@set path=%CORBA_HOME%\Win32Debug\OB\bin;%path%
@set path=%CORBA_HOME%\Win32Debug\OB\lib;%path%
idl.exe --c-suffix _idl.cpp --h-suffix _idl.h %*