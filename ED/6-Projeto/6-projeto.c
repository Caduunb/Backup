/*
	   @authors:	Caio Eduardo Coelho de Oliveira e Raul Silva Menezes
	@matriculas:	16/0003679	e 	16/0017327
		 @flags:	gcc -Wall
*/
#include "proj6.h"

int main()
{
	char arquivo[20];
	PALAVRA palavra;
	lista* listaPalavras;
	listaPalavras = criaLista();

	//Criando vetor de pesos
	unsigned int vetPesos[MaiorPalavra];
	criavetPesos(vetPesos);
	int i;

	//Salvando palavras no arquivo IndInvert.txt
	FILE* entrada;
	int documento = 1;
	while(documento<=qtdDocs)
	{
		sprintf(arquivo, "docs/doc%d.txt", documento);
		printf("LENDO ARQUIVO %s\n", arquivo);
		entrada = fopen(arquivo,"r");
		if(entrada==NULL)	exit(1);
		while(!feof(entrada))
		{
			fscanf(entrada, " %s", palavra);
			if(fgetc(entrada) == '\n')	continue; //FLAG
			incluiNaLista(listaPalavras, documento, palavra);
		}
		fclose(entrada);
		documento++;
	}
	lista *aux = listaPalavras;
	salvaIndInvert(aux);

	//Criando HashDocs.txt
	Hash Tabela[TamanhoTabela];
	for(i =0; i<TamanhoTabela; i++)
		Tabela[i].celula = criaLista();
	printf("Tabela criada. Espaço das listas alocado.\n");
	listaPalavras = listaPalavras->prox;   //Warning: remover header "zzzzz"
	while(listaPalavras!=NULL)
	{
		insereNaHT(listaPalavras, Tabela, vetPesos);
		listaPalavras = listaPalavras->prox;
	}
	Hash* AUX = Tabela;
	salvaHashDocs(AUX);

	//Interaçao com usuario. Busca de palavras em documentos analisados
	printf("O que procuras, Big User?\n");
	scanf(" %s", palavra);
	int key, indice;
	key = hashFunction(palavra, vetPesos);
	indice = key%TamanhoTabela;
	printf("INDICE: %d\n", indice);
	int marcador = 0;
	while(Tabela[indice].celula != NULL)
	{
		if(strcmp(Tabela[indice].celula->palavra, palavra) == 0)
		{
			printf("Encontrada!\n%s esta contida em:\n", Tabela[indice].celula->palavra);
			for(i=0; i<qtdDocs && Tabela[indice].celula->documentos[i]!=0; i++)
				printf("doc%d.txt, ", Tabela[indice].celula->documentos[i]);
			marcador = 1;
			printf("\n");
			break;
		}
		Tabela[indice].celula = Tabela[indice].celula->prox; 
	}
	if(marcador == 0)
		printf("Palavra nao encontrada em nenhum dos documentos analisados.\n");
	return 0;
}