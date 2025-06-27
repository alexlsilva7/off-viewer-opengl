#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para ler um arquivo OFF e preencher a estrutura Mesh
int read_off_file(const char *filename, Mesh *mesh) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo OFF");
        return -1;
    }

    char line[256];
    // Ler a primeira linha (deve ser "OFF")
    if (fgets(line, sizeof(line), file) == NULL || strncmp(line, "OFF", 3) != 0) {
        fprintf(stderr, "Formato de arquivo OFF inválido: \"OFF\" não encontrado.\n");
        fclose(file);
        return -1;
    }

    // Ler a segunda linha (num_vertices num_faces num_edges)
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Erro ao ler cabeçalho do arquivo OFF.\n");
        fclose(file);
        return -1;
    }
    int num_edges_dummy; // Não usaremos o número de arestas para este trabalho
    if (sscanf(line, "%d %d %d", &mesh->num_vertices, &mesh->num_faces, &num_edges_dummy) != 3) {
        fprintf(stderr, "Erro ao parsear num_vertices, num_faces, num_edges.\n");
        fclose(file);
        return -1;
    }

    // Alocar memória para os vértices
    mesh->vertices = (Vertex *)malloc(mesh->num_vertices * sizeof(Vertex));
    if (!mesh->vertices) {
        perror("Erro ao alocar memória para vértices");
        fclose(file);
        return -1;
    }

    // Ler os vértices
    for (int i = 0; i < mesh->num_vertices; ++i) {
        if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%lf %lf %lf", 
                                                              &mesh->vertices[i].x, 
                                                              &mesh->vertices[i].y, 
                                                              &mesh->vertices[i].z) != 3) {
            fprintf(stderr, "Erro ao ler vértice %d.\n", i);
            free(mesh->vertices);
            fclose(file);
            return -1;
        }
    }

    // Alocar memória para as faces
    mesh->faces = (Face *)malloc(mesh->num_faces * sizeof(Face));
    if (!mesh->faces) {
        perror("Erro ao alocar memória para faces");
        free(mesh->vertices);
        fclose(file);
        return -1;
    }

    // Ler as faces
    for (int i = 0; i < mesh->num_faces; ++i) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Erro ao ler face %d.\n", i);
            free(mesh->vertices);
            free(mesh->faces);
            fclose(file);
            return -1;
        }

        char *token = strtok(line, " \t\n");
        if (!token) {
            fprintf(stderr, "Erro ao parsear número de vértices da face %d.\n", i);
            free(mesh->vertices);
            free(mesh->faces);
            fclose(file);
            return -1;
        }
        mesh->faces[i].num_vertices = atoi(token);

        if (mesh->faces[i].num_vertices < 3 || mesh->faces[i].num_vertices > 4) {
            fprintf(stderr, "Número de vértices inválido para a face %d: %d. Apenas 3 ou 4 são suportados.\n", i, mesh->faces[i].num_vertices);
            free(mesh->vertices);
            free(mesh->faces);
            fclose(file);
            return -1;
        }

        mesh->faces[i].vertex_indices = (int *)malloc(mesh->faces[i].num_vertices * sizeof(int));
        if (!mesh->faces[i].vertex_indices) {
            perror("Erro ao alocar memória para índices de vértices da face");
            free(mesh->vertices);
            // Liberar memória das faces já alocadas
            for (int j = 0; j < i; ++j) {
                free(mesh->faces[j].vertex_indices);
            }
            free(mesh->faces);
            fclose(file);
            return -1;
        }

        for (int j = 0; j < mesh->faces[i].num_vertices; ++j) {
            token = strtok(NULL, " \t\n");
            if (!token) {
                fprintf(stderr, "Erro ao ler índice de vértice %d da face %d.\n", j, i);
                free(mesh->vertices);
                // Liberar memória das faces já alocadas
                for (int k = 0; k <= i; ++k) {
                    free(mesh->faces[k].vertex_indices);
                }
                free(mesh->faces);
                fclose(file);
                return -1;
            }
            mesh->faces[i].vertex_indices[j] = atoi(token);
        }
    }

    fclose(file);
    return 0;
}

// Função para liberar a memória alocada para a malha
void free_mesh(Mesh *mesh) {
    if (mesh) {
        if (mesh->vertices) {
            free(mesh->vertices);
            mesh->vertices = NULL;
        }
        if (mesh->faces) {
            for (int i = 0; i < mesh->num_faces; ++i) {
                if (mesh->faces[i].vertex_indices) {
                    free(mesh->faces[i].vertex_indices);
                    mesh->faces[i].vertex_indices = NULL;
                }
            }
            free(mesh->faces);
            mesh->faces = NULL;
        }
        mesh->num_vertices = 0;
        mesh->num_faces = 0;
    }
}
