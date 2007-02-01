# **********************************************************************
#
# Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ..\..\..

CLIENT		= client.exe
SERVER		= server.exe
SERVERAMD	= serveramd.exe
COLLOCATED	= collocated.exe

TARGETS		= $(CLIENT) $(SERVER) $(SERVERAMD) $(COLLOCATED)

COBJS		= Test.obj \
		  Client.obj \
		  AllTests.obj

SOBJS		= Test.obj \
		  TestI.obj \
		  Server.obj

SAMDOBJS	= TestAMD.obj \
		  TestAMDI.obj \
		  ServerAMD.obj

COLOBJS		= Test.obj \
		  TestI.obj \
		  Collocated.obj \
		  AllTests.obj

SRCS		= $(COBJS:.obj=.cpp) \
		  $(SOBJS:.obj=.cpp) \
		  $(SAMDOBJS:.obj=.cpp) \
		  $(COLOBJS:.obj=.cpp)

!include $(top_srcdir)/config/Make.rules.mak

CPPFLAGS	= -I. -I../../include $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN

!if "$(CPP_COMPILER)" != "BCC2006" & "$(OPTIMIZE)" != "yes"
CPDBFLAGS        = /pdb:$(CLIENT:.exe=.pdb)
SPDBFLAGS        = /pdb:$(SERVER:.exe=.pdb)
SAPDBFLAGS       = /pdb:$(SERVERAMD:.exe=.pdb)
COPDBFLAGS       = /pdb:$(COLLOCATED:.exe=.pdb)
!endif

$(CLIENT): $(COBJS)
	$(LINK) $(LD_EXEFLAGS) $(CPDBFLAGS) $(COBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(CLIENT).manifest \
	    mt -nologo -manifest $(CLIENT).manifest -outputresource:$(CLIENT);#1 & del /q $(CLIENT).manifest
	-if exist $(CLIENT:.exe=.exp) del /q $(CLIENT:.exe=.exp)

$(SERVER): $(SOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SPDBFLAGS) $(SOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(SERVER).manifest \
	    mt -nologo -manifest $(SERVER).manifest -outputresource:$(SERVER);#1 & del /q $(SERVER).manifest
	-if exist $(SERVER:.exe=.exp) del /q $(SERVER:.exe=.exp)

$(SERVERAMD): $(SAMDOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SAPDBFLAGS) $(SAMDOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(SERVERAMD).manifest \
	    mt -nologo -manifest $(SERVERAMD).manifest -outputresource:$(SERVERAMD);#1 & del /q $(SERVERAMD).manifest
	-if exist $(SERVERAMD:.exe=.exp) del /q $(SERVERAMD:.exe=.exp)

$(COLLOCATED): $(COLOBJS)
	$(LINK) $(LD_EXEFLAGS) $(COPDBFLAGS) $(COLOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(COLLOCATED).manifest \
	    mt -nologo -manifest $(COLLOCATED).manifest -outputresource:$(COLLOCATED);#1 & del /q $(COLLOCATED).manifest
	-if exist $(COLLOCATED:.exe=.exp) del /q $(COLLOCATED:.exe=.exp)

clean::
	del /q Test.cpp Test.h
	del /q TestAMD.cpp TestAMD.h

!include .depend
