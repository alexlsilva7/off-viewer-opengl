#ifndef MESH_H
#define MESH_H

// Estrutura para representar um vértice
typedef struct {
    double x, y, z;
} Vertex;

// Estrutura para representar uma face
// Uma face pode ser triangular (3 vértices) ou quadrilátera (4 vértices)
typedef struct {
    int num_vertices; // Número de vértices na face (3 ou 4)
    int *vertex_indices; // Array de índices dos vértices que compõem a face
} Face;

// Estrutura para representar a malha completa
typedef struct {
    int num_vertices;
    Vertex *vertices; // Array de vértices

    int num_faces;
    Face *faces; // Array de faces
} Mesh;

// Protótipo da função para ler um arquivo OFF
// Retorna 0 em caso de sucesso, -1 em caso de erro
int read_off_file(const char *filename, Mesh *mesh);

// Protótipo da função para liberar a memória alocada para a malha
void free_mesh(Mesh *mesh);

#endif // MESH_H
