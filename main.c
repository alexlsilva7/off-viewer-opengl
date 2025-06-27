#include <stdio.h>
#include <string.h>
#include "mesh.h"

int main() {
    Mesh my_mesh;
    // Inicializa a malha para evitar lixo de memória
    my_mesh.num_vertices = 0;
    my_mesh.vertices = NULL;
    my_mesh.num_faces = 0;
    my_mesh.faces = NULL;

    char filename[256];
    int choice;

    printf("Escolha um arquivo OFF para carregar:\n");
    printf("1. hand-hybrid.off\n");
    printf("2. triangles.off\n");
    printf("Digite sua escolha (1 ou 2): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strcpy(filename, "hand-hybrid.off");
            break;
        case 2:
            strcpy(filename, "triangles.off");
            break;
        default:
            fprintf(stderr, "Escolha inválida. Saindo.\n");
            return 1;
    }

    printf("Tentando ler o arquivo OFF: %s\n", filename);
    if (read_off_file(filename, &my_mesh) == 0) {
        printf("Arquivo OFF lido com sucesso!\n");
        printf("Número de vértices: %d\n", my_mesh.num_vertices);
        printf("Número de faces: %d\n", my_mesh.num_faces);

        printf("\nDetalhes dos Vértices (primeiros 5 e últimos 5, se houver):\n");
        int display_count = 0;
        for (int i = 0; i < my_mesh.num_vertices && display_count < 5; ++i) {
            printf("Vértice %d: (%.2f, %.2f, %.2f)\n", i, 
                   my_mesh.vertices[i].x, 
                   my_mesh.vertices[i].y, 
                   my_mesh.vertices[i].z);
            display_count++;
        }
        if (my_mesh.num_vertices > 10) {
            printf("...\n");
            for (int i = my_mesh.num_vertices - 5; i < my_mesh.num_vertices; ++i) {
                printf("Vértice %d: (%.2f, %.2f, %.2f)\n", i, 
                       my_mesh.vertices[i].x, 
                       my_mesh.vertices[i].y, 
                       my_mesh.vertices[i].z);
            }
        }

        printf("\nDetalhes das Faces (primeiras 5 e últimas 5, se houver):\n");
        display_count = 0;
        for (int i = 0; i < my_mesh.num_faces && display_count < 5; ++i) {
            printf("Face %d (vértices: %d): ", i, my_mesh.faces[i].num_vertices);
            for (int j = 0; j < my_mesh.faces[i].num_vertices; ++j) {
                printf("%d ", my_mesh.faces[i].vertex_indices[j]);
            }
            printf("\n");
            display_count++;
        }
        if (my_mesh.num_faces > 10) {
            printf("...\n");
            for (int i = my_mesh.num_faces - 5; i < my_mesh.num_faces; ++i) {
                printf("Face %d (vértices: %d): ", i, my_mesh.faces[i].num_vertices);
                for (int j = 0; j < my_mesh.faces[i].num_vertices; ++j) {
                    printf("%d ", my_mesh.faces[i].vertex_indices[j]);
                }
                printf("\n");
            }
        }

        // Liberar a memória alocada
        free_mesh(&my_mesh);
        printf("\nMemória da malha liberada.\n");
    } else {
        fprintf(stderr, "Falha ao ler o arquivo OFF.\n");
    }

    return 0;
}