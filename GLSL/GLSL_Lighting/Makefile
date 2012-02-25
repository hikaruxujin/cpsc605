#---------------------------------------------------------------------------------
.SUFFIXES:
.SILENT:

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# EXT is the extension of the application (including dot)
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# PACKAGES is a list of packages to link to the 
# CROSS is a target for cross compilation ended with a dash
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
EXT			:=	
BUILD		:=	build
SOURCES		:=	source
PACKAGES	:=	
CROSS		:=	
VERSION		:=	

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ASFLAGS		:=	
CFLAGS		:=	-s -O2 -mmmx -msse -msse2
CXXFLAGS	:=	-fno-rtti -fno-exceptions -fomit-frame-pointer -ffast-math 
LDFLAGS		:=	

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:= -lGL -lGLU -lGLEW -lglut

#---------------------------------------------------------------------------------
# everything is automatic from here on
#---------------------------------------------------------------------------------
CXXFLAGS	+=	$(CFLAGS)
LDFLAGS		+=	$(CFLAGS)
CFLAGS		+=	$(INCLUDE) $(foreach pkg,$(PACKAGES),`pkg-config --cflags $(pkg)`)
LIBS		+=	$(foreach pkg,$(PACKAGES),`pkg-config --libs $(pkg)`)

export AS	:=	$(CROSS)as$(VERSION)
export CC	:=	$(CROSS)gcc$(VERSION)
export CXX	:=	$(CROSS)g++$(VERSION)

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@
	
#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.o: %.s
	@echo $(notdir $<)
	$(AS) --MD $(DEPSDIR)/$*.d $(ASFLAGS) $< -o$@

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)$(EXT)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

ASFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean all Makefile.$(SYSTEM) 

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	$(RM) -rf $(BUILD) $(OUTPUT)

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main target
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)
	@echo linking...
	@$(LD)  $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
	@echo built $(notdir $@)

-include $(DEPENDS)

endif
