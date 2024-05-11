CC = clang
OUTPUT = build
NAME = test
OS = LINUX

SRC-DIR = src
DEPS-DIR = $(SRC-DIR)/deps
INCLUDE = -I"include" -I"." -I"deps"
FLAGS = -std=c99 -Wall -Wextra -Wpedantic
MAC_GEN_APP = false


ifeq ($(OS),WINDOWS)
	EXE = $(OUTPUT)/$(NAME).exe
	LIBS = -luser32 -lkernel32 -lgdi32 -lopengl32 -luuid -lole32 -lcomctl32
	DEPS-SRC = $(notdir $(wildcard $(DEPS-DIR)/*.c))

else ifeq ($(OS),MAC)
	EXE = $(OUTPUT)/$(NAME)
	LIBS = -framework Cocoa -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -framework IOKit
	DEPS-SRC = $(notdir $(wildcard $(DEPS-DIR)/*.c /$(DEPS-DIR)/mac/*.c))

else
	EXE = $(OUTPUT)/$(NAME)
	LIBS = -lX11 -lXrandr -lGL -lm
	DEPS-SRC = $(notdir $(wildcard $(DEPS-DIR)/*.c))
endif

# do not edit this
SRC-FILES = $(notdir $(wildcard $(SRC-DIR)/*.c))
SRC-OBJ = $(addprefix $(OUTPUT)/, $(SRC-FILES:.c=.o))
DEPS-OBJ = $(addprefix $(OUTPUT)/, $(DEPS-SRC:.c=.o))

# 'make'
all: $(OUTPUT) $(EXE) run

# Run the exe.
run: $(EXE)
ifeq ($(MAC_GEN_APP), true)
	make generateApp
	open $(OUTPUT)/$(NAME).app
else
	./$(EXE)
endif

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


$(EXE): $(DEPS-OBJ) $(SRC-OBJ)
	$(CC) $(FLAGS) $^ $(LIBS) -o $@
ifeq ($(OS),MAC)
#make generateApp
endif

$(OUTPUT)/%.o: $(SRC-DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDE) -c $^ -o $(OUTPUT)/$(notdir $@)

$(OUTPUT)/%.o: %.h
	$(CC) $(FLAGS) $(INCLUDE) -c $(DEPS-DIR)/$(basename $(notdir $^)).c -o $(OUTPUT)/$(notdir $@)
$(OUTPUT)/%.o: include/%.h
	$(CC) $(FLAGS) $(INCLUDE) -c $(DEPS-DIR)/$(basename $(notdir $^)).c -o $(OUTPUT)/$(notdir $@)
$(OUTPUT)/%.o: include/sili/%.h
	$(CC) $(FLAGS) $(INCLUDE) -c $(DEPS-DIR)/$(basename $(notdir $^)).c -o $(OUTPUT)/$(notdir $@)
$(OUTPUT)/%.o: include/stb/%.h
	$(CC) $(FLAGS) $(INCLUDE) -c $(DEPS-DIR)/$(basename $(notdir $^)).c -o $(OUTPUT)/$(notdir $@)

# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)

# App generator settings. Apart from ICON, you shouldn't change anything.
ICON=

# Changes depending on the targetted platform.
APP_ROOT_PATH=$(EXE).app/Contents
APP_EXE_PATH=MacOS
APP_RES_PATH=Resources

generateApp: $(EXE)
	@rm -rf $(EXE).app
	@echo "Creating $(NAME).app"
	@mkdir -p $(EXE).app $(APP_ROOT_PATH)/$(APP_EXE_PATH) $(APP_ROOT_PATH)/$(APP_RES_PATH)
	@cp $(EXE) $(APP_ROOT_PATH)/$(APP_EXE_PATH)/$(NAME)

ifeq ($(ICON),) # Makefile is STILL dum with tabs.
else
	@sips -z 512 512   $(ICON) --out $(APP_ROOT_PATH)$(APP_RES_PATH)/app.png

#	@mkdir -p "app.iconset"
#	sips -z 512 512   $(ICON) --out app.iconset/icon_512x512.png
#	iconutil -c icns -o $(APP_ROOT_PATH)$(APP_RES_PATH)/app.icns app.iconset
#	@rm -rf app.iconset
endif
	@echo "Writing Info.plist to $(APP_ROOT_PATH)"
	@printf '\
	<?xml version="1.0" encoding="UTF-8"?>									\n\
	<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">	\n\
	<plist version="1.0">											\n\
	<dict>													\n\
		<key>CFBundleName</key>										\n\
		<string>$(NAME)</string>									\n\
														\n\
		<key>CFBundleDisplayName</key>									\n\
		<string>$(NAME)</string>									\n\
														\n\
		<key>CFBundleExecutable</key>									\n\
		<string>$(NAME)</string>										\n\
														\n\
		<key>CFBundleIdentifier</key>									\n\
		<string>com.$(NAME).silicon</string>								\n\
														\n\
		<key>CFBundleShortVersionString</key>								\n\
		<string>1.0.0</string>										\n\
														\n\
		<key>CFBundleVersion</key>									\n\
		<string>1</string>										\n\
														\n\
		<key>CFBundleIconFile</key>									\n\
		<string>app</string>										\n\
														\n\
		<key>LSRequiresIPhoneOS</key>									\n\
		<false/>										\n\
	</dict>													\n\
	</plist>' > $(APP_ROOT_PATH)/Info.plist

	@touch $(EXE).app
