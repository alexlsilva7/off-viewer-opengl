# Visualizador OFF Melhorado

Um visualizador 3D interativo para arquivos no formato OFF (Object File Format) desenvolvido em C com OpenGL/GLUT.

## Características

### 🎮 Controles Interativos
- **Rotação**: Clique e arraste com o botão esquerdo do mouse
- **Pan (Translação)**: Clique e arraste com o botão direito do mouse
- **Zoom**: Use a roda do mouse (scroll)
- **Reset da Câmera**: Pressione `R` para voltar à posição inicial

### 🖼️ Modos de Visualização
- **Sólido** (`S`): Renderização com iluminação e sombreamento
- **Wireframe** (`W`): Visualização das arestas do modelo
- **Pontos** (`P`): Exibição apenas dos vértices

### 🎨 Temas Visuais Modernos
- **4 Temas Pré-definidos**:
  - `1`: Azul Moderno (padrão)
  - `2`: Escuro Elegante
  - `3`: Cinza Profissional
  - `4`: Aquecido
- **Troca de Temas**: Pressione `T` para alternar entre temas
- **Transições Suaves**: Animações fluidas entre cores

### ✨ Interface de Usuário Moderna
- **Barra de Status**: Exibe FPS, estatísticas do modelo e tema atual
- **HUD Animado**: Interface com transparência e animações suaves
- **Menu de Ajuda Interativo**: Pressione `F1` para exibir controles detalhados
- **Feedback Visual**: Bordas coloridas e elementos destacados
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
| `T` | Alternar tema (próximo) |
| `F1` | Menu de ajuda interativo |
| `1` | Tema Azul Moderno |
| `2` | Tema Escuro Elegante |
| `3` | Tema Cinza Profissional |
| `4` | Tema Aquecido |

### Mouse
| Ação | Função |
|------|--------|
| Botão esquerdo + arraste | Rotacionar objeto |
| Botão direito + arraste | Mover objeto (pan) |
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

## Melhorias Implementadas

### Comparado à Versão Original
1. **Controles de Câmera Avançados**
   - Rotação orbit completa
   - Sistema de pan/translação
   - Zoom suave e limitado

2. **Qualidade Visual**
   - Iluminação e sombreamento
   - Múltiplos modos de renderização
   - Cores personalizáveis
   - Anti-aliasing para linhas e pontos

3. **Interface do Usuário**
   - HUD informativo
   - Controles intuitivos
   - Feedback visual em tempo real

4. **Robustez**
   - Tratamento de erros melhorado
   - Gerenciamento de memória seguro
   - Suporte a diferentes tipos de face

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
- `Camera`: Controla posição, rotação e zoom
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