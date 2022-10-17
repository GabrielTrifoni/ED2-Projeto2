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

typedef struct {
    int status;
    int numReg;
} HeaderSec;

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

void atualizarSecundario();

void buscaSecundaria();

/*----------------------------MAIN----------------------------*/

int main() {

    int opcao = 0;

    abreArquivos();
    atualizarPrimario();

    while (1) {
        printf("---------Menu---------\n");
        printf("1 - Inserir biblioteca\n");
        printf("2 - Busca primaria\n");
        printf("3 - Busca secundaria\n");
        printf("4 - Atualizar secundaria\n");
        printf("5 - Print Arquivos\n");
        printf("0 - Sair\n");
        printf("----------------------\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 0: // Sair
            	atualizarPrimario();
                printf("Saindo...\n");
                exit(0);
            case 1: // Inserir
                inserirBiblioteca();
                break;
            case 2: // Busca primaria
                printf("Busca primaria\n");
                buscaPrimaria();
                break;
            case 3: // Busca secundaria
                printf("Busca secundaria\n");
                buscaSecundaria();
                break;
            case 4: // Atualizar secundaria
                atualizarSecundario();
                break;
            case 5: // Print Arquivos
                printArquivosMenu();
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
        HeaderSec headersec1;
        headersec1.status = 0; // 0 = desatualizado | 1 = atualizado
        headersec1.numReg = 0;
        fwrite(&headersec1, sizeof(HeaderSec), 1, secundario1);
        printf("Arquivo de indice secundario 1 criado\n");
    }
    if ((secundario2 = fopen("secundario2.bin", "r+b")) == NULL) {
        secundario2 = fopen("secundario2.bin", "w+b");
        HeaderSec headersec2;
        headersec2.status = 0; // 0 = desatualizado | 1 = atualizado
        headersec2.numReg = 0;
        fwrite(&headersec2, sizeof(HeaderSec), 1, secundario2);
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
    FILE *secundario1 = fopen("secundario1.bin", "r+b");

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
    fwrite(&status, sizeof(int), 1, secundario1);

    fclose(insere);
    fclose(biblioteca);
    fclose(primario);
    fclose(secundario1);

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
        fread(&indice.isbn, sizeof(char) * 13, 1, primario);
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

void atualizarSecundario() {
		
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");
    FILE *secundario1 = fopen("secundario1.bin", "r+b");
    FILE *secundario2 = fopen("secundario2.bin", "r+b");
	
    HeaderB headerb;
    Header header;
    HeaderSec headersec1;
    HeaderSec headersec2;
    Indice indice;

    int i = 0, j = 0, k = 0, l = 0, x = 0, y = 0, t = 0, cont = 0, n = 0, n2 = 0, tamanho2 = 0, offset = 0, cont2 = 0, offset3 = 0, ultimo = -1, aux = 0, prox = 0, lista = 0, numReg1 = 0;
    char ch, ch2, buffer[50], buffer2[50], isbn[14], isbn2[14], isbn3[14], isbn4[14];

    fread(&headersec1, sizeof(HeaderSec), 1, secundario1);
    if (headersec1.status == 0) {
        fread(&headerb, sizeof(HeaderB), 1, biblioteca);
        for (i = 0; i < headerb.numInsere; i++) {
        	n = 0;
            cont = 0;
            offset = ftell(biblioteca);
            fread(&tamanho2, sizeof(int), 1, biblioteca);
            fread(&isbn, sizeof(char) * 13, 1, biblioteca);
            for (j = 0; j < tamanho2; j++) {
                fread(&ch, sizeof(char), 1, biblioteca);
                if (ch == '#') {
                    fread(&ch, sizeof(char), 1, biblioteca);
                    cont++;
                }
                if (cont == 2) {
                    buffer[n] = ch;
                    n++;
                }
            }
            buffer[n] = '\0';
            printf("Autor: %s  ISBN: %s\n", buffer, isbn);

            rewind(secundario1);
            rewind(secundario2);
            fread(&headersec1, sizeof(HeaderSec), 1, secundario1);
            fread(&headersec2, sizeof(HeaderSec), 1, secundario2);
            //printf("HeaderReg1: %d / HeaderReg2: %d\n", headersec1.numReg, headersec2.numReg);

            if (headersec1.numReg == 0 && headersec2.numReg == 0) {
                fseek(secundario1, sizeof(HeaderSec), SEEK_SET);
                fseek(secundario2, sizeof(HeaderSec), SEEK_SET);
                lista = ftell(secundario2);
                fwrite(&isbn, sizeof(char) * 13, 1, secundario2);
                fwrite(&ultimo, sizeof(int), 1, secundario2);
                fwrite(&buffer, n, 1, secundario1);
                fwrite("#", sizeof(char), 1, secundario1);
                fwrite(&lista, sizeof(int), 1, secundario1);
                rewind(secundario1);
                rewind(secundario2);
                headersec1.numReg++;
                headersec2.numReg++;
                fwrite(&headersec1, sizeof(HeaderSec), 1, secundario1);
                fwrite(&headersec2, sizeof(HeaderSec), 1, secundario2);

            } else {
                rewind(secundario1);
                fread(&headersec1, sizeof(HeaderSec), 1, secundario1);
                for (k = 0; k < headerb.numInsere; k++) {
                    n2 = 0;
                    do {
                        fread(&ch2, sizeof(char), 1, secundario1);
                        if (ch2 != '#') {
                            buffer2[n2] = ch2;
                            n2++;
                        }
                    } while (ch2 != '#');
                    buffer2[n2] = '\0';
                    fread(&lista, sizeof(int), 1, secundario1);
                    isbn2[13] = '\0';
                    //printf("Buffer2:%s\n", buffer2);

                    if (strcmp(buffer, buffer2) == 0) {

                        fseek(secundario2, lista, SEEK_SET);
                        fread(&isbn2, sizeof(char) * 13, 1, secundario2);

                        if (strcmp(isbn, isbn2) != 0) {
                            //printf("Mesmo autor\nBuffer1:%s | Buffer2:%s\n", buffer, buffer2);
                            //printf("Isbn1:%s | Isbn2:%s\n\n", isbn, isbn2);
                            fseek(secundario2, lista, SEEK_SET);
                            fread(&isbn2, sizeof(char) * 13, 1, secundario2);
                            fread(&prox, sizeof(int), 1, secundario2);
                            //printf("Isbn: %s", isbn2);
                            //printf("prox: %d", prox);
                            aux = prox;
                            fseek(secundario2, 0, SEEK_END);
                            prox = ftell(secundario2);
                            fwrite(&isbn, sizeof(char) * 13, 1, secundario2);
                            fwrite(&aux, sizeof(int), 1, secundario2);
                            fseek(secundario2, lista + (sizeof(char) * 13), SEEK_SET);
                            fwrite(&prox, sizeof(int), 1, secundario2);
                        }

                    } /*else if(strcmp(buffer, buffer2) != 0){
                        fseek(secundario1, 0, SEEK_END);
                        fseek(secundario2, 0, SEEK_END);
                        lista = ftell(secundario2);
                        fwrite(&isbn, n, 1, secundario2);
                        fwrite(&ultimo, sizeof(int), 1, secundario2);
                        fwrite(&buffer, n, 1, secundario1);
                        fwrite("#", sizeof(char), 1, secundario1);
                        fwrite(&lista, sizeof(int), 1, secundario1);
                    }*/
                }

            }
            x = (offset + sizeof(tamanho2) + tamanho2);
            fseek(biblioteca, x, SEEK_SET);
        }
        
        header.status = 1;
        rewind(secundario1);
        fwrite(&header.status, sizeof(int), 1, secundario1);

    }
    fclose(biblioteca);
    fclose(secundario1);
    fclose(secundario2);
}

void buscaSecundaria() {

    FILE *lerBuscaS = fopen("busca_s.bin", "rb");
    FILE *secundario1 = fopen("secundario1.bin", "rb");
    FILE *secundario2 = fopen("secundario2.bin", "r+b");
    FILE *biblioteca = fopen("biblioteca.bin", "r+b");
    FILE *primario = fopen("primario.bin", "rb");

    HeaderB headerb;
    HeaderSec headersec1;

    fread(&headerb, sizeof(HeaderB), 1, biblioteca);

    char autor[50], buffer[50], ch, isbn[14];
    int i = 0, j = 0, n = 0, offset = 0, tam3 = 0;

	fseek(lerBuscaS, headerb.numBuscaS*sizeof(char)*50, SEEK_SET);
    fread(&autor, sizeof(autor), 1, lerBuscaS);
	fseek(secundario1, sizeof(HeaderSec), SEEK_SET);
    
	//printf("AutorBuscaS:%s", autor);
    for (i = 0; i < headerb.numInsere; i++) {
        n = 0;
        do {
            fread(&ch, sizeof(char), 1, secundario1);
            if (ch != '#') {
                buffer[n] = ch;
                n++;
            }
        } while (ch != '#');
        buffer[n] = '\0';
        if (strcmp(buffer, autor) == 0) {
            fread(&offset, sizeof(int), 1, secundario1);
            //printf("Buffer:%s\n", buffer);
            break;
        }
        fseek(secundario1, sizeof(int), SEEK_CUR);
    }

    fseek(primario, sizeof(int), SEEK_SET);
    
    Indice *indice = malloc(sizeof(Indice) * headerb.numInsere);
    
    do {
        fseek(secundario2, offset, SEEK_SET);
        //printf("offset:%d\n", offset);
        fread(&isbn, sizeof(char) * 13, 1, secundario2);
        isbn[13] = '\0';
        fread(&offset, sizeof(int), 1, secundario2);
    	//printf("isbn:%s offset:%d\n", isbn, offset);

		fseek(primario, sizeof(Header), SEEK_SET);
        for (j = 0; j < headerb.numInsere; j++) {
            fread(&indice[j].isbn, sizeof(char) * 13, 1, primario);
            fread(&indice[j].offset, sizeof(int), 1, primario);
            indice[j].isbn[13] = '\0';
            //printf("indice.isbn[%d]: %s  /  offset: %d\n\n", j, indice[j].isbn, indice[j].offset);
			
            if (strcmp(isbn, indice[j].isbn) == 0) { // se encontrou o isbn
                fseek(biblioteca, indice[j].offset, SEEK_SET);
                fread(&tam3, sizeof(int), 1, biblioteca);
                printf("Livro encontrado: ");
                for (j = 0; j < tam3; j++) {
                    fread(&ch, sizeof(char), 1, biblioteca);
                    printf("%c", ch);
                }
                printf("\n");
                break;
            }
        }
    } while (offset != -1);

    fclose(lerBuscaS);
    fclose(secundario1);
    fclose(secundario2);
    fclose(biblioteca);

    return;

}

