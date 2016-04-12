#include "faturacaoGlobal.h"
#include "avl.h"
#include <stdlib.h>
#include <string.h> /* para utilizar a função memcpy() */

/* !! Decidir onde vamos colocar estas defnições */
#define N_FILIAIS 3
#define N_MESES 12

/* !! Decidir se vale a pena validar meses */
#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)

/* Estruturas de dados do módulo:
		        		
	 _FaturacaoGlobal_______________	   __FatAnualProd_______________
	|								|	  |								|
	|  AVL todosProdutos;			| ==> |	Produto prod;				|
	|  FatMes fatMensal[13];		| 	  |	int totalVendas[N_FILIAIS];	|
	|_______________________________|	  |_____________________________|
				 
				 ||
				 \/	
	 _FatMes_______________________
	|                              |
	|  int totalVendas;            |
	|  int totalFaturado;          |
	|  AVL fatProds;               |
	|______________________________|

				 ||
				 \/

 	 _FatMensalProd________________
	|                              |
	|  Produto prod;               |
	|  int vendas[2][3]            |
	|  double faturacao[2][3]      |
	|______________________________|
*/

/* Guarda informação sobre a faturação de um produto (num dado mês) */
struct fatMensalProd{
	Produto prod;
	int vendas[N_TIPOS_VENDA][N_FILIAIS];
	double faturacao[N_TIPOS_VENDA][N_FILIAIS];
};

/* Guarda informação sobre a faturação de um dado mês */
typedef struct fatMes{
	int totalVendas;
	double totalFaturado;
	AVL fatProds;
}* FatMes;

/* 
 * Armazena, para um dado produto, informação sobre o seu
 * total de vendas anuais em cada uma das filiais
 */
typedef struct fatAnualProd{
	Produto prod;
	int totalVendas[N_FILIAIS];
}* FatAnualProd;

/* Faturação global */
struct fatGlobal{
	/* referencia todos os produtos (mesmo os não vendidos). Permite saber qual
	 * foi o total de vendas anuais de cada produto, globalmente ou filial a filial */
	AVL todosProdutos;
	/* guarda, para cada mês, informação quantitiva sobre as vendas e produtos
	 * vendidos nesse mês (produtos não vendidos não são referenciados) */
	FatMes fatMensal[N_MESES+1]; 
};

/* Funções privadas ao módulo */

static FatAnualProd criaFatAnualProd(Produto p);
static FatMensalProd criaFatMensalProd(
	Produto p, 
	int quantidade, 
	double totalFaturado,
	TipoVenda tipo, 
	int filial
);
static Produto** alocaArrayNaoComprados(int tamanho);

/* Funções utilizadas na criação de AVLs */
static int comparaFatMensalProd(const void* , const void* );
static int comparaFatAnualProd(const void* , const void* );
static void atualizaFatMensalProd(void *, void* );
static void atualizaFatAnualProd(void* , void* );

FaturacaoGlobal criaFaturacaoGlobal()
{
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatGlobal));
	
	if(fg != NULL){
		fg->todosProdutos = criaAVLgenerica(comparaFatAnualProd, atualizaFatAnualProd);
		for(i = 1; i <= N_MESES; i++){
			fg->fatMensal[i]->totalVendas = 0;
			fg->fatMensal[i]->totalFaturado = 0;
			fg->fatMensal[i]->fatProds = criaAVLgenerica(comparaFatMensalProd, atualizaFatMensalProd);
		}
	}
	return fg; /* se o malloc() falhou, é retornado NULL. Na main decidimos o que fazer */
}

/* Regista um produto na faturação global, guardando-o na AVL de total de vendas
 * anuais com o total de vendas igual a 0 para cada uma das filiais */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p)
{	
	FatAnualProd fAnualProd = criaFatAnualProd(p);

	if(fAnualProd != NULL)
		fg->todosProdutos = insere(fg->todosProdutos, fAnualProd);
	else /* falha de alocação a criar FatAnualProd */
		fg = NULL;
	return fg;
}

/* Cria a estrutura de dados que guarda o total de
 * vendas anuais de um produto, para cada filial. */
static FatAnualProd criaFatAnualProd(Produto p)
{
	FatAnualProd fAnualProd = calloc(1, sizeof(struct fatAnualProd));

	if(fAnualProd != NULL)
		fAnualProd->prod = p; /* !! discutir se vale a pena guardar uma cópia */
	return fAnualProd;
}


/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(
	FaturacaoGlobal fg,
	Produto p,
	double preco_unit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes)
{
	double totalFaturado = quantidade * preco_unit;
	FatMensalProd fatP = criaFatMensalProd(p, quantidade, totalFaturado, tipo, filial);

	if(fatP != NULL){
		FatMes fatMes = fg->fatMensal[mes];
		fatMes->fatProds = insere(fatMes->fatProds, (ValorNodo) fatP);
		/* Atualiza valores totais do mês */
		fatMes->totalVendas += quantidade;
		fatMes->totalFaturado += totalFaturado;
	}
	else /* falha de alocação ao criar a faturação do produto */
		fg = NULL;
	return fg;
}


/* Cria estrutura de dados com informação sobre a faturação de um produto */
static FatMensalProd criaFatMensalProd(
	Produto p,
	int quantidade,
	double totalFaturado,
	TipoVenda tipo,
	int filial)
{
	FatMensalProd fProd = malloc(sizeof(struct fatMensalProd));

	if(fProd != NULL){
		int i, j;

		fProd->prod = p;
		for(i = 0; i < N_TIPOS_VENDA; ++i){
			for(j = 0; j < N_FILIAIS; ++j){
				fProd->vendas[i][j] = 0;
				fProd->faturacao[i][j] = 0;
			}
		}
		fProd->vendas[tipo][filial] = quantidade;
		fProd->faturacao[tipo][filial] = totalFaturado;
	}
	return fProd;
}

/* Devolve informação sobre a faturação de um produto num dado mês */
FatMensalProd obterFatMensalProduto(const FaturacaoGlobal fg, const Produto p, int mes)
{	/* !! assumindo que o mês é válido */
	return (FatMensalProd) procuraAVL(fg->fatMensal[mes]->fatProds, p);
}

/* -> Funções usadas na query6 <- */

/* Devolve o total de vendas de um dado mês */
int totalVendasMes(const FaturacaoGlobal fg, int mes)
{
	return fg->fatMensal[mes]->totalVendas;
}

/* Devolve o total faturado num dado mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes)
{
	return fg->fatMensal[mes]->totalFaturado;
}

/* Devolve o total de vendas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes, total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalVendasMes(fg, mes);
	return total;
}

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes;
	double total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalFaturadoMes(fg, mes);
	return total;
}

/* -> Fim das funções usadas na query6 <- */

/* -> Funções usadas na query3 <- */

int* vendasPorFilial(FatMensalProd fProd, TipoVenda tipo)
{	
	int* copiaVendas;

	if(fProd == NULL) /* o produto foi vendido */
		copiaVendas = calloc(N_FILIAIS, sizeof(int));
	else{
		copiaVendas = malloc(N_FILIAIS * sizeof(int));
		if(copiaVendas != NULL)
			copiaVendas = memcpy(copiaVendas, fProd->vendas[tipo], N_FILIAIS * sizeof(int));
	}
	return copiaVendas;
}

int vendasTotais(FatMensalProd fProd, TipoVenda tipo)
{
	int total = 0;

	if(fProd != NULL){
		int i;

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProd->vendas[tipo][i];
	}
	return total;
}

double* faturacaoPorFilial(FatMensalProd fProd, TipoVenda tipo)
{	
	double* copiaFat;

	if(fProd == NULL) /* o produto não foi vendido */
		copiaFat = calloc(N_FILIAIS, sizeof(int));
	else{
		copiaFat = malloc(N_FILIAIS * sizeof(double));
		if(copiaFat != NULL)
			copiaFat = memcpy(copiaFat, fProd->faturacao[tipo], N_FILIAIS * sizeof(double));
	}	
	return copiaFat;
}

double faturacaoTotal(FatMensalProd fProd, TipoVenda tipo)
{
	double total = 0;

	if(fProd != NULL){
		int i;

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProd->faturacao[tipo][i];
	}
	return total;
}

/* -> Fim das funções utilizadas na query3 <- */

/* -> Início das funções usadas na query4 <- */

/* Indica se um produto não teve vendas anuais */

/* Devolve o número total de vendas anuais de um produto */
int obtemTotalVendasAnuais(FatAnualProd fAnualProd)
{
	int i;
	int total = 0;

	for(i = 0; i < N_FILIAIS; ++i)
		total += fAnualProd->totalVendas[i];
	return total;
}

bool naoComprado(FatAnualProd fAnualProd)
{	
	return obtemTotalVendasAnuais(fAnualProd) == 0;
}

/* Devolve conjunto ordenado com os códigos de produtos que
 * não forma comprados em nenhuma das filiais. */
ConjuntoProds naoCompradosGlobal(FaturacaoGlobal fg)
{	
	/* Aqui podiamos fazer um filter com o predicado 'semVendas' */
	int total;
	Produto* naoComprados;
	FatAnualProd* arrTodosProdutos;
	ConjuntoProds cProds = NULL;

	total = tamanho(fg->todosProdutos);
	naoComprados = malloc(total * sizeof(Produto)); /* 1.37MB */
	if(naoComprados != NULL){
		int i, j;
		Produto* novoNaoComprados; /* guarda retorno de realloc() para verificação de erros */
		
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		for(i = 0, j = 0; i < total; ++i)
			if(naoComprado(arrTodosProdutos[i]))
				naoComprados[j++] = arrTodosProdutos[i]->prod; /* !Possibilidade de criar cópia */
		
		novoNaoComprados = realloc(naoComprados, j * sizeof(Produto));
		if(novoNaoComprados == NULL){ /* falha de realocação */
			free(naoComprados);
			cProds = NULL;
		}
		else{
			naoComprados = novoNaoComprados;
			cProds = criaConjuntoProds(j, naoComprados);
		}
	}
	return cProds;
}

/* !! Esta função precisa de limpeza de código e de simplificações */
ConjuntoProds* naoCompradosPorFilial(FaturacaoGlobal fg)
{
	int total;
	Produto** naoComprados;
	FatAnualProd* arrTodosProdutos;
	ConjuntoProds* cProdsFiliais = malloc(N_FILIAIS * sizeof(ConjuntoProds));

	total = tamanho(fg->todosProdutos);
	naoComprados = alocaArrayNaoComprados(total);
	if(naoComprados != NULL && cProdsFiliais != NULL){
		int i, j;
		int inds[N_FILIAIS] = {0}; /* array que na posição j tem o índice da filial j+1 */
		
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		for(i = 0; i < total; ++i){ /* itera ao longo das vendas anuais */
			FatAnualProd fAnualProd = arrTodosProdutos[i];
			for(j = 0; j < N_FILIAIS; ++j){
				if(fAnualProd->totalVendas[j] == 0)
					naoComprados[j][inds[j]++] = fAnualProd->prod;
			}
		}

		/* não está a ser verificado o valor de retorno do realloc() */
		for(i = 0; i < N_FILIAIS; ++i){
			naoComprados[i] = realloc(naoComprados[i], inds[i] * sizeof(Produto));
			cProdsFiliais[i] = criaConjuntoProds(inds[i], naoComprados[i]);
		}
	}
	return cProdsFiliais;
}

/* !Discutir possibilidade de fazer uma função de alocação de arrays de arrays */
static Produto** alocaArrayNaoComprados(int tamanho)
{
	Produto** naoComprados = malloc(N_FILIAIS * sizeof(Produto *));

	if(naoComprados != NULL){
		int i = 0;

		for(i = 0; i < N_FILIAIS; ++i)
			naoComprados[i] = malloc(tamanho * sizeof(Produto));
			if(naoComprados[i] == NULL){ 
				/* falha da alocação */
			}
	}
	return naoComprados;
}

/* -> Fim das funções utilizadas na query4 <- */

/* -> Funções utilizadas na criação da AVLs <- */

static int comparaFatMensalProd(const void* p1, const void* p2)
{	
	return comparaCodigosProduto( ((FatMensalProd) p1)->prod, 
		                          ((FatMensalProd) p2)->prod );
}

static int comparaFatAnualProd(const void* p1, const void* p2)
{
	return comparaCodigosProduto( ((FatAnualProd) p1)->prod, 
		                          ((FatAnualProd) p2)->prod );
}

static void atualizaFatMensalProd(void* p1, void* p2)
{	
	int i, j;
	FatMensalProd atual = p1;
	FatMensalProd adicional = p2;

	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 0; j < N_FILIAIS; ++j){
			atual->vendas[i][j] += adicional->vendas[i][j];
			atual->faturacao[i][j] += adicional->faturacao[i][j];
		}
	}
	free(adicional);
}

static void atualizaFatAnualProd(void* p1, void* p2)
{	
	int i;
	FatAnualProd atual = p1;
	FatAnualProd adicional = p2;

	for(i = 0; i < N_FILIAIS; ++i)
		atual->totalVendas[i] += adicional->totalVendas[i];
}

/* -> Fim das funções utilizadas na criação de AVLs */