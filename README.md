# Visualizador OFF Simplificado

Um visualizador 3D simplificado para arquivos no formato OFF (Object File Format) desenvolvido em C com OpenGL/GLUT.

## Características

### 🎮 Controles Interativos
- **Pan (Movimento)**: Clique e arraste com qualquer botão do mouse
- **Zoom**: Use a roda do mouse (scroll)
- **Reset da Câmera**: Pressione `R` para voltar à posição inicial

### 🖼️ Modos de Visualização
- **Sólido** (`S`): Renderização com iluminação e sombreamento
- **Wireframe** (`W`): Visualização das arestas do modelo
- **Pontos** (`P`): Exibição apenas dos vértices

### ✨ Interface de Usuário Limpa
- **Barra de Status**: Exibe FPS, estatísticas do modelo e modo de renderização
- **HUD Informativo**: Interface simples com informações essenciais
- **Menu de Ajuda Completo**: Pressione `F1` para exibir controles detalhados
- **Contador de FPS**: Monitoramento de performance em tempo real

## Requisitos do Sistema

### Dependências
- **OpenGL**: Biblioteca gráfica 3D
- **GLUT**: Toolkit para interface gráfica
- **GCC**: Compilador C (com suporte a C99)

### Instalação das Dependências (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev build-essential
```

## Compilação

### Usando o Makefile (Recomendado)
```bash
# Compilar o projeto
make

# Ou compilar com informações de debug
make debug-build

# Ou compilar versão otimizada
make release
```

### Compilação Manual
```bash
gcc -Wall -Wextra -std=c99 -O2 main.c mesh.c -o off_viewer -lglut -lGL -lGLU -lm
```

## Execução

### Usando o Makefile
```bash
# Executar com arquivo específico
make run FILE=caminho/para/arquivo.off

# Executar com arquivos de exemplo
make run-triangles  # Usa ../triangles.off
make run-hand      # Usa ../hand-hybrid.off
```

### Execução Direta
```bash
./off_viewer caminho/para/arquivo.off
```

## Comandos do Makefile

| Comando | Descrição |
|---------|-----------|
| `make` | Compila o projeto |
| `make clean` | Remove arquivos de compilação |
| `make install-deps` | Instala dependências (Ubuntu/Debian) |
| `make run FILE=arquivo.off` | Executa com arquivo específico |
| `make run-triangles` | Executa com triangles.off |
| `make run-hand` | Executa com hand-hybrid.off |
| `make debug FILE=arquivo.off` | Executa com debugger (gdb) |
| `make memcheck FILE=arquivo.off` | Verifica vazamentos de memória |
| `make debug-build` | Compila versão de debug |
| `make release` | Compila versão otimizada |
| `make info` | Mostra informações do projeto |

## Controles Detalhados

### Teclado
| Tecla | Função |
|-------|--------|
| `ESC` | Sair do programa |
| `W` | Modo Wireframe |
| `S` | Modo Sólido |
| `P` | Modo Pontos |
| `R` | Reset da câmera |
| `H` | Toggle do HUD |
| `F1` | Menu de ajuda interativo |

### Mouse
| Ação | Função |
|------|--------|
| Qualquer botão + arraste | Mover objeto (pan) |
| Roda do mouse | Zoom in/out |

## Formato de Arquivo OFF

O visualizador suporta arquivos no formato OFF padrão:
```
OFF
num_vertices num_faces num_edges
x1 y1 z1
x2 y2 z2
...
3 v1 v2 v3
4 v1 v2 v3 v4
...
```

### Características Suportadas
- ✅ Faces triangulares (3 vértices)
- ✅ Faces quadrilaterais (4 vértices) - automaticamente trianguladas
- ✅ Coordenadas de vértices em ponto flutuante
- ✅ Cálculo automático de normais para iluminação

## Características da Versão Simplificada

### Simplificações Realizadas
1. **Controles Simplificados**
   - Removida rotação 3D complexa
   - Mantido apenas pan (movimento) e zoom
   - Interface mais intuitiva

2. **Código Limpo**
   - Removido sistema de temas múltiplos
   - Removidas animações e transições
   - Código mais focado e legível

3. **Interface Direta**
   - HUD com informações essenciais
   - Menu de ajuda reorganizado
   - Controles claros e diretos

4. **Mantidas Funcionalidades Essenciais**
   - Três modos de renderização
   - Iluminação e sombreamento
   - Suporte a malhas triangulares e quadrilaterais
   - Contador de FPS

## Solução de Problemas

### Erro de Compilação
```bash
# Verificar dependências
make check-deps

# Instalar dependências
make install-deps
```

### Problemas de Performance
```bash
# Compilar versão otimizada
make release
./off_viewer_release arquivo.off
```

### Debug e Análise
```bash
# Executar com debugger
make debug FILE=arquivo.off

# Verificar vazamentos de memória
make memcheck FILE=arquivo.off
```

## Arquitetura do Código

### Estruturas Principais
- `Camera`: Controla posição, movimento (pan) e zoom
- `Mesh`: Armazena vértices e faces do modelo
- `RenderMode`: Enumera modos de renderização

### Funções Principais
- `display()`: Loop de renderização principal
- `draw_mesh()`: Renderização da malha 3D
- `draw_hud()`: Interface do usuário
- `mouse_motion()`: Controles de mouse
- `keyboard()`: Controles de teclado

## Contribuição

Para contribuir com o projeto:
1. Faça um fork do repositório
2. Crie uma branch para sua feature
3. Teste suas modificações
4. Submeta um pull request

## Licença

Este projeto é de domínio público e pode ser usado livremente para fins educacionais e comerciais.