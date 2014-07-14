REM Regenerate parser files 
rem flex  scan.lpp
rem copy lexyy.c scan.cpp

bison --define --verbose parse.ypp
copy parse.ypp_tab.h y_tab.h
copy parse.ypp_tab.c parse.cpp





