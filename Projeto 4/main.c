#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Struct_Celula *Apontador;
struct Struct_Celula {
    int Id;
    Apontador Prox;
};
typedef struct Struct_Celula Celula;

typedef struct TipoLista {
    Apontador Primeiro, Ultimo;
    int Codigo_Vertice;
    int Peso_Vertice;
    int Id_Vertice; // TIRAR OU MANTER O ID?
    char Nome_Vertice[42];
    int Semestre;
    int Horarios[3];
} TipoLista;

typedef struct TipoGrafo {
    TipoLista Adj[31];  // 31 ou 32?
    int NumVertices;
    int NumArestas;
} TipoGrafo;

void cria_lista_vazia(TipoLista *Lista, int codigo, int peso, int id, char nome[], int semestre) {
    Lista->Primeiro = (Apontador) malloc (sizeof(Celula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Codigo_Vertice = codigo;    // Armazenamos o código da disciplina
    Lista->Peso_Vertice = peso;
    Lista->Id_Vertice = id;
    strcpy(Lista->Nome_Vertice, nome);  // Armazenamos o nome da disciplina
    Lista->Semestre = semestre;
    for(int i = 0; i < 3; i++)
        Lista->Horarios[i] = 0;
    Lista->Primeiro->Prox = NULL;
}

/*Função responsável por criar o grafo, enviando o código, nome e peso da disciplina para identificação*/
void cria_grafo_vazio(TipoGrafo *Grafo) {
    int codigos[31] = {113468, 113476, 113034, 113450, 116319, 113042,
    113093, 116394, 117889, 115045, 113107, 113417, 116572, 117366,
    117897, 117901, 116343, 116378,116441, 116653, 117935, 116416,
    116432, 116882, 117943, 117960, 116726, 117536, 117927, 117951, 117919};
    char nomes[31][42] = {"INTRODUCAO AOS SISTEMAS COMPUTACIONAIS", "ALGORITMOS E PROGRAMACAO DE COMPUTADORES", "CALCULO 1",
    "FUNDAMENTOS TEORICOS DA COMPUTACAO", "ESTRUTURAS DE DADOS", "CALCULO 2", "INTRODUCAO A ALGEBRA LINEAR", 
    "ORGANIZACAO E ARQUITETURA DE COMPUTADORES", "TECNICAS DE PROGRAMACAO 1", "PROBABILIDADE E ESTATISTICA", "ALGEBRA 1", "CALCULO NUMERICO", 
    "REDES DE COMPUTADORES", "LOGICA COMPUTACIONAL 1", "TECNICAS DE PROGRAMACAO 2", "TEORIA E APLICACAO DE GRAFOS",
    "LINGUAGENS DE PROGRAMACAO", "BANCOS DE DADOS", "ENGENHARIA DE SOFTWARE", "INTRODUCAO A INTELIGENCIA ARTIFICIAL", "PROGRAMACAO CONCORRENTE", 
    "SISTEMAS DE INFORMACAO", "SOFTWARE BASICO", "AUTOMATOS E COMPUTABILIDADE", "COMPUTACAO EXPERIMENTAL", "FUNDAMENTOS DE SISTEMAS OPERACIONAIS",
    "INFORMATICA E SOCIEDADE", "PROJETO E ANALISE DE ALGORITMOS", "SEGURANCA COMPUTACIONAL", "COMPILADORES",
    "METODOLOGIA CIENTIFICA"};   
    int pesos[31] = {4, 6, 6, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 4, 4, 2, 4, 4, 4, 2};
    int semestre[31] = {1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8};
    for(int i = 0; i < Grafo->NumVertices; i++)
        cria_lista_vazia(&Grafo->Adj[i], codigos[i], pesos[i], i, nomes[i], semestre[i]);
}

void insere_aresta(int id, TipoLista *Lista) {
    Lista->Ultimo->Prox = (Apontador) malloc (sizeof(Celula));
    Lista->Ultimo = Lista->Ultimo->Prox;
    Lista->Ultimo->Id = id;
    Lista->Ultimo->Prox = NULL;
}

void imprime_grafo(TipoGrafo *Grafo) {
    Apontador Aux;
    for(int i = 0; i < Grafo->NumVertices; i++) {
        printf("%d: ", Grafo->Adj[i].Id_Vertice);
        if(Grafo->Adj[i].Primeiro != Grafo->Adj[i].Ultimo) {    // Se não for vazio
            Aux = Grafo->Adj[i].Primeiro->Prox;
            while(Aux != NULL) {
                printf("%d -> ", Aux->Id);
                Aux = Aux->Prox;
            }
        }
        printf("NULL\n\n");
    }
}

void monta_grafo(TipoGrafo *Grafo) {
    char linha[22];
    int numeros[2], i = 0;
    FILE *arq = fopen("arestas.txt", "r");

    if(arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    while(!feof(arq)) {
        fgets(linha, 22, arq);
        if(linha[0] != '/') {
            char *numero = strtok(linha, " ");
            while(numero) {
                numeros[i] = atoi(numero);
                numero = strtok(NULL, " ");
                i++;
            }
            insere_aresta(numeros[1], &Grafo->Adj[numeros[0]]);
            insere_aresta(numeros[0], &Grafo->Adj[numeros[1]]);
            i = 0;
        }
    }

    fclose(arq);
}

void libera_grafo(TipoGrafo *Grafo) {
    Apontador anterior, aux;
    for(int i = 0; i < Grafo->NumVertices; i++) {
        aux = Grafo->Adj[i].Primeiro->Prox;
        free(Grafo->Adj[i].Primeiro);   // Libera o primeiro elemento
        Grafo->Adj[i].Primeiro = NULL;
        while(aux != NULL) {            // Libera os demais, até que não existam outros
            anterior = aux;
            aux = aux->Prox;
            free(anterior);
        }
    }
}

int main() {
    TipoGrafo Grafo;
    Grafo.NumVertices = 31;
    Grafo.NumArestas = 102;

    cria_grafo_vazio(&Grafo);

    monta_grafo(&Grafo);

    imprime_grafo(&Grafo);

    

    libera_grafo(&Grafo);

    return 0;
}