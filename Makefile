# Nom de l'exécutable
TARGET = PestiVentura

# Dossiers
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
ASSETS_DIR = assets/fonts

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Détection de l'OS
ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
	SDL_CFLAGS := -IC:/SDL2/include -I$(INCLUDE_DIR)
	SDL_LDFLAGS := -LC:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer
	RM := del /Q
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		SDL_CFLAGS := $(shell sdl2-config --cflags) -I$(INCLUDE_DIR)
		SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_mixer
		RM := rm -rf
	endif
	ifeq ($(UNAME_S),Darwin)
		# Configuration pour macOS
		ifeq ($(shell test -d /opt/homebrew/opt/sdl2 && echo yes),yes)
			SDL_CFLAGS := -I/opt/homebrew/include -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_ttf/include/SDL2 -I/opt/homebrew/opt/sdl2_image/include/SDL2 -I/opt/homebrew/opt/sdl2_mixer/include/SDL2 -I$(INCLUDE_DIR)
			SDL_LDFLAGS := -L/opt/homebrew/lib -L/opt/homebrew/opt/sdl2/lib -L/opt/homebrew/opt/sdl2_ttf/lib -L/opt/homebrew/opt/sdl2_image/lib -L/opt/homebrew/opt/sdl2_mixer/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
		else
			SDL_CFLAGS := -I/usr/local/include/SDL2 -I$(INCLUDE_DIR)
			SDL_LDFLAGS := -L/usr/local/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
		endif
		RM := rm -rf
	endif
endif

# Compilation flags
CFLAGS = $(SDL_CFLAGS)

# Création des dossiers de sortie
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilation de chaque fichier source en objet
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(SDL_LDFLAGS) -o $(BUILD_DIR)/$(TARGET)

# Commande de compilation par défaut
all: $(TARGET)

# Nettoyage des fichiers compilés
clean:
	$(RM) $(BUILD_DIR)

# Exécution
run: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# Phony targets
.PHONY: all clean run
