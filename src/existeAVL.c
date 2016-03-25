int existeAVL(AVL arv, ValorNodo val){
	int existe = 0;
	int r_compara; /* resultado de uma comparação */
	AVL_NODO *nodo_atual = arv->raiz;
	
	while(!existe && nodo_atual){
		r_compara = arv->compara(nodo_atual->valor, val);
		
		if(r_compara < 0) /* procura na subárvore esquerda */
			nodo_atual = nodo_atual->esquerda;
		else if(r_compara > 0) /* procura na subárvore direita */
			nodo_atual = nodo_atual->direita;
		else
			existe = 1;
	}

	return existe;
}