#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    int nlinhas;
    int ncolunas;
    int ncores;
    int **mapa;
} tmapa;

void gera_mapa(tmapa *m, int semente)
{
    int i, j;

    if (semente < 0)
        srand(time(NULL));
    else
        srand(semente);
    m->mapa = (int **)malloc(m->nlinhas * sizeof(int *));
    for (i = 0; i < m->nlinhas; i++)
    {
        m->mapa[i] = (int *)malloc(m->ncolunas * sizeof(int));
        for (j = 0; j < m->ncolunas; j++)
            m->mapa[i][j] = 1 + rand() % m->ncores;
    }
}

void carrega_mapa(tmapa *m)
{
    int i, j;

    scanf("%d", &(m->nlinhas));
    scanf("%d", &(m->ncolunas));
    scanf("%d", &(m->ncores));
    m->mapa = (int **)malloc(m->nlinhas * sizeof(int *));
    for (i = 0; i < m->nlinhas; i++)
    {
        m->mapa[i] = (int *)malloc(m->ncolunas * sizeof(int));
        for (j = 0; j < m->ncolunas; j++)
            scanf("%d", &(m->mapa[i][j]));
    }
}

void mostra_mapa(tmapa *m)
{
    int i, j;

    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for (i = 0; i < m->nlinhas; i++)
    {
        for (j = 0; j < m->ncolunas; j++)
            if (m->ncores > 10)
                printf("%02d ", m->mapa[i][j]);
            else
                printf("%d ", m->mapa[i][j]);
        printf("\n");
    }
}

void mostra_mapa_cor(tmapa *m)
{
    int i, j;
    char *cor_ansi[] = {"\x1b[0m",
                        "\x1b[31m", "\x1b[32m", "\x1b[33m",
                        "\x1b[34m", "\x1b[35m", "\x1b[36m",
                        "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
                        "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
                        "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};

    if (m->ncores > 15)
    {
        mostra_mapa(m);
        return;
    }
    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for (i = 0; i < m->nlinhas; i++)
    {
        for (j = 0; j < m->ncolunas; j++)
            if (m->ncores > 10)
                printf("%s%02d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
            else
                printf("%s%d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
        printf("\n");
    }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor)
{
    m->mapa[l][c] = cor;
    if (l < m->nlinhas - 1 && m->mapa[l + 1][c] == fundo)
        pinta(m, l + 1, c, fundo, cor);
    if (l < m->nlinhas - 1 && c < m->ncolunas - 1 && m->mapa[l + 1][c + 1] == fundo)
        pinta(m, l + 1, c + 1, fundo, cor);
    if (c < m->ncolunas - 1 && m->mapa[l][c + 1] == fundo)
        pinta(m, l, c + 1, fundo, cor);
    if (l > 0 && c < m->ncolunas - 1 && m->mapa[l - 1][c + 1] == fundo)
        pinta(m, l - 1, c + 1, fundo, cor);
    if (l > 0 && m->mapa[l - 1][c] == fundo)
        pinta(m, l - 1, c, fundo, cor);
    if (l > 0 && c > 0 && m->mapa[l - 1][c - 1] == fundo)
        pinta(m, l - 1, c - 1, fundo, cor);
    if (c > 0 && m->mapa[l][c - 1] == fundo)
        pinta(m, l, c - 1, fundo, cor);
    if (l < m->nlinhas - 1 && c > 0 && m->mapa[l + 1][c - 1] == fundo)
        pinta(m, l + 1, c - 1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor)
{
    if (cor == m->mapa[0][0])
        return;
    pinta(m, 0, 0, m->mapa[0][0], cor);
}

int min(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
        return b;
}
int condicaoDeParada(tmapa *m)
{
    int cor = m->mapa[0][0];

    for (int i = 0; i < m->nlinhas; i++)
    {
        for (int j = 0; j < m->ncolunas; j++)
        {
            if (m->mapa[i][j] != cor)
                return 0;
        }
    }
    printf("\n Fim\n");

    return 1;
}

int main(int argc, char **argv)
{
    int cor;
    tmapa m;
    int semente = -1;
    int steps = 0;
    int parada = 0;

    if ((argc < 4 || argc > 5) && argc != 1)
    {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }
    else
    {
        if (argc == 1)
        {
            carrega_mapa(&m);
        }
        else
        {
            m.nlinhas = atoi(argv[1]);
            m.ncolunas = atoi(argv[2]);
            m.ncores = atoi(argv[3]);
            if (argc == 5)
                semente = atoi(argv[4]);
            else
                semente = -1;
            gera_mapa(&m, semente);
        }
    }
    mostra_mapa_cor(&m);
    // for (int i = 0; i < min(m.ncolunas, m.nlinhas) && parada == 0; i++)
    // {
    //     pinta_mapa(&m, m.mapa[i][i]);
    //     mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
    //     steps++;
    //     parada = condicaoDeParada(&m);
    // }
    cor = 1;
    while (cor > 0 && cor <= m.ncores && parada == 0)
    {
        scanf("%d", &cor);
        pinta_mapa(&m, cor);
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        steps++;
        parada = condicaoDeParada(&m);
    }

    mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
    printf("\nnumero de passos: %d", steps);

    return 0;
}