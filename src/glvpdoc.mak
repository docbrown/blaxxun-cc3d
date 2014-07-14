# MAKEFILE
# GLView GLVPDOC Autodoc Project
#
# Copyright (c)1993 Microsoft Corporation, All Rights Reserved
#
# Eric Artzt, Program Manager
# Multimedia Document Archicture
#
# Internet  :  erica@microsoft.com
#

TARGET  = GLVPDOC 
TITLE   = GLView Programmers Help
AD      = D:\msdnsmpl\untools\autoduck\autoduck.exe
ADLOG   = /v /L$(TARGET).LOG /N
ADHLP   = /RH /C$(TARGET).LOG /O$(TARGET).RTF /D "title=$(TITLE)"
ADDOC   = /RD /O$(TARGET).DOC /D "doc_header=$(DOCHDR)"
#ADTAB   = 8
ADTAB   = 4
#HC      = hc31.exe
HC      = hcrtf.exe -x 
#SOURCE  = *.cpp *.h *.d
SOURCE  = *.cpp	*.h *.d core\*.cpp core\*.h

# Help and Doc targets

hlp : $(TARGET).hlp

doc : $(TARGET).doc

#clean: cleanad

#cleanad:
#    if exist *.rtf del *.rtf
#    if exist *.hpj del *.hpj
#    if exist $(TARGET).log del $(TARGET).log
#    if exist $(TARGET).doc del $(TARGET).doc
#    if exist $(TARGET).hlp del $(TARGET).hlp

# Generate a Help file

$(TARGET).rtf : $(TARGET).log $(SOURCE) $(FMT)
    $(AD) $(ADHLP) /t$(ADTAB) $(SOURCE)

$(TARGET).hlp : $(TARGET).rtf
    $(HC) $(TARGET)

# Generate a topic log file

$(TARGET).log : $(SOURCE) $(FMT)
    $(AD) $(ADLOG) $(SOURCE)

# Generate a print documentation file

$(TARGET).doc : $(FMT) $(SOURCE)
    $(AD) $(ADDOC) /t$(ADTAB) $(SOURCE)

