# Makefile para o Visualizador OFF
# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -O2

# Bibliotecas OpenGL/GLUT
LIBS = -lglut -lGL -lGLU -lm

# Diretórios
SRC_DIR = .
BUILD_DIR = build

# Arquivos fonte
SOURCES = main.c mesh.c
OBJECTS = $(SOURCES:.c=.o)

# Nome do executável
TARGET = off_viewer

# Regra principal
all: $(TARGET)

# Criar diretório de build se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "✓ Executável $(TARGET) criado com sucesso!"

# Compilar arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f *.o $(TARGET)
	@echo "✓ Arquivos de compilação removidos"

# Limpeza completa
distclean: clean
	rm -rf $(BUILD_DIR)
	@echo "✓ Limpeza completa realizada"

# Instalar dependências (Ubuntu/Debian)
install-deps:
	@echo "Instalando dependências OpenGL/GLUT..."
	sudo apt-get update
	sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

# Executar com arquivo de exemplo
run-triangles: $(TARGET)
	./$(TARGET) ./triangles.off

run-hand: $(TARGET)
	./$(TARGET) ./hand-hybrid.off

# Executar com arquivo personalizado
run: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Uso: make run FILE=caminho/para/arquivo.off"; \
		echo "Ou use: make run-triangles ou make run-hand"; \
	else \
		./$(TARGET) $(FILE); \
	fi

# Debugar com gdb
debug: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Uso: make debug FILE=caminho/para/arquivo.off"; \
	else \
		gdb --args ./$(TARGET) $(FILE); \
	fi

# Verificar vazamentos de memória com valgrind
memcheck: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Uso: make memcheck FILE=caminho/para/arquivo.off"; \
	else \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) $(FILE); \
	fi

# Mostrar informações sobre o sistema
info:
	@echo "=== Informações do Sistema ==="
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Bibliotecas: $(LIBS)"
	@echo "Arquivos fonte: $(SOURCES)"
	@echo "Executável: $(TARGET)"
	@echo ""
	@echo "=== Comandos Disponíveis ==="
	@echo "make            - Compilar o projeto"
	@echo "make clean      - Limpar arquivos de compilação"
	@echo "make install-deps - Instalar dependências"
	@echo "make run FILE=arquivo.off - Executar com arquivo específico"
	@echo "make run-triangles - Executar com triangles.off"
	@echo "make run-hand   - Executar com hand-hybrid.off"
	@echo "make debug FILE=arquivo.off - Debugar com gdb"
	@echo "make memcheck FILE=arquivo.off - Verificar vazamentos"

# Compilação com debug
debug-build:
	$(CC) $(CFLAGS) -g -DDEBUG $(SOURCES) -o $(TARGET)_debug $(LIBS)
	@echo "✓ Executável de debug criado: $(TARGET)_debug"

# Compilação otimizada para release
release:
	$(CC) $(CFLAGS) -O3 -DNDEBUG $(SOURCES) -o $(TARGET)_release $(LIBS)
	strip $(TARGET)_release
	@echo "✓ Executável otimizado criado: $(TARGET)_release"

# Verificar se as dependências estão instaladas
check-deps:
	@echo "Verificando dependências..."
	@pkg-config --exists gl glu || (echo "❌ OpenGL não encontrado" && exit 1)
	@pkg-config --exists glut || (echo "❌ GLUT não encontrado" && exit 1)
	@echo "✓ Todas as dependências estão instaladas"

# Ajuda
help: info

# Declarar alvos que não são arquivos
.PHONY: all clean distclean install-deps run run-triangles run-hand debug memcheck info debug-build release check-deps help