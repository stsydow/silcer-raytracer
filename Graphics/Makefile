# Copyright (c) 2010-2011, Sebastian Koch <seb@cs.tu-berlin.de> and
#		           Stefan Sydow <stsydow@cs.tu-berlin.de>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this list of
# conditions and the following disclaimer.
# 
# Redistributions in binary form must reproduce the above copyright notice, this list of
# conditions and the following disclaimer in the documentation and/or other materials
# provided with the distribution.
# 
# Neither the name of the TU-Berlin nor the names of its contributors may be used to
# endorse or promote products derived from this software without specific prior written
# permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ROOT=..

INC=-I/usr/include/freetype2 
OPTIMIZE = -march=core2 -msse4 -mcx16 -mpopcnt -msahf -O0
#GPROF     = -pg
CFLAGS	  = -g ${GPROF} -Wall -Wextra -Wno-packed-bitfield-compat ${OPTIMIZE} -pipe ${INC}
LDFLAGS	  = -g ${GPROF} -Wall -Wextra  ${OPTIMIZE} -pipe ${LIBS}
CPP_SRCS = \
Coordinate2D.cpp \
Font.cpp \
Graphics.cpp \
Mouse.cpp \
OffModel.cpp \
Perspective.cpp \
RayTraceCamera.cpp \
RayTracer.cpp \
Slicer.cpp \
Sprite.cpp \
TextSprite.cpp \
Texture.cpp \
TransformGroup.cpp \
ViewPort.cpp 
OBJ	  = ${CPP_SRCS:.cpp=.o}
DEPFILE = .depend
DEPEXIST  = $(strip $(wildcard $(DEPFILE)))

.PHONY:	dep tags clean

all:	dep ${OBJ}

%.o: %.cpp 
	${CXX} ${CFLAGS} -c -o $@ "$<"

clean:
	rm -f  *.o ${DEPFILE} 

tags:
	ctags *.[hc] ../include/*.h /usr/include/*.h /usr/include/{sys,linux,bits,net}/*.h
	etags *.[hc] ../include/*.h /usr/include/*.h /usr/include/{sys,linux,bits,net}/*.h

dep:	${DEPFILE}

${DEPFILE}: ${CPP_SRCS} 
	${CC} ${CFLAGS} -M ${CPP_SRCS} >  $@

ifeq "$(DEPEXIST)" "$(DEPFILE)"
include	${DEPFILE}
endif

# vi: set ai ts=8 sw=4 tw=90: 
