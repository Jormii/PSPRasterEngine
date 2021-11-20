MAIN_OBJ = ../main.o
SOURCE_DIR = ../src/
INCLUDE_DIR = "../include/"

TYPES_DIR = $(SOURCE_DIR)types/
ENGINE_DIR = $(SOURCE_DIR)engine/

TARGET = EnginePSP
OBJS = 	$(TYPES_DIR)vec2f.o $(TYPES_DIR)vec2i.o $(TYPES_DIR)vec3f.o $(TYPES_DIR)vec3i.o $(TYPES_DIR)vec4f.o $(TYPES_DIR)vec4i.o \
		$(TYPES_DIR)mat3f.o $(TYPES_DIR)mat4f.o $(TYPES_DIR)rgba.o $(TYPES_DIR)texture.o $(TYPES_DIR)mesh.o \
		$(ENGINE_DIR)utils.o $(ENGINE_DIR)light.o $(ENGINE_DIR)draw_matrices.o $(ENGINE_DIR)edge.o $(ENGINE_DIR)fragment.o \
		$(ENGINE_DIR)rasterization.o $(ENGINE_DIR)engine.o \
		$(SOURCE_DIR)callbacks.o $(SOURCE_DIR)debug.o \
		$(MAIN_OBJ)

LIBS = -lpspgu -lpspvfpu -lstdc++ -lm

INCDIR = $(INCLUDE_DIR) $(INCLUDE_DIR)types/ $(INCLUDE_DIR)/engine
CFLAGS = -G0 -Wall -O2 -D DEBUG
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = EnginePSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

