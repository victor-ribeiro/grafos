#include <stdio.h>
#include<stdio_ext.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define VISITADO 'V'
#define NAO_VISITADO 'N'

struct no {
	int valor;
	char cVis;
	struct no *prox;
	struct adj *ini;
};

struct adj {
	int custo;
	struct no *viz;
	struct adj *prox;
};

struct Lista {
	struct no *primeiro;
};

int incluir (struct Lista *pListaAux, int valor);
int excluir (struct Lista *pListaAux, int valor);
int imprimir (struct Lista *pListaAux);
struct no* noGuloso (struct Lista *pListaAux);
struct no* buscaNo (struct Lista *pListaAux, int valor);

int main (){
	struct Lista *pLista;
	struct no *proxAmeba, *pNoAux;
	char opaux = 1;
	char op;
	int valorno;
  	
  	pLista = (struct Lista *) malloc (sizeof(struct Lista));
	if(!pLista) return -1;
  	pLista->primeiro = NULL;
	
	while(opaux){
		printf("********************************************************\n");
		printf("******* Menu de opcoes para o Programa de Grafo ********\n");
		printf("********************************************************\n\n");
		printf("I - para incluir um no\n");
		printf("E - para excluir um no\n");
		printf("P - para impressao tradicional\n");
		printf("G - para impressao gulosa\n");
		printf("S - para sair\n\n");
		printf("Escolha uma das opcoes acima:\n\n");
		__fpurge(stdin);
		opaux = getchar();
		putchar(opaux);
				
		switch (opaux){
			case 'i':
			case 'I':
				printf(">>>>>>>>>>>>>>>>>     Inclusao     <<<<<<<<<<<<<<<<<<<\n\n");
				printf("Digite 0 para sair\n");
				printf("Insira o valor do no a ser incluido: ");
				scanf("%d", &valorno);
				incluir(pLista, valorno);
			break;
			case 'e':
			case 'E':
				printf(">>>>>>>>>>>>>>>>>     Exclusao     <<<<<<<<<<<<<<<<<<<\n\n");
				printf("Insira o valor do no a ser excluido: ");
				scanf("%d", &valorno);
				excluir(pLista, valorno);
				printf("\n");
			break;
			case 'p':
			case 'P':
				printf(">>>>>>>>>>>>     Impressao Tradicional     <<<<<<<<<<<\n\n");
				imprimir(pLista);
			break;
			case 'g':
			case 'G':
				printf(">>>>>>>>>>>>>>     Impressao Gulosa     <<<<<<<<<<<<<<\n\n");
				pNoAux = NULL;
				while (!pNoAux) {
					printf("Insira o valor do no inicial: ");
					scanf("%d", &valorno);
					pNoAux = buscaNo(pLista, valorno);
					if(!pNoAux) printf("\nNo nao encontrado.\n\n");
				}
				pNoAux->cVis = VISITADO;
				printf("\n%d ", pNoAux->valor);
				while ((proxAmeba = noGuloso(pLista))){
					proxAmeba->cVis = VISITADO;
					printf("%d ", proxAmeba->valor);
				}
				for(pNoAux = pLista->primeiro; pNoAux; pNoAux = pNoAux->prox) pNoAux->cVis = NAO_VISITADO;
				printf("\n\n");
			break;
			case 's':
			case 'S':
				printf("programa encerrado.\n");
				return 0;				
			default:
				printf("\nOpção não encontrada\n");
			break;
		}
	}
	return 0;
}

int incluir (struct Lista *pListaAux, int valor){
	
	struct no *pAuxIn;
	struct adj *pViz;
	int i, nViz, custo;
		
	for(pAuxIn = pListaAux->primeiro; (pAuxIn)&&(pAuxIn->valor != valor); pAuxIn = pAuxIn->prox); 
	if(pAuxIn){
		printf("nó já existe\n");
		return -5;
	}
	else{
		pAuxIn = (struct no *) malloc (sizeof(struct no));
		
		if(!pAuxIn) return -6;
		else{
			pAuxIn->valor = valor;
			pAuxIn->prox = pListaAux->primeiro;
			pListaAux->primeiro = pAuxIn;
			pAuxIn->cVis = NAO_VISITADO;
			pAuxIn->ini = NULL;
			
			printf("Insira o numero de vizinhos para o no %d: ", pAuxIn->valor);
			
			scanf("%d", &nViz);
			printf("\n");
			
			for(i=0; nViz && i<nViz; i++){
				printf("%do vizinho: ", i+1);
				scanf("%d", &valor);
				printf("Informe o custo da vizinhança");
				scanf("%d", &custo);
				
				for(pAuxIn = pListaAux->primeiro; (pAuxIn)&&(pAuxIn->valor != valor); pAuxIn = pAuxIn->prox);
					
				if(!pAuxIn){
					printf("\nO vizinho nao existe.\n\n");
					i--;
				}
				else{
					if(!(pViz = (struct adj *) malloc (sizeof(struct adj)))) return -6;
					pViz->viz = pAuxIn;
					pViz->prox = pListaAux->primeiro->ini;
					pListaAux->primeiro->ini = pViz;
					pViz->custo = custo;
					if(!(pViz = (struct adj *) malloc (sizeof(struct adj)))) return -6;
					pViz->viz = pListaAux->primeiro;
					pViz->prox = pAuxIn->ini;
					pAuxIn->ini = pViz;
					pViz->custo = custo;			
				}
			}
			return 0;
		}
			
	}
	return 0;
}

int excluir (struct Lista *pListaAux, int valor){
	
	struct no *pAuxEx, *pAuxEx2;
	struct adj *pViz, *pViz2, *pViz3;
		
	for(pAuxEx = pListaAux->primeiro, pAuxEx2 = NULL; (pAuxEx)&&(pAuxEx->valor != valor); pAuxEx2 = pAuxEx, pAuxEx = pAuxEx->prox);
	
	if(!pAuxEx) return -5;
	else{
		for(pViz = pAuxEx->ini; pViz; pViz = pViz->prox){
			for(pViz2 = pViz->viz->ini, pViz3 = NULL; pViz2->viz->valor != pAuxEx->valor; pViz3 = pViz2, pViz2 = pViz2->prox);
			if(pViz3) pViz3->prox = pViz2->prox; /* exclusao no meio ou fim (adj) */
			else pViz->viz->ini = pViz2->prox; /* exclusao no inicio (adj) */
			free(pViz2);
		}
		
		if(pAuxEx2) pAuxEx2->prox = pAuxEx->prox;/* exclusao no meio ou fim (no) */
		else pListaAux->primeiro = pListaAux->primeiro->prox; /* exclusao no inicio (no) */
		for(pViz = pAuxEx->ini; pViz; pViz = pViz2){
			pViz2 = pViz->prox;
			free(pViz);
		}
		free(pAuxEx); 
	}
	printf("\n");
	return 0;
}

int imprimir (struct Lista *pListaAux){
	struct no *pAuxImp;
	struct adj *pViz;	
	if(!(pListaAux->primeiro)) return -3;
	else{
		for(pAuxImp = pListaAux->primeiro; pAuxImp; pAuxImp = pAuxImp->prox){
			printf("[ %d ]", pAuxImp->valor);
			for(pViz = pAuxImp->ini; pViz; pViz = pViz->prox){
				if (pViz->viz) printf(" --> %d ", pViz->viz->valor);
			}
			printf("\n");
		}
	}
	printf("\n");
	return 0;
}

struct no* noGuloso (struct Lista *pListaAux){
	struct no *pNoAux, *pMax;
	struct adj *pAdjAux;
	int max;
	
	pMax = NULL;
	max = -1;
	
	for(pNoAux = pListaAux->primeiro; pNoAux; pNoAux = pNoAux->prox){
		if (pNoAux->cVis == VISITADO) {
			for(pAdjAux = pNoAux->ini; pAdjAux; pAdjAux = pAdjAux->prox){
				if ((pAdjAux->viz->cVis != VISITADO) && (pAdjAux->custo > max)){
					max = pAdjAux->custo;
					pMax = pAdjAux->viz;
				}
			}
		}
	}
	return pMax;
}

struct no* buscaNo (struct Lista *pListaAux, int valor){
	struct no *pAux;	
	for(pAux = pListaAux->primeiro; (pAux)&&(pAux->valor != valor); pAux = pAux->prox);
	return pAux;
}
