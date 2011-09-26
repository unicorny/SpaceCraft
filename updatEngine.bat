xcopy /Y Y:\Engine2\bin\libEngine2.dll .
xcopy /Y Y:\Engine2\bin\libEngine2.dll  import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\libCore2.dll .
xcopy /Y Y:\Engine2\dependencies\bin\libCore2.dll import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\libImage.dll .

xcopy /Y Y:\Engine2\bin\Debug\Engine2.dll .
xcopy /Y Y:\Engine2\bin\Debug\Engine2.dll  import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\Core2.dll .
xcopy /Y Y:\Engine2\dependencies\bin\Debug\Core2.dll import\bin\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\Image.dll .

xcopy /Y Y:\Engine2\bin\Debug\Engine2.dll Debug\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\Core2.dll Debug\
xcopy /Y Y:\Engine2\dependencies\bin\Debug\Image.dll Debug\

xcopy /Y /s Y:\Engine2\include\* import\include\
xcopy /Y /s Y:\Engine2\include\* Y:\Engine2\import\include\
xcopy /Y /s Y:\Engine2\dependencies\Core2\include\* import\include\Core2\
xcopy /Y /s Y:\Engine2\lib\Debug\*.lib import\lib\
xcopy /Y /s Y:\Engine2\dependencies\lib\Debug\Core2.lib import\lib\

