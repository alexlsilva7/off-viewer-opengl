#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mesh.h"

// Constantes
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MAX_FILENAME_LENGTH 256
#define TRUE 1
#define FALSE 0

// Estrutura para controlar a câmera
typedef struct {
    float distance;      // Distância do objeto
    float pan_x;         // Translação em X
    float pan_y;         // Translação em Y
} Camera;

// Enumeração para modos de renderização
typedef enum {
    RENDER_SOLID = 0,
    RENDER_WIREFRAME,
    RENDER_POINTS
} RenderMode;

// Variáveis globais
Mesh myMesh;
Camera camera = {5.0f, 0.0f, 0.0f};
Camera initial_camera = {5.0f, 0.0f, 0.0f};
RenderMode render_mode = RENDER_SOLID;
char filename[MAX_FILENAME_LENGTH] = "";
int show_hud = TRUE;
int show_help = FALSE;

// Variáveis para controle do mouse
int mouse_left_pressed = FALSE;
int mouse_right_pressed = FALSE;
int last_mouse_x = 0;
int last_mouse_y = 0;

// Variáveis para UI
int fps_counter = 0;
int fps_display = 0;
int fps_timer = 0;

// Cores fixas
float bg_color[3] = {0.1f, 0.1f, 0.15f};
float object_color[3] = {0.8f, 0.8f, 0.9f};
float text_color[3] = {1.0f, 1.0f, 1.0f};
float accent_color[3] = {0.3f, 0.7f, 1.0f};

// Protótipos das funções
void init_opengl();
void setup_lighting();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special_keys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse_motion(int x, int y);
void draw_hud();
void draw_help_menu();
void draw_mesh();
void draw_status_bar();
void reset_camera();
void render_text(float x, float y, const char* text);
void render_text_centered(float x, float y, const char* text);
void calculate_fps();
void draw_rounded_rect(float x, float y, float width, float height, float alpha);

// Função para inicializar OpenGL
void init_opengl() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Configurações de qualidade
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    // Configurar cor de fundo
    glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0f);
    
    // Configurar iluminação
    setup_lighting();
}

// Função para configurar iluminação
void setup_lighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Posição da luz
    float light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};
    float light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Propriedades do material
    float mat_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float mat_diffuse[] = {object_color[0], object_color[1], object_color[2], 1.0f};
    float mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float mat_shininess = 32.0f;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


// Função de renderização principal
void display() {
    // Calcular FPS
    calculate_fps();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Aplicar transformações da câmera
    glTranslatef(camera.pan_x, camera.pan_y, -camera.distance);
    
    // Desenhar a malha
    draw_mesh();
    
    // Desenhar barra de status
    draw_status_bar();
    
    // Desenhar HUD se habilitado
    if (show_hud) {
        draw_hud();
    }
    
    // Desenhar menu de ajuda se habilitado
    if (show_help) {
        draw_help_menu();
    }
    
    glutSwapBuffers();
}

// Função para desenhar a malha
void draw_mesh() {
    if (!myMesh.vertices || !myMesh.faces) {
        return;
    }
    
    // Configurar modo de renderização
    switch (render_mode) {
        case RENDER_WIREFRAME:
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(object_color[0], object_color[1], object_color[2]);
            break;
            
        case RENDER_POINTS:
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glPointSize(3.0f);
            glColor3f(object_color[0], object_color[1], object_color[2]);
            break;
            
        case RENDER_SOLID:
        default:
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
    
    // Renderizar faces
    for (int i = 0; i < myMesh.num_faces; i++) {
        Face* face = &myMesh.faces[i];
        
        if (face->num_vertices == 3) {
            // Triângulo
            glBegin(GL_TRIANGLES);
            
            // Calcular normal da face para iluminação
            if (render_mode == RENDER_SOLID) {
                Vertex* v0 = &myMesh.vertices[face->vertex_indices[0]];
                Vertex* v1 = &myMesh.vertices[face->vertex_indices[1]];
                Vertex* v2 = &myMesh.vertices[face->vertex_indices[2]];
                
                // Vetores das arestas
                float edge1[3] = {v1->x - v0->x, v1->y - v0->y, v1->z - v0->z};
                float edge2[3] = {v2->x - v0->x, v2->y - v0->y, v2->z - v0->z};
                
                // Produto cruzado para normal
                float normal[3];
                normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
                normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
                normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
                
                // Normalizar
                float length = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
                if (length > 0.0f) {
                    normal[0] /= length;
                    normal[1] /= length;
                    normal[2] /= length;
                }
                
                glNormal3f(normal[0], normal[1], normal[2]);
            }
            
            for (int j = 0; j < 3; j++) {
                Vertex* v = &myMesh.vertices[face->vertex_indices[j]];
                glVertex3f(v->x, v->y, v->z);
            }
            glEnd();
            
        } else if (face->num_vertices == 4) {
            // Quadrilátero - dividir em dois triângulos
            glBegin(GL_TRIANGLES);
            
            // Primeiro triângulo (0, 1, 2)
            if (render_mode == RENDER_SOLID) {
                Vertex* v0 = &myMesh.vertices[face->vertex_indices[0]];
                Vertex* v1 = &myMesh.vertices[face->vertex_indices[1]];
                Vertex* v2 = &myMesh.vertices[face->vertex_indices[2]];
                
                float edge1[3] = {v1->x - v0->x, v1->y - v0->y, v1->z - v0->z};
                float edge2[3] = {v2->x - v0->x, v2->y - v0->y, v2->z - v0->z};
                
                float normal[3];
                normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
                normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
                normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
                
                float length = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
                if (length > 0.0f) {
                    normal[0] /= length;
                    normal[1] /= length;
                    normal[2] /= length;
                }
                
                glNormal3f(normal[0], normal[1], normal[2]);
            }
            
            for (int j = 0; j < 3; j++) {
                Vertex* v = &myMesh.vertices[face->vertex_indices[j]];
                glVertex3f(v->x, v->y, v->z);
            }
            
            // Segundo triângulo (0, 2, 3)
            glVertex3f(myMesh.vertices[face->vertex_indices[0]].x, 
                      myMesh.vertices[face->vertex_indices[0]].y, 
                      myMesh.vertices[face->vertex_indices[0]].z);
            glVertex3f(myMesh.vertices[face->vertex_indices[2]].x, 
                      myMesh.vertices[face->vertex_indices[2]].y, 
                      myMesh.vertices[face->vertex_indices[2]].z);
            glVertex3f(myMesh.vertices[face->vertex_indices[3]].x, 
                      myMesh.vertices[face->vertex_indices[3]].y, 
                      myMesh.vertices[face->vertex_indices[3]].z);
            
            glEnd();
        }
    }
    
    // Restaurar configurações
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Função para desenhar o HUD
void draw_hud() {
    // Salvar estado atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Fundo semi-transparente para o HUD com padding superior
    float hud_top = WINDOW_HEIGHT - 170.0f;
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, hud_top);
    glVertex2f(300, hud_top);
    glVertex2f(300, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    
    // Borda colorida
    glColor4f(accent_color[0], accent_color[1], accent_color[2], 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, hud_top);
    glVertex2f(300, hud_top);
    glVertex2f(300, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    
    glColor3f(text_color[0], text_color[1], text_color[2]);
    
    // Informações do modelo com padding
    char info[512];
    snprintf(info, sizeof(info), "Arquivo: %s", filename);
    render_text(10.0f, WINDOW_HEIGHT - 155.0f, info);
    
    snprintf(info, sizeof(info), "Vertices: %d  Faces: %d", myMesh.num_vertices, myMesh.num_faces);
    render_text(10.0f, WINDOW_HEIGHT - 135.0f, info);
    
    // Modo de renderização
    const char* mode_names[] = {"Solido", "Wireframe", "Pontos"};
    snprintf(info, sizeof(info), "Modo: %s", mode_names[render_mode]);
    render_text(10.0f, WINDOW_HEIGHT - 115.0f, info);
    
    // Controles básicos
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text(10.0f, WINDOW_HEIGHT - 90.0f, "Controles:");
    
    glColor3f(text_color[0], text_color[1], text_color[2]);
    render_text(10.0f, WINDOW_HEIGHT - 70.0f, "H: HUD  F1: Ajuda");
    render_text(10.0f, WINDOW_HEIGHT - 50.0f, "W/S/P: Modo  R: Reset");
    render_text(10.0f, WINDOW_HEIGHT - 30.0f, "ESC: Sair");
    
    glDisable(GL_BLEND);
    
    // Restaurar estado
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Função para renderizar texto (usando GLUT)
void render_text(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

// Função para redimensionamento da janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    gluPerspective(60.0, aspect, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

// Função para controle do teclado
void keyboard(unsigned char key, int x, int y) {
    (void)x; // Suprimir warning de parâmetro não utilizado
    (void)y; // Suprimir warning de parâmetro não utilizado
    switch (key) {
        case 27: // ESC
            free_mesh(&myMesh);
            exit(0);
            break;
            
        case 'w':
        case 'W':
            render_mode = RENDER_WIREFRAME;
            break;
            
        case 's':
        case 'S':
            render_mode = RENDER_SOLID;
            break;
            
        case 'p':
        case 'P':
            render_mode = RENDER_POINTS;
            break;
            
        case 'r':
        case 'R':
            reset_camera();
            break;
            
        case 'h':
        case 'H':
            show_hud = !show_hud;
            break;
            
    }
    
    glutPostRedisplay();
}

// Função para controle de teclas especiais
void special_keys(int key, int x, int y) {
    (void)x; // Suprimir warning de parâmetro não utilizado
    (void)y; // Suprimir warning de parâmetro não utilizado
    
    switch (key) {
        case GLUT_KEY_F1:
            show_help = !show_help;
            break;
    }
    
    glutPostRedisplay();
}

// Função para controle do mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouse_left_pressed = (state == GLUT_DOWN);
        if (mouse_left_pressed) {
            last_mouse_x = x;
            last_mouse_y = y;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        mouse_right_pressed = (state == GLUT_DOWN);
        if (mouse_right_pressed) {
            last_mouse_x = x;
            last_mouse_y = y;
        }
    } else if (button == 3 && state == GLUT_UP) { // Scroll up
        camera.distance *= 0.9f;
        if (camera.distance < 0.1f) camera.distance = 0.1f;
    } else if (button == 4 && state == GLUT_UP) { // Scroll down
        camera.distance *= 1.1f;
        if (camera.distance > 50.0f) camera.distance = 50.0f;
    }
    
    glutPostRedisplay();
}

// Função para movimento do mouse
void mouse_motion(int x, int y) {
    int dx = x - last_mouse_x;
    int dy = y - last_mouse_y;
    
    if (mouse_left_pressed || mouse_right_pressed) {
        // Pan (translação)
        float sensitivity = 0.001f * camera.distance;
        camera.pan_x += dx * sensitivity;
        camera.pan_y -= dy * sensitivity; // Inverter Y para movimento mais natural
    }
    
    last_mouse_x = x;
    last_mouse_y = y;
    
    glutPostRedisplay();
}

// Função para resetar a câmera
void reset_camera() {
    camera = initial_camera;
}


// Função para calcular FPS
void calculate_fps() {
    fps_counter++;
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    
    if (current_time - fps_timer >= 1000) {
        fps_display = fps_counter;
        fps_counter = 0;
        fps_timer = current_time;
    }
}

// Função para desenhar retângulo com cantos arredondados (simplificado)
void draw_rounded_rect(float x, float y, float width, float height, float alpha) {
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, alpha * 0.7f);
    
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Bordas com cor de destaque
    glColor4f(accent_color[0], accent_color[1], accent_color[2], alpha);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    glDisable(GL_BLEND);
}

// Função para renderizar texto centralizado
void render_text_centered(float x, float y, const char* text) {
    int len = strlen(text);
    float text_width = len * 8.0f; // Aproximação para GLUT_BITMAP_8_BY_13
    glRasterPos2f(x - text_width / 2.0f, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

// Função para desenhar barra de status moderna
void draw_status_bar() {
    // Salvar estado atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Desenhar barra de status no topo
    float bar_height = 30.0f;
    draw_rounded_rect(10.0f, 10.0f, WINDOW_WIDTH - 20.0f, bar_height, 0.8f);
    
    // Informações na barra de status
    glColor3f(text_color[0], text_color[1], text_color[2]);
    
    char status_text[256];
    snprintf(status_text, sizeof(status_text), "FPS: %d | Vertices: %d | Faces: %d | Modo: %s",
             fps_display, myMesh.num_vertices, myMesh.num_faces,
             (render_mode == RENDER_SOLID) ? "Solido" :
             (render_mode == RENDER_WIREFRAME) ? "Wireframe" : "Pontos");
    
    render_text(20.0f, 28.0f, status_text);
    
    
    // Restaurar estado
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Função para desenhar menu de ajuda interativo
void draw_help_menu() {
    // Salvar estado atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Fundo semi-transparente
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    
    // Janela de ajuda centralizada
    float help_width = 480.0f;
    float help_height = 350.0f;
    float help_x = (WINDOW_WIDTH - help_width) / 2.0f;
    float help_y = (WINDOW_HEIGHT - help_height) / 2.0f;
    
    draw_rounded_rect(help_x, help_y, help_width, help_height, 1.0f);
    
    // Título principal
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text_centered(WINDOW_WIDTH / 2.0f, help_y + 25.0f, "=== AJUDA - VISUALIZADOR OFF ===");
    
    // Conteúdo da ajuda
    glColor3f(text_color[0], text_color[1], text_color[2]);
    float line_height = 18.0f;
    float start_y = help_y + 50.0f;
    float col1_x = help_x + 20.0f;
    float col2_x = help_x + 250.0f;
    
    // Coluna 1 - Controles do Mouse
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text(col1_x, start_y, "CONTROLES DO MOUSE:");
    glColor3f(text_color[0], text_color[1], text_color[2]);
    render_text(col1_x + 10.0f, start_y + line_height * 1.5f, "Arrastar com qualquer botao:");
    render_text(col1_x + 20.0f, start_y + line_height * 2.5f, "- Move o objeto (Pan)");
    render_text(col1_x + 10.0f, start_y + line_height * 4.0f, "Roda do mouse:");
    render_text(col1_x + 20.0f, start_y + line_height * 5.0f, "- Zoom In/Out");
    
    // Coluna 2 - Controles do Teclado
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text(col2_x, start_y, "CONTROLES DO TECLADO:");
    glColor3f(text_color[0], text_color[1], text_color[2]);
    render_text(col2_x + 10.0f, start_y + line_height * 1.5f, "W - Modo Wireframe");
    render_text(col2_x + 10.0f, start_y + line_height * 2.5f, "S - Modo Solido");
    render_text(col2_x + 10.0f, start_y + line_height * 3.5f, "P - Modo Pontos");
    render_text(col2_x + 10.0f, start_y + line_height * 5.0f, "R - Reset Camera");
    render_text(col2_x + 10.0f, start_y + line_height * 6.0f, "H - Mostrar/Ocultar HUD");
    render_text(col2_x + 10.0f, start_y + line_height * 7.0f, "F1 - Esta Ajuda");
    render_text(col2_x + 10.0f, start_y + line_height * 8.0f, "ESC - Sair do programa");
    
    // Seção de informações adicionais
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text(col1_x, start_y + line_height * 10.0f, "SOBRE O PROGRAMA:");
    glColor3f(text_color[0], text_color[1], text_color[2]);
    render_text(col1_x + 10.0f, start_y + line_height * 11.5f, "Visualizador simplificado de arquivos OFF (Object File Format)");
    render_text(col1_x + 10.0f, start_y + line_height * 12.5f, "Suporta malhas triangulares e quadrilaterais");
    
    // Instrução para fechar (destacada)
    glColor3f(accent_color[0], accent_color[1], accent_color[2]);
    render_text_centered(WINDOW_WIDTH / 2.0f, help_y + help_height - 25.0f, ">>> Pressione F1 novamente para fechar <<<");
    
    glDisable(GL_BLEND);
    
    // Restaurar estado
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Função principal
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_off>\n", argv[0]);
        return 1;
    }
    
    // Salvar nome do arquivo
    strncpy(filename, argv[1], MAX_FILENAME_LENGTH - 1);
    filename[MAX_FILENAME_LENGTH - 1] = '\0';
    
    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Visualizador OFF");
    
    // Inicializar OpenGL
    init_opengl();
    
    // Carregar arquivo OFF
    printf("Carregando arquivo OFF: %s\n", argv[1]);
    if (read_off_file(argv[1], &myMesh) != 0) {
        fprintf(stderr, "Erro ao carregar arquivo OFF: %s\n", argv[1]);
        return 1;
    }
    
    printf("Malha carregada com sucesso: %d vertices, %d faces.\n", 
           myMesh.num_vertices, myMesh.num_faces);
    
    
    // Configurar callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_motion);
    
    // Iniciar loop principal
    glutMainLoop();
    
    // Limpeza (nunca será executado devido ao glutMainLoop)
    free_mesh(&myMesh);
    
    return 0;
}