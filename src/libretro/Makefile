STATIC_LINKING := 0
AR             := ar

ifeq ($(platform),)
   platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
endif
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
	EXE_EXT = .exe
	system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	system_platform = osx
	arch = intel
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	system_platform = win
endif

TARGET_NAME := m2000
LIBM		= -lm

ifeq ($(STATIC_LINKING), 1)
EXT := a
endif

ifneq (,$(findstring unix,$(platform)))
   EXT ?= so
   TARGET := $(TARGET_NAME)_libretro.$(EXT)
   SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined
else ifneq (,$(findstring osx,$(platform)))
   ifeq ($(CROSS_COMPILE),1)
      TARGET_RULE   = -target $(LIBRETRO_APPLE_PLATFORM) -isysroot $(LIBRETRO_APPLE_ISYSROOT)
      CFLAGS   += $(TARGET_RULE)
      CPPFLAGS += $(TARGET_RULE)
      CXXFLAGS += $(TARGET_RULE)
      LDFLAGS  += $(TARGET_RULE)
   endif
   TARGET := $(TARGET_NAME)_libretro.dylib
   SHARED := -dynamiclib
else ifeq ($(platform),ios-arm64)
   ifeq ($(IOSSDK),)
      IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
   endif
   CFLAGS   += -arch arm64 -isysroot $(IOSSDK)
   CXXFLAGS += -arch arm64 -isysroot $(IOSSDK)
   LDFLAGS  += -arch arm64 -isysroot $(IOSSDK)
   TARGET := $(TARGET_NAME)_libretro_ios.dylib
   SHARED := -dynamiclib
else ifeq ($(platform),tvos-arm64)
   ifeq ($(IOSSDK),)
      IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
   endif
   CFLAGS   += -arch arm64 -isysroot $(IOSSDK)
   CXXFLAGS += -arch arm64 -isysroot $(IOSSDK)
   LDFLAGS  += -arch arm64 -isysroot $(IOSSDK)
   TARGET := $(TARGET_NAME)_libretro_tvos.dylib
   SHARED := -dynamiclib
else
   CC = gcc
   TARGET := $(TARGET_NAME)_libretro.dll
   SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=link.T -Wl,--no-undefined
endif

LDFLAGS += $(LIBM)

ifeq ($(DEBUG), 1)
   CFLAGS += -O0 -g
else
   CFLAGS += -O3
endif

VPATH = ../
OBJECTS := P2000.o Z80.o m2000_libretro.o
CFLAGS += -I./libretro-common/include -Wall -std=gnu99 -fPIC

all: clean $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CC) -fPIC $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) $(LDFLAGS)
endif

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
