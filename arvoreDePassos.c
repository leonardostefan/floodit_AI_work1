typedef struct
{
    tmapa *raiz;
} arvoreDePassos;

typedef struct
{
    int nlinhas;
    int ncolunas;
    int corAtual;
    int nCores;
    int **mapa;
} tmapa;
typedef struct{
    nodoArvoreMapa* anterior;
    tmapa* mapa;
    nodoArvoreMapa* proximos; 
}nodoArvoreMapa

void expandirProximo