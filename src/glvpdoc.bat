REM Create GLView documentation from source files 
nmake /f GLVPDOC.MAK
rem start winhelp GLVPDOC.HLP
start winhlp32 GLVPDOC.HLP

