xcopy /Y Y:\Engine2\bin\libEngine2.dll .
xcopy /Y Y:\Engine2\bin\libEngine2.dll  import\bin\
xcopy /Y Y:\Engine2\import\bin\libCore2.dll .
xcopy /Y Y:\Engine2\import\bin\libCore2.dll import\bin\
xcopy /Y Y:\Engine2\import\bin\libImage.dll .
xcopy /Y /s Y:\Engine2\include\* import\include\
xcopy /Y /s Y:\Engine2\include\* Y:\Engine2\import\include\
xcopy /Y /s Y:\Engine2\dependencies\Core2\include\* import\include\Core2\

