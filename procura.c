#include <stdio.h>
#include <string.h>
/*#include <ncurses.h>*/
#include "mpi.h"

int main(int argc,char *argv[]) {
    /*Variáveis de contagem*/
    int numPalavras = 0, i, no;
    int tamanhoArquivo=0, tamanhoPalavra =0;
    int divideArquivo;
    /*Variaveis MPI*/
    int ret, rank, size, tag;
    MPI_Status status;
    double tempo;

    FILE *arquivo;
    /*Variável que armazena caracter para processamento*/
    char busca[20],str[20];

    ret = MPI_Init(&argc, &argv);
    ret = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ret = MPI_Comm_size(MPI_COMM_WORLD, &size);
    tag=100;

    printf("   *** Busca Arquivo *** \n");
    printf("Iremos procurar a palavra Jesus no arquivo Biblia.txt: ");
    tempo=MPI_Wtime();

    strcpy(busca,"Jesus");
    for (i = 0; busca[i] != '\0';i++){ /*busca[i] != "\ 0"*/
    tamanhoPalavra = i+1;    /*tamanho da palavra a ser buscada*/
    };
    //printf("\nA Palavra tem %d caracteres\n",tamanhoPalavra);

    if((arquivo = fopen("texto.txt","r")) == NULL){
    printf("\n\nERRO AO ABRIR O ARQUIVO\n");
    }
    else{
        numPalavras = 0;
    fseek (arquivo, 0, SEEK_END);
    tamanhoArquivo = ftell(arquivo);
    printf("\nO arquivo tem %d caracteres\n",tamanhoArquivo);

if (rank == 0){
    no = 0;
    divideArquivo = (tamanhoArquivo/4)-tamanhoPalavra+1;
    fseek(arquivo,divideArquivo*no,SEEK_SET);
    printf("Hora Inicio %f",tempo);
    i = ftell(arquivo);
    while (i < (divideArquivo*(no+1))){  		//Percorre o arquivo nó 0
        i=ftell(arquivo);
       /* if ((i%100) == 0){
            printf("%d de %d - Palavras encontradas: %d \n",i,tamanhoArquivo, numPalavras);
        }*/
        fgets(str,tamanhoPalavra+1,arquivo); /*lê a qtd de caractetes da palavra a ser buscada*/
       // printf("\n%s",str);
        if (strstr(str,busca)!=NULL){        /*Ve se a string busca existe dentro da string str*/
            numPalavras++;
            fseek(arquivo,0,SEEK_CUR);
        }else{
            if (!feof(arquivo)){
                /*Captura fim do arquivo*/
                if ((ftell(arquivo) < tamanhoArquivo)){
                    fseek(arquivo,(tamanhoPalavra*(-1))+1,SEEK_CUR);
                }
                if(strstr(str,"\n")!=NULL){
                    /*Captura Quebra de linha*/
                    fseek(arquivo,tamanhoPalavra -1,SEEK_CUR);
                }
            }
        }
    }//while

        fclose(arquivo);
	tempo = MPI_Wtime() - tempo;
	ret = MPI_Finalize();
        printf("\n O numero de palavras do encontradas foi: %d\n", numPalavras);
	printf("Tempo decorrido foi de %f", tempo);
//        getch();
    }
    return 0;
}
}


