#include "catalogoClientes.h"
#include "avl.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */

/* número de AVLs usadas no catálogo */
#define calculaPos(c) ((inicioCodigoCliente((c))) -'A')
#define MAX_AVL 26 

struct catClientes {
	AVL catalogo[MAX_AVL];
};

struct conjClientes {
	int total;
	Cliente * clientes;
	int pag;
	int i;
};

static int compara(const void *, const void *);
static void* duplica(const void *);

CatClientes criaCatClientes() {
	int i;
	CatClientes catC = malloc(sizeof(struct catClientes));
	
	if(catC) 
		for(i = 0; i < MAX_AVL; ++i)
			catC->catalogo[i] = criaAVL(compara, duplica);
	return catC;
}

CatClientes insereCliente(CatClientes catC, Cliente c)
{
	int i = calculaPos(c);
	catC->catalogo[i] = insere(catC->catalogo[i], c);
	return catC;
}

bool existeCliente(CatClientes catC, Cliente c)
{
	bool existe = FALSE;

	if(catC != NULL){
		int i = calculaPos(c);
		existe = existeAVL(catC->catalogo[i], c);
	}
	return existe;
}

int totalClientesLetra(CatClientes catC, char l)
{
	int i = isalpha(l) ? toupper(l) - 'A' : -1;
	return (i == -1) ? 0 : tamanho(catC->catalogo[i]);
}

int totalClientes(CatClientes catC)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; ++i)
		total += tamanho(catC->catalogo[i]);

	return total;
}

void removeCatClientes(CatClientes catC)
{
	int i;
	for(i = 0; i < MAX_AVL; ++i)
		apagaAVL(catC->catalogo[i]);
	free(catC); 
}

/* Funções que manipulam conjuntos de clientes */

ConjClientes criaConjClientes(int total, Cliente* clientes) {
	ConjClientes conjuntoC = malloc(sizeof(struct conjClientes));

	if(conjuntoC){
		conjuntoC->total = total;
		conjuntoC->clientes = clientes;
		conjuntoC->pag = 1;
		conjuntoC->i = 1;
	}
	return conjuntoC;
}

void removeConjClientes(ConjClientes conjuntoC) {
	int i, total;

	total = conjuntoC->total;
	for(i = 0; i < total; ++i)
		removeCliente(conjuntoC->clientes[i]);
}

/* Em casos de sucesso, a função 'obterCodigosC' devolve um array de strings 
 * ordenado crescentemente, com os códigos de produtos de um conjunto de produtos. 
 * Se ocorrer um erro, é devolvido NULL */
char** obterCodigosC(ConjClientes conjuntoC) {
	int total = conjuntoC->total;
	char** codigos = malloc(total * sizeof(char *));
	
	if(codigos != NULL){
		int i = 0;
		char* codigoC;
		
		for(i = 0; i < total; ++i){
			codigoC = obterCodigoCliente(conjuntoC->clientes[i]);
			if(codigoC == NULL)
				break; /* se houve uma falha de alocação, saimos do ciclo */
			codigos[i] = codigoC;
		}
		if(i < total){ /* tratamento de falhas de alocação */
			for(i = i-1; i >= 0; --i)
				free(codigos[i]);
			codigos = NULL;
		}
	}
	return codigos;
}

int cardinalidade(ConjClientes conjuntoC) {
	return conjuntoC->total;
}

int obterPag(ConjClientes conjuntoC) {
	return conjuntoC->pag;
}

int obterIndice(ConjClientes conjuntoC) {
	return conjuntoC->i;
}

ConjClientes clientesPorLetra(CatClientes catC, char l) {	
	ConjClientes conjuntoC = NULL;

	if(isupper(l)){ 
		int i = l - 'A'; /* índice da avl a consultar */
		int total = tamanho(catC->catalogo[i]);
		Cliente* clientes = (Cliente*) inorder(catC->catalogo[i]);;
		
		conjuntoC = criaConjClientes(total, clientes); 
	}
	return conjuntoC;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Cliente */
static int compara(const void *cliente1, const void *cliente2) {	
	char* codigo1 = obterCodigoCliente((Cliente) cliente1);
	char* codigo2 = obterCodigoCliente((Cliente) cliente2);
	int r = strcmp(codigo1, codigo2);

	free(codigo1); 
	free(codigo2);

	return r;
}

/* Cria um duplicado de um Cliente */
static void* duplica(const void *cliente) {
	char* codigo = obterCodigoCliente((Cliente) cliente);
	Cliente dup = criaCliente(codigo);

	free(codigo);
	return (void*) dup;
}