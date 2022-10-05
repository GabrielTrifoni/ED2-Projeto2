#include <stdio.h>
#include <string.h>

typedef struct {
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
} Livro;

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
                break;
            case 2: // 
                break;
            case 3: // 
                break;
            case 4: // Print Arquivos
                // printArquivosMenu();
                break;
        }
    }

    return 0;
}
