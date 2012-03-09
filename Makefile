# Copyright (c) 2011i-2012, Sebastian Koch <seb at cs.tu-berlin.de> and
#		      Stefan Sydow <stsydow at cs.tu-berlin.de>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this list
# of conditions and the following disclaimer.
# 
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or other
# materials provided with the distribution.
# 
# Neither the name of the TU-Berlin nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific prior
# written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

OPTIMIZE = -march=core2 -msse4 -mcx16 -mpopcnt -msahf -O0 -g
LIBS      = -lSDLmain -lSDL -lGL -lGLU -lfreetype
INC       = -I/usr/include/freetype2 
#GPROF     = -pg
CFLAGS	  = ${GPROF} -Wall -Wextra -Wno-packed-bitfield-compat ${OPTIMIZE} -pipe ${INC}
LDFLAGS	  = ${GPROF} -Wall -Wextra  ${OPTIMIZE} -pipe ${LIBS}
CPP_SRCS  = main.cpp 
OBJ	  = ${CPP_SRCS:.cpp=.o}
SUBDIRS   = Data Graphics
DEPFILE   = .depend
ALL_SRC  = $(shell find $(SUBDIRS) -name '*.cpp')
ALL_OBJ   = ${ALL_SRC:.cpp=.o}
DEPEXIST  = $(strip $(wildcard $(DEPFILE)))

.PHONY: dep all clean

all: build rayTrace

%.o: %.cpp 
	${CXX} ${CFLAGS} -c -o $@ "$<"

rayTrace: main.o ${ALL_OBJ}
	${CXX} ${LDFLAGS} -o $@ $+ -lm

build:
	@echo "Building in /..."
	@for dir in $(SUBDIRS); do \
		if [ -e $$dir/Makefile ]; then \
			echo "Compiling in $$dir:"; \
			$(MAKE) -C $$dir all;\
			s=$$?; \
			if [ $$s -ne 0 ]; then \
			exit $$s; \
			fi; \
		fi; \
	done

clean:  
	@echo "Cleanup in /..."
	@for dir in $(SUBDIRS); do \
		if [ -e $$dir/Makefile ]; then \
			echo "Cleaning in src/$$dir:"; \
			$(MAKE) -C $$dir clean; \
			s=$$?; \
			if [ $$s -ne 0 ]; then \
				exit $$s; \
			fi; \
		fi; \
	done


install: build
	@echo "Install in /..."
	@for dir in $(SUBDIRS); do \
		if [ -e $$dir/Makefile ]; then \
			echo "Install in src/$$dir:"; \
			$(MAKE) -C $$dir install; \
			s=$$?; \
			if [ $$s -ne 0 ]; then \
				exit $$s; \
			fi; \
		fi; \
	done

dep:	${DEPFILE}

${DEPFILE}: ${CPP_SRCS} 
	${CC} ${CFLAGS} -M ${CPP_SRCS} >  $@

ifeq "$(DEPEXIST)" "$(DEPFILE)"
include	${DEPFILE}
endif

# vi: set ai ts=8 sw=4 tw=90: 
