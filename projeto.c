#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

/*----------------------------STRUCTS----------------------------*/
typedef struct {
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
} Livro;

typedef struct {
    char isbn[14];
    int offset;
} Indice;

typedef struct {
    int numInsere;
    int numBuscaP;
    int numBuscaS;
} HeaderB;

typedef struct {
    int status;
} Header;

/*----------------------------PROTOTIPOS----------------------------*/

void abreArquivos();

int contChar(char *ch);

void printArquivo(char *arqNome);

void printArquivosMenu();

void quicksort(Indice *index, int first, int last);

int verificarQuantLido(FILE *insere, int quant);

void inserirBiblioteca();

void atualizarPrimario();

void buscaPrimaria();

/*----------------------------MAIN----------------------------*/

int main() {

    int opcao = 0;

    abreArquivos();

    while (1) {
        printf("---------Menu---------\n");
        printf("1 - Inserir biblioteca\n");
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
            case 1: // Inserir
                inserirBiblioteca();
                break;
            case 2: // Busca primaria
                printf("Busca primaria\n");
                atualizarPrimario();
                buscaPrimaria();
                break;
            case 3: // Busca secundaria
                printf("Busca secundaria\n");
                break;
            case 4: // Print Arquivos
                printArquivosMenu();
                break;
        }
    }
}

/*----------------------------FUNCOES----------------------------*/

void abreArquivos() {

    FILE *biblioteca;
    FILE *primario;
    FILE *secundario1;
    FILE *secundario2;
    FILE *lerInsere;
    FILE *lerBuscaP;
    FILE *lerBuscaS;

    if ((biblioteca = fopen("biblioteca.bin", "r+b")) == NULL) {
        biblioteca = fopen("biblioteca.bin", "w+b");
        HeaderB headerb;
        headerb.numInsere = 0;
        headerb.numBuscaP = 0;
        headerb.numBuscaS = 0;
        fwrite(&headerb, sizeof(HeaderB), 1, biblioteca);
        printf("Arquivo biblioteca criado\n");
    }

    if ((primario = fopen("primario.bin", "r+b")) == NULL) {
        primario = fopen("primario.bin", "w+b");
        Header header;
        header.status = 0; // 0 = desatualizado | 1 = atualizado
        fwrite(&header, sizeof(Header), 1, primario);
        printf("Arquivo de indice primario criado\n");
    }

    if ((secundario1 = fopen("secundario1.bin", "r+b")) == NULL) {
        secundario1 = fopen("secundario1.bin", "w+b");
        Header header;
        header.status = 0; // 0 = desatualizado | 1 = atualizado
        fwrite(&header, sizeof(Header), 1, secundario1);
        printf("Arquivo de indice secundario 1 criado\n");
    }
    if ((secundario2 = fopen("secundario2.bin", "r+b")) == NULL) {
        secundario2 = fopen("secundario2.bin", "w+b");
        Header header;
        header.status = 0; // 0 = desatualizado | 1 = atualizado
        fwrite(&header, sizeof(Header), 1, secundario2);
        printf("Arquivo de indice secundario 2 criado\n");
    }

    if ((lerInsere = fopen("insere.bin", "rb")) == NULL) {
        printf("Nao foi possivel abrir arquivo insere\n");
        getch();
    }

    if ((lerBuscaP = fopen("busca_p.bin", "rb")) == NULL) {
        printf("Nao foi possivel abrir arquivo insere\n");
        getch();
    }

    if ((lerBuscaS = fopen("busca_s.bin", "rb")) == NULL) {
        printf("Nao foi possivel abrir arquivo insere\n");
        getch();
    }

    fclose(biblioteca);
    fclose(primario);
    fclose(secundario1);
    fclose(secundario2);
    fclose(lerInsere);
    fclose(lerBuscaP);
    fclose(lerBuscaS);
}

int contChar(char *ch) {
    int cont = 0, i = 0;
    for (i = 0; i < strlen(ch); i++) {
        cont++;
    }
    return cont;
}

void printArquivo(char *arqNome) {
    FILE *arq = fopen(arqNome, "rb");
    char ch;

    if (strcmp(arqNome, "biblioteca.bin") == 0) { // se o arquivo for biblioteca.bin
        HeaderB headerb;
        fread(&headerb, sizeof(HeaderB), 1, arq);
        printf("Header: \n");
        printf("NumInsere: %d  | NumBuscaP: %d  | NumBuscaS: %d \n\n",
               headerb.numInsere, headerb.numBuscaP, headerb.numBuscaS);
        fseek(arq, sizeof(HeaderB), 0);
        while (fread(&ch, sizeof(char), 1, arq)) {
            printf("%c", ch);
        }
        printf("\n");
    } else { // arquivo generico
        while (fread(&ch, sizeof(char), 1, arq)) {
            printf("%c", ch);
        }
        printf("\n");
    }
}

void printArquivosMenu() {
    abreArquivos();

    int opcao = -1;

    printf("---------Print---------\n");
    printf("1 - Biblioteca\n");
    printf("2 - Primario\n");
    printf("3 - Secundario 1\n");
    printf("4 - Secundario 2\n");
    printf("5 - Insere\n");
    printf("0 - Voltar\n");
    printf("----------------------\n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 0: // Sair
            printf("Voltando...\n");
            return;
        case 1:
            printArquivo("biblioteca.bin");
            break;
        case 2:
            printArquivo("primario.bin");
            break;
        case 3:
            printArquivo("secundario1.bin");
            break;
        case 4:
            printArquivo("secundario2.bin");
            break;
        case 5:
            printArquivo("insere.bin");
            break;
        default:
            printf("Opcao invalida!");
            break;
    }
}

void quicksort(Indice *index, int first, int last) {
    int i, j, pivot;
    char temp[13];
    int tempInt;
    if (first < last) {
        pivot = first;
        i = first;
        j = last;

        while (i < j) {
            while (strcmp(index[i].isbn, index[pivot].isbn) < 0 && i < last)
                i++;
            while (strcmp(index[j].isbn, index[pivot].isbn) > 0)
                j--;
            if (i < j) {
                strcpy(temp, index[i].isbn);
                strcpy(index[i].isbn, index[j].isbn);
                strcpy(index[j].isbn, temp);

                tempInt = index[i].offset;
                index[i].offset = index[j].offset;
                index[j].offset = tempInt;
            }
        }

        strcpy(temp, index[pivot].isbn);
        strcpy(index[pivot].isbn, index[j].isbn);
        strcpy(index[j].isbn, temp);

        tempInt = index[pivot].offset;
        index[pivot].offset = index[j].offset;
        index[j].offset = tempInt;

        quicksort(index, first, j - 1);
        quicksort(index, j + 1, last);

    }
}

int verificarQuantLido(FILE *insere, int quant) {
    int quantLivrosTotal = 0;
    Livro livro;
    rewind(insere);
    while (fread(&livro, sizeof(Livro), 1, insere)) {
        quantLivrosTotal++;
    }

    if (quant == quantLivrosTotal) {
        printf("Todos os registros ja foram lidos...\n");
        return 0;
    }

    return 1;
}

void inserirBiblioteca() {

    FILE *insere = fopen("insere.bin", "rb");
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");
    FILE *primario = fopen("primario.bin", "r+b");

    Livro livro;
    HeaderB headerb;
    int tam = 0, status = 0;

    fread(&headerb, sizeof(HeaderB), 1, biblioteca); // le header do biblioteca

    if (verificarQuantLido(insere, headerb.numInsere) == 0) { // verifica quantidade lido
        return;
    }

    fseek(insere, headerb.numInsere * sizeof(Livro), SEEK_SET); // le o registro
    fread(&livro, sizeof(Livro), 1, insere); // le o registro

    tam = (contChar(livro.isbn) + contChar(livro.titulo) +
           contChar(livro.autor) + contChar(livro.ano) + 3);

    fseek(biblioteca, 0, SEEK_END);
    fwrite(&tam, sizeof(int), 1, biblioteca); // escreve o tamanho
    fprintf(biblioteca, "%s#%s#%s#%s", livro.isbn, livro.titulo, livro.autor, livro.ano); // escreve o registro
    printf("%d%s#%s#%s#%s\n", tam, livro.isbn, livro.titulo, livro.autor, livro.ano);

    headerb.numInsere++;
    rewind(biblioteca);
    fwrite(&headerb.numInsere, sizeof(int), 1, biblioteca); // atualiza header

    fwrite(&status, sizeof(int), 1, primario);

    fclose(insere);
    fclose(biblioteca);
    fclose(primario);

}

void atualizarPrimario() {

    FILE *primario = fopen("primario.bin", "r+b");
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");

    Header header;
    HeaderB headerb;
    Livro livro;

    int i = 0, tamanho = 0, x = 0;

    fread(&header, sizeof(Header), 1, primario);

    if (header.status == 0) { // se primario estiver desatualizado
        fread(&headerb, sizeof(HeaderB), 1, biblioteca);

        Indice *indice = malloc(headerb.numInsere * sizeof(Indice));

        fseek(biblioteca, sizeof(HeaderB), SEEK_SET);

        for (i = 0; i < headerb.numInsere; i++) {
            indice[i].offset = ftell(biblioteca);
            fread(&tamanho, sizeof(int), 1, biblioteca);
            fread(&livro.isbn, sizeof(char) * 13, 1, biblioteca);
            memcpy(indice[i].isbn, livro.isbn, 13);
            indice[i].isbn[13] = '\0';

            x = (indice[i].offset + sizeof(tamanho) + tamanho);
            fseek(biblioteca, x, SEEK_SET);
        }

        quicksort(indice, 0, headerb.numInsere - 1);

        fseek(primario, sizeof(Header), SEEK_SET);
        for (i = 0; i < headerb.numInsere; i++) {
            fwrite(&indice[i].isbn, sizeof(char) * 13, 1, primario);
            fwrite(&indice[i].offset, sizeof(int), 1, primario);
        }

        header.status = 1;
        rewind(primario);
        fwrite(&header.status, sizeof(int), 1, primario);
    }

    fclose(primario);
    fclose(biblioteca);
}

void buscaPrimaria() {

    FILE *lerBuscaP = fopen("busca_p.bin", "rb");
    FILE *primario = fopen("primario.bin", "rb");
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");

    HeaderB headerb;
    Header header;
    Indice indice;
    Livro livro;

    char isbn[14], ch;
    int i = 0, j = 0, tam2 = 0;

    fread(&headerb, sizeof(HeaderB), 1, biblioteca);

    fseek(lerBuscaP, headerb.numBuscaP * sizeof(char) * 14, SEEK_SET);
    fread(&isbn, sizeof(isbn), 1, lerBuscaP);
    isbn[13] = '\0';

    printf("ISBN a ser buscado:%s\n", isbn);

    fseek(primario, sizeof(Header), SEEK_SET);


    for (i = 0; i < headerb.numInsere; i++) {
        fread(&indice.isbn, sizeof(char)*13, 1, primario);
        fread(&indice.offset, sizeof(int), 1, primario);
        indice.isbn[13] = '\0';
        //printf("isbn[%d]: %s  /  offset: %d\n", i, indice.isbn, indice.offset);

        if (strcmp(isbn, indice.isbn) == 0) { // se encontrou o isbn
            fseek(biblioteca, indice.offset, SEEK_SET);
            fread(&tam2, sizeof(int), 1, biblioteca);
            printf("Livro encontrado: ");
            for (j = 0; j < tam2; j++) {
                fread(&ch, sizeof(char), 1, biblioteca);
                printf("%c", ch);
            }
            printf("\n");
            break;
        }
    }

    headerb.numBuscaP++;
    rewind(biblioteca);
    fwrite(&headerb, sizeof(HeaderB), 1, biblioteca);

    fclose(lerBuscaP);
    fclose(biblioteca);
    fclose(primario);
}
