#include <stdio.h>
#include <string.h>
/*#include <ncurses.h>*/
#include "mpi.h"

int main(int argc,char *argv[]) {
    /*Variáveis de contagem*/
    int numPalavras = 0, retorno, i, no;
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

if (rank == 0){
    printf("   *** Busca Arquivo *** \n");
    printf("Iremos procurar a palavra Jesus no arquivo Biblia.txt: ");
    tempo=MPI_Wtime();
}
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
if (rank  == 0){ 
   printf("\nO arquivo tem %d caracteres\n",tamanhoArquivo);
}
    no = rank;
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

if (rank == 0){
        ret = MPI_Recv(&retorno, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
	numPalavras=numPalavras+retorno;
	if (size>2){
		ret = MPI_Recv(&retorno, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
        	numPalavras=numPalavras+retorno;
	}
	if (size>3){
		ret = MPI_Recv(&retorno, 1, MPI_INT, 3, tag, MPI_COMM_WORLD, &status);
        	numPalavras=numPalavras+retorno;
	}
	tempo = MPI_Wtime() - tempo;

	printf("\n O numero de palavras do encontradas foi: %d\n", numPalavras);
        printf("Tempo decorrido foi de %f\n\n\n", tempo);
}//no 0
if (rank == 1){
    ret = MPI_Send(&numPalavras, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
}
if (rank == 2){
    ret = MPI_Send(&numPalavras, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
}
if (rank == 3){
    ret = MPI_Send(&numPalavras, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
}

    ret = MPI_Finalize();
    return 0;
}
}


