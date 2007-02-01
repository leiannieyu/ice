# **********************************************************************
#
# Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ..\..\..

PUBLISHER	= publisher.exe
SUBSCRIBER	= subscriber.exe

TARGETS		= $(PUBLISHER) $(SUBSCRIBER)

OBJS		= Clock.obj

POBJS		= Publisher.obj

SOBJS		= Subscriber.obj

SRCS		= $(OBJS:.obj=.cpp) \
		  $(POBJS:.obj=.cpp) \
		  $(SOBJS:.obj=.cpp)


!include $(top_srcdir)/config/Make.rules.mak

CPPFLAGS	= -I. $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN
LIBS		= $(top_srcdir)\lib\icestorm$(LIBSUFFIX).lib $(LIBS)

!if "$(CPP_COMPILER)" != "BCC2006" & "$(OPTIMIZE)" != "yes"
PPDBFLAGS        = /pdb:$(PUBLISHER:.exe=.pdb)
SPDBFLAGS        = /pdb:$(SUBSCRIBER:.exe=.pdb)
!endif

$(PUBLISHER): $(OBJS) $(POBJS)
	$(LINK) $(LD_EXEFLAGS) $(PPDBFLAGS) $(OBJS) $(POBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(PUBLISHER).manifest \
	    mt -nologo -manifest $(PUBLISHER).manifest -outputresource:$(PUBLISHER);#1 & del /q $(PUBLISHER).manifest
	-if exist $(PUBLISHER:.exe=.exp) del /q $(PUBLISHER:.exe=.exp)

$(SUBSCRIBER): $(OBJS) $(SOBJS)
	$(LINK) $(LD_EXEFLAGS) $(SPDBFLAGS) $(OBJS) $(SOBJS) $(PREOUT)$@ $(PRELIBS)$(LIBS)
	-if exist $(SUBSCRIBER).manifest \
	    mt -nologo -manifest $(SUBSCRIBER).manifest -outputresource:$(SUBSCRIBER);#1 & del /q $(SUBSCRIBER).manifest
	-if exist $(SUBSCRIBER:.exe=.exp) del /q $(SUBSCRIBER:.exe=.exp)

clean::
	del /q Clock.cpp Clock.h

!include .depend
