MAIN_OBJ = ../main_psp.o
SOURCE_DIR = ../src/
INCLUDE_DIR = "../include/"

TYPES_DIR = $(SOURCE_DIR)types/
ENGINE_DIR = $(SOURCE_DIR)engine/

TARGET = EnginePSP
OBJS = 	$(TYPES_DIR)vec2f.o $(TYPES_DIR)vec2i.o $(TYPES_DIR)vec3f.o $(TYPES_DIR)vec3i.o $(TYPES_DIR)vec4f.o \
		$(TYPES_DIR)mat4f.o $(TYPES_DIR)rgba.o $(TYPES_DIR)mesh.o \
		$(ENGINE_DIR)utils.o $(ENGINE_DIR)draw_matrices.o $(ENGINE_DIR)edge.o $(ENGINE_DIR)fragment.o \
		$(ENGINE_DIR)rasterization.o $(ENGINE_DIR)engine.o \
		$(MAIN_OBJ)

LIBS = -lpspgu -lstdc++ -lm

INCDIR = $(INCLUDE_DIR)types/ $(INCLUDE_DIR)/engine
CFLAGS = -G0 -Wall -O2 -D PSP
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = EnginePSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

