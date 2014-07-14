@REM Regenerate parser files 
rem flex  scan.lpp
rem copy lexyy.c scan.cpp

bison --define --verbose --debug --yacc parse.ypp
copy y_tab.h y.tab.h
copy y_tab.c parse.cpp





