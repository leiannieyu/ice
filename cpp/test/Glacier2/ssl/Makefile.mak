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

TARGETS		= $(CLIENT) $(SERVER)

COBJS		= Client.obj

SOBJS		= Server.obj

SRCS		= $(COBJS:.obj=.cpp) \
		  $(SOBJS:.obj=.cpp)

!include $(top_srcdir)/config/Make.rules.mak

CPPFLAGS	= -I. -I../../include $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN

!if "$(CPP_COMPILER)" != "BCC2006" & "$(OPTIMIZE)" != "yes"
CPDBFLAGS        = /pdb:$(CLIENT:.exe=.pdb)
SPDBFLAGS        = /pdb:$(SERVER:.exe=.pdb)
!endif

$(CLIENT): $(COBJS)
	$(LINK) $(LD_EXEFLAGS) $(CPDBFLAGS) $(COBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS) glacier2$(LIBSUFFIX).lib
	-if exist $(CLIENT).manifest \
	    mt -nologo -manifest $(CLIENT).manifest -outputresource:$(CLIENT);#1 & del /q $(CLIENT).manifest
	-if exist $(CLIENT:.exe=.exp) del /q $(CLIENT:.exe=.exp)

$(SERVER): $(SOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SPDBFLAGS) $(SOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS) glacier2$(LIBSUFFIX).lib \
	  $(OPENSSL_LIBS) icessl$(LIBSUFFIX).lib
	-if exist $(SERVER).manifest \
	    mt -nologo -manifest $(SERVER).manifest -outputresource:$(SERVER);#1 & del /q $(SERVER).manifest
	-if exist $(SERVER:.exe=.exp) del /q $(SERVER:.exe=.exp)

!include .depend
