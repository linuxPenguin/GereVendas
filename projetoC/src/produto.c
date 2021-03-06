#include "produto.h"
#include <stdlib.h>
#include <string.h>

struct produto {
	char* codigoProduto;
};

Produto criaProduto(char* codigoProduto)
{
	Produto novo = malloc(sizeof(struct produto));
	int len;

	if(novo == NULL) /* falha de alocação */
		return NULL;
	
	len = strlen(codigoProduto);
	novo->codigoProduto = malloc((len + 1) * sizeof(char));
	
	if(novo->codigoProduto != NULL)
		strcpy(novo->codigoProduto, codigoProduto);
	else{ /* falha de alocação de novo->codigoProduto */
		free(novo);
		novo = NULL;
	}
	return novo;
}

Produto duplicaProduto(Produto p)
{
	Produto copia = malloc(sizeof(struct produto));
	int len;

	if(copia == NULL) /* falha de alocação */
		return NULL;

	len = strlen(p->codigoProduto);
	copia->codigoProduto = malloc((len + 1) * sizeof(char));
	
	if(copia->codigoProduto != NULL)
		strcpy(copia->codigoProduto, p->codigoProduto);
	else{
		free(copia);
		copia = NULL;
	}
	return copia;
}

Produto apagaProduto(Produto p)
{
	if(p){
		free(p->codigoProduto);
		free(p);
	}
	return NULL;
}

char* obterCodigoProduto(Produto prod){
	int len = strlen(prod->codigoProduto);
	char* copiaCodigo = malloc((len + 1) * sizeof(char));
	
	if(copiaCodigo)
		strcpy(copiaCodigo, prod->codigoProduto);
	return copiaCodigo;
}

char inicioCodigoProduto(Produto prod)
{
	return prod->codigoProduto[0];
}

int comparaCodigosProduto(Produto p1, Produto p2)
{
	return strcmp(p1->codigoProduto, p2->codigoProduto);
}
