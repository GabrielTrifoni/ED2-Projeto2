#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

typedef struct {
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
} Livro;

typedef struct {
    int numInsere;
} Header;

void abreArquivos();

void inserir();

int contChar(char *ch);

int contRegStr(FILE *arq);

void printArquivosMenu();

void printArquivos(FILE *arq);

int main() {
    int opcao = -1;

    while (1) {
        printf("---------Menu---------\n");
        printf("1 - Inserir\n");
        printf("2 - Busca primaria\n");
        printf("3 - Busca secundaria\n");
        printf("4 - Print Arquivos\n");
        printf("0 - Sair\n");
        printf("----------------------\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 0: // Sair
                printf("Saindo...\n");
                exit(0);
                break;
            case 1: // Inserir
                inserir();
                break;
            case 2: // Busca primaria
                break;
            case 3: // Busca secundaria
                break;
            case 4: // Print Arquivos
                printArquivosMenu();
                break;
        }
    }

    return 0;
}

void abreArquivos() {
    FILE *insere;
    FILE *primario;
    FILE *secundario;
    FILE *biblioteca;

    if ((insere = fopen("insere.bin", "rb")) == NULL) {
        printf("Nao foi possivel abrir arquivo insere\n");
        getch();
    }
    if ((primario = fopen("primario.bin", "r+b")) == NULL) {
        primario = fopen("primario.bin", "w+b");
        printf("Arquivo de indice primario criado\n");
    }
    if ((secundario = fopen("secundario.bin", "r+b")) == NULL) {
        secundario = fopen("secundario.bin", "w+b");
        printf("Arquivo de indice secundario criado\n");
    }
    if ((biblioteca = fopen("biblioteca.bin", "r+b")) == NULL) {
        biblioteca = fopen("biblioteca.bin", "w+b");
        Header header;
        header.numInsere = 0;
        fwrite(&header, sizeof(Header), 1, biblioteca);
        printf("Arquivo biblioteca criado\n");
    }

    fclose(insere);
    fclose(primario);
    fclose(secundario);
    fclose(biblioteca);
}

void inserir() {
    abreArquivos();

    FILE *insere = fopen("insere.bin", "rb");
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");
    Livro livro;
    int inicio = 0, tam = 0;

    rewind(biblioteca);
    fread(&inicio, sizeof(int), 1, biblioteca);

    if (inicio == contRegStr(insere)) {
        printf("Todos os registros ja foram lidos...\n");
        return;
    }

    fseek(biblioteca, 0, SEEK_END);
    fseek(insere, inicio * sizeof(Livro), SEEK_SET);

    fread(&livro, sizeof(Livro), 1, insere);

    tam = (contChar(livro.isbn) + contChar(livro.titulo) +
           contChar(livro.autor) + contChar(livro.ano) + 3); // 3 = "...#...#...#..."

    fwrite(&tam, sizeof(int), 1, biblioteca);
    fwrite(&livro.isbn, contChar(livro.isbn), 1, biblioteca);
    fwrite("#", sizeof(char), 1, biblioteca);
    fwrite(&livro.titulo, contChar(livro.titulo), 1, biblioteca);
    fwrite("#", sizeof(char), 1, biblioteca);
    fwrite(&livro.autor, contChar(livro.autor), 1, biblioteca);
    fwrite("#", sizeof(char), 1, biblioteca);
    fwrite(&livro.ano, contChar(livro.ano), 1, biblioteca);
    printf("Inserido: %d#%s#%s#%s#%s \n", tam, livro.isbn, livro.titulo, livro.autor, livro.ano);

    inicio++;
    rewind(biblioteca);
    fwrite(&inicio, sizeof(int), 1, biblioteca);

    fclose(insere);
    fclose(biblioteca);
}

int contChar(char *ch) {
    int cont = 0, i = 0;
    for (i = 0; i < strlen(ch); i++) {
        cont++;
    }
    return cont;
}

int contRegStr(FILE *arq) {
    Livro livro;
    int cont = 0;

    rewind(arq);
    while (fread(&livro, sizeof(Livro), 1, arq)) {
        cont++;
    }

    return cont;
}

void printArquivosMenu() {
    FILE *insere = fopen("insere.bin", "rb");
    FILE *biblioteca = fopen("biblioteca.bin", "rb");
    FILE *primario = fopen("primario.bin", "rb");
    FILE *secundario = fopen("secundario.bin", "rb");
    int opcao = 0;

    while (1) {
        printf("---------Print--------\n");
        printf("1 - Inserir\n");
        printf("2 - Biblioteca\n");
        printf("3 - Primario\n");
        printf("4 - Secundario\n");
        printf("0 - Voltar\n");
        printf("----------------------\n");
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("Voltando para o menu...\n");
            break;
        } else if (opcao == 1) {
            printArquivos(insere);
            break;
        } else if (opcao == 2) {
            printArquivos(biblioteca);
            break;
        } else if (opcao == 3) {
            printArquivos(primario);
            break;
        } else if (opcao == 4) {
            printArquivos(secundario);
            break;
        }

        printf("Opcao invalida, tente novamente\n");
    }

    fclose(insere);
    fclose(biblioteca);
    fclose(primario);
    fclose(secundario);
}

void printArquivos(FILE *arq) {
    char *ch = malloc(sizeof(char));

    while (fread(&ch, sizeof(char), 1, arq)) {
        printf("%c", ch);
    }

    printf("\n");
    return;
}