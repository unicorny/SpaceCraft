xcopy /Y Y:\Engine2\bin\libDREngine2.dll .
xcopy /Y Y:\Engine2\bin\libDREngine2.dll  import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\libDRCore2.dll .
xcopy /Y Y:\Engine2\dependencies\bin\libDRCore2.dll import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\libDRImage.dll .

xcopy /Y Y:\Engine2\bin\Debug\DREngine2.dll .
xcopy /Y Y:\Engine2\bin\Debug\DREngine2.dll  import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\DRCore2.dll .
xcopy /Y Y:\Engine2\dependencies\bin\Debug\DRCore2.dll import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\DRImage.dll .

xcopy /Y Y:\Engine2\bin\Debug\DREngine2.dll Debug\
xcopy /Y Y:\Engine2\bin\Release\DREngine2.dll Release\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\DRCore2.dll Debug\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\DRImage.dll Debug\
xcopy /Y Y:\Engine2\dependencies\bin\Release\DRCore2.dll Release\
xcopy /Y Y:\Engine2\dependencies\bin\Release\DRImage.dll Release\

xcopy /Y /s Y:\Engine2\include\* import\include\
xcopy /Y /s Y:\Engine2\include\* Y:\Engine2\import\include\
xcopy /Y /s Y:\Engine2\dependencies\Core2\include\* import\include\Core2\
xcopy /Y /s Y:\Engine2\lib\Debug\*.lib import\lib\
xcopy /Y /s Y:\Engine2\dependencies\lib\Debug\DRCore2.lib import\lib\

