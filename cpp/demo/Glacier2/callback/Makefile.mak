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
SESSION_SERVER	= sessionserver.exe

TARGETS		= $(CLIENT) $(SERVER) $(SESSION_SERVER)

OBJS		= Callback.obj \
		  CallbackI.obj

COBJS		= Client.obj

SOBJS		= Server.obj

SSOBJS		= SessionServer.obj \
		  SessionI.obj

SRCS		= $(OBJS:.obj=.cpp) \
		  $(COBJS:.obj=.cpp) \
		  $(SOBJS:.obj=.cpp) \
		  $(SSOBJS:.obj=.cpp)

!include $(top_srcdir)/config/Make.rules.mak

CPPFLAGS	= -I. $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN

!if "$(CPP_COMPILER)" != "BCC2006" & "$(OPTIMIZE)" != "yes"
CPDBFLAGS        = /pdb:$(CLIENT:.exe=.pdb)
SPDBFLAGS        = /pdb:$(SERVER:.exe=.pdb)
SSPDBFLAGS       = /pdb:$(SESSION_SERVER:.exe=.pdb)
!endif

$(CLIENT): $(OBJS) $(COBJS)
	$(LINK) $(LD_EXEFLAGS) $(CPDBFLAGS) $(OBJS) $(COBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS) glacier2$(LIBSUFFIX).lib
	-if exist $(CLIENT).manifest \
	    mt -nologo -manifest $(CLIENT).manifest -outputresource:$(CLIENT);#1 & del /q $(CLIENT).manifest
	-if exist $(CLIENT:.exe=.exp) del /q $(CLIENT:.exe=.exp)

$(SERVER): $(OBJS) $(SOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SPDBFLAGS) $(OBJS) $(SOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(SERVER).manifest \
	    mt -nologo -manifest $(SERVER).manifest -outputresource:$(SERVER);#1 & del /q $(SERVER).manifest
	-if exist $(SERVER:.exe=.exp) del /q $(SERVER:.exe=.exp)

$(SESSION_SERVER): $(SSOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SSPDBFLAGS) $(SSOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS) glacier2$(LIBSUFFIX).lib
	-if exist $(SESSION_SERVER).manifest \
	    mt -nologo -manifest $(SESSION_SERVER).manifest -outputresource:$(SESSION_SERVER);#1 & \
	    del /q $(SESSION_SERVER).manifest
	-if exist $(SESSION_SERVER:.exe=.exp) del /q $(SESSION_SERVER:.exe=.exp)

clean::
	del /q Callback.cpp Callback.h

!include .depend
