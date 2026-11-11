set arg1="DittoXPortable"

if "%1" == "bit32" set arg1="DittoXPortable32"

cd DittoXSetup


Rmdir DittoX /s /q

MkDir DittoX
MkDir DittoX\Help
MkDir DittoX\Language
MkDir DittoX\Themes
MkDir DittoX\Addins

if "%1" == "bit64" goto bit64

copy ..\Release\DittoX.exe DittoX\DittoX.exe
copy ..\Release\ICU_Loader.dll DittoX\ICU_Loader.dll
copy ..\Release\Addins\*.dll DittoX\Addins\

copy "%WINDIR%\SysWOW64\vcruntime140.dll" DittoX\vcruntime140.dll
copy "%WINDIR%\SysWOW64\msvcp140.dll" DittoX\msvcp140.dll
copy "%WINDIR%\SysWOW64\mfc140u.dll" DittoX\mfc140u.dll

if "%1" == "bit32" goto skipBit64
:bit64

copy ..\Release64\DittoX.exe DittoX\DittoX.exe
copy ..\Release64\ICU_Loader.dll DittoX\ICU_Loader.dll
copy ..\Release64\Addins\*.dll DittoX\Addins\
copy ..\Release64\libpng16.dll DittoX\libpng16.dll
copy ..\Release64\focus.dll DittoX\focus.dll

copy "%WINDIR%\System32\vcruntime140.dll" DittoX\vcruntime140.dll
copy "%WINDIR%\System32\vcruntime140_1.dll" DittoX\vcruntime140_1.dll
copy "%WINDIR%\System32\msvcp140.dll" DittoX\msvcp140.dll
copy "%WINDIR%\System32\mfc140u.dll" DittoX\mfc140u.dll

:skipBit64

copy Changes.txt DittoX\Changes.txt

copy portable DittoX\portable

copy ..\Debug\language\*.xml DittoX\language\
copy ..\Debug\themes\*.xml DittoX\themes\

del Output\%arg1%.zip 2>nul
7z a -tzip Output\%arg1%.zip "DittoX\*" -r

Rmdir DittoX /s /q

cd ..
