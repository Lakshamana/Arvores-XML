/*
## Universidade Federal da Bahia
## Docente: Danilo Azevedo
## Bacharelado em Ciencia da Computação - Estrutura de Dados II

Chaves testadas: 20,40,10,30,15,35,7,26,18,22,5,42,13,27,8,32,38,24,45,25.
*/

// Enviado por LaKshamana (@LaKshamana)

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define ORDEM 2
#define namefile "_arquivo.xml"


int cont = -1, count = 0;

typedef struct
{
  int chave;
  char nome[20];
  int idade;
  int ApPage[2];
  int rank;
} Registro;

typedef struct Pagina_str *Apontador;
typedef struct Pagina_str
{
  int n;
  int pageNum;
  int num;
  Registro r[2*ORDEM];
  Apontador p[2*ORDEM+1];
} Pagina;

struct Node{
    Registro info;
    struct Node *prox;
};
typedef struct Node node;

//##########################################################
void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu);
void Busca(Registro Reg, Apontador Ap);
void em_ordem(Apontador raiz);
void buscainFile(Registro Reg, Apontador pagina);
int isleaf(Apontador a);
int file_exists(const char *filename);
void Insere(Registro Reg, Apontador *Ap);
void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir);
void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
int Imprime2(Apontador p, int Nivel, int aux, int n);
int Imprime(Apontador p, int N, int aux, int n);
void InsertPosFile(Registro Reg, Apontador *Ap);
node *busca(Registro info, node *LISTA);
void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno);
void pos_ordem(Apontador raiz);
void Retira(int Ch, Apontador *Ap);
void Ret(int Ch, Apontador *Ap, int *Diminuiu);
void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu);
void saveAux(Apontador p, int Nivel);
void salvar(Apontador pagina, Registro Reg[]);
void recuperarReg(Apontador *arv, node *LISTA);
void insereInicio(Registro info, node *LISTA);
void execut(Registro info, node *LISTA);
void exibe(node *LISTA);

int menu_b(void){
    int op;
	printf("\n[MENU ARVORE B]\n");
    printf("1.Inserir NO......................:\n");
    printf("2.Remover NO......................:\n");
    printf("3.Imprimir em intra-Ordem.........:\n");
    printf("4.Buscar elemento.................:\n");
    printf("5.Imprimir de outra forma.........:\n");
    printf("0.Sair do programa ...............:\n\n");
    printf("-OPCAO: "); scanf("%d", &op);
    return op;
}

typedef Apontador TipoDicionario;
void Inicializa(TipoDicionario *Dicionario)
{
  *Dicionario = NULL;
}  /* Inicializa a estrutura */

void inicia(node *LISTA)
{
    LISTA->prox = NULL;
}

//############################### Estrutura 1 Btree

void Antecessor(Apontador Ap, int Ind, Apontador ApPai, int *Diminuiu)
{
  if (ApPai->p[ApPai->n] != NULL)
  {
    Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
    if (*Diminuiu)
      Reconstitui(ApPai->p[ApPai->n], ApPai, ApPai->n, Diminuiu);
    return;
  }
  Ap->r[Ind - 1] = ApPai->r[ApPai->n - 1];
  ApPai->n--;
  *Diminuiu = ApPai->n < ORDEM;
}  /* Antecessor */

void Busca(Registro Reg, Apontador Ap)
{
  int i;

  if (Ap == NULL) //
  {
    printf("chave nao encontrada: %d\n", Reg.chave);
    return;
  }
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    printf("chave: %d \n", Reg.chave);
    buscainFile(Ap->r[i-1],Ap);
    return;
  }
  if (Reg.chave < Ap->r[i - 1].chave)
    Busca(Reg, Ap->p[i-1]);
  else
    Busca(Reg, Ap->p[i]);
}

void buscainFile(Registro Reg, Apontador pagina)
{
    Registro reg[2*ORDEM];
    int i;
    FILE *arq = fopen(namefile,"rb");
    if (arq == NULL)
    exit(1);
    fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
    fread(reg, (2*ORDEM*sizeof(Registro)),1,arq);
    fclose(arq);
    for(i = 0; i < 2*ORDEM; i++){
        if (Reg.chave == reg[i].chave)
        printf("%s%d\n",reg[i].nome,reg[i].idade);
            }
    }

void em_ordem(Apontador raiz)
{
  int i;
  if (raiz != NULL)
   {
     for (i = 0; i < raiz->n; i++)
      {
        em_ordem(raiz->p[i]);
        printf("%d ", raiz->r[i].chave);
        printf("\n");
      }
     em_ordem(raiz->p[i]);
      }
}

int file_exists(const char *filename)
{
  FILE *arquivo;

  if((arquivo = fopen(filename, "rb")))
  {
    fclose(arquivo);
    return 1;
  }
  return 0;
}

int isleaf(Apontador a)
{
    if (a->p[0] == NULL)
    return 1;
    else
    return 0;
}

void InsereNaPagina(Apontador Ap, Registro Reg, Apontador ApDir)
{

  int k;
  int NaoAchouPosicao;

  k = Ap->n;
  NaoAchouPosicao = k > 0;
  while (NaoAchouPosicao)
  {
    if (Reg.chave >= Ap->r[k - 1].chave)
    {
      NaoAchouPosicao = 0;
      break;
    }

    Ap->r[k] = Ap->r[k - 1];

    Ap->p[k + 1] = Ap->p[k];

    k--;
    if (k < 1)
      NaoAchouPosicao = 0;
  }
  Ap->r[k] = Reg;
  Ap->p[k + 1] = ApDir;
  Ap->n++;
}

void Ins(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  if (Ap == NULL)
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    printf("chave ja existente: %d \n", Reg.chave);
    *Cresceu = 0;
    return;
  }
  if (Reg.chave < Ap->r[i - 1].chave)
    Ins(Reg, Ap->p[i-1], Cresceu, RegRetorno, ApRetorno);
  else
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
  if (!*Cresceu)
    return;
  if (Ap->n < 2*ORDEM)
  {  /* Verificando se a pagina tem espaco */
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = 0;
    return;
  }
  /* Split: Pagina tem que ser dividida */
  ApTemp = (Apontador) malloc(sizeof(Pagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2*ORDEM - 1], Ap->p[2*ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else{
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }
  for (j = ORDEM + 2; j <= 2*ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);


  Ap->n = ORDEM;
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  *RegRetorno = Ap->r[ORDEM];
  *ApRetorno = ApTemp;
 for (j = ORDEM; j < Ap->n+2; j++){
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
        }
}  /*Ins*/

void Insere(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu) { /* Se arvore cresce na altura pela raiz */
    ApTemp = (Apontador) malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
    //save in file
  }
  saveAux(*Ap, 2*ORDEM);
}  /*Insercao*/

int Imprime2(Apontador p, int Nivel, int aux, int n)
{
  int i;

  if (p == NULL)
    return 0;
  if(Nivel == aux){
      n++;
    printf("No: %d : ",p->pageNum);
  for (i = 0; i < p->n; i++){
      if (p->p[i] !=NULL)
    printf("Apontador: %d ", p->p[i]->pageNum);
    else
    printf("Apontador: null ");
    printf("chave: %d ", p->r[i].chave);
   }
   printf("\n");
   return n;
 }else{
    aux++;
     for (i = 0; i <= p->n; i++)
     Imprime2(p->p[i], Nivel, aux, n);
     }
    return 0;
}

int Imprime(Apontador p, int N, int aux, int n)
{
  int i;

  if (p == NULL)
    return 0;
  if(p->pageNum != aux){
    p->num = N;
    aux = p->pageNum;
    return aux;
 }else{
    aux++;
     for (i = 0; i <= p->n; i++)
     Imprime(p->p[i], N, aux, n);
     }
     return 0;
}

void InsertPosFile(Registro Reg, Apontador *Ap)
{
  int Cresceu;
  Registro RegRetorno;
  Apontador ApRetorno;
  Apontador ApTemp;

  InsPosFile(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu) { /* Se arvore cresce na altura pela raiz */
    ApTemp = (Apontador) malloc(sizeof(Pagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    cont++;
    ApTemp->pageNum = cont;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
  }
}

void InsPosFile(Registro Reg, Apontador Ap, int *Cresceu, Registro *RegRetorno, Apontador *ApRetorno)
{
  Apontador ApTemp;
  Registro Aux;
  int i, j;

  if (Ap == NULL) //
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }
  i = 1;
  while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    i++;
  if (Reg.chave == Ap->r[i - 1].chave)
  {
    //printf("chave ja existente: %d \n", Reg.chave);
    *Cresceu = 0;
    return;
  }
  if (Reg.chave < Ap->r[i - 1].chave)
    InsPosFile(Reg, Ap->p[i-1], Cresceu, RegRetorno, ApRetorno);
  else
    InsPosFile(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
  if (!*Cresceu)
    return;
  if (Ap->n < 2*ORDEM)
  {  /* Verificando se a pagina tem espaco */
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = 0;
    return;
  }
  /* Split: Pagina tem que ser dividida */
  ApTemp = (Apontador) malloc(sizeof(Pagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;
  cont++;
  ApTemp->pageNum = cont;

  if (i <= ORDEM + 1)
  {
    InsereNaPagina(ApTemp, Ap->r[2*ORDEM - 1], Ap->p[2*ORDEM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  }
  else{
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    }
  for (j = ORDEM + 2; j <= 2*ORDEM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);


  Ap->n = ORDEM;
  ApTemp->p[0] = Ap->p[ORDEM + 1];
  *RegRetorno = Ap->r[ORDEM];
  *ApRetorno = ApTemp;
 for (j = ORDEM; j < Ap->n+2; j++){
    Aux.chave = 0;
    Aux.rank = 0;
    Ap->r[j] = Aux;
    }
}

void pos_ordem(Apontador raiz)
{
  int i;
 if (isleaf(raiz)){
    for (i = raiz->n-1; i >=0; i--){
        printf("%d\n", raiz->r[i].chave);

     }
  }
  else{
    pos_ordem(raiz->p[raiz->n]);
    for (i = raiz->n-1; i >=0; i--){
        printf("%d\n", raiz->r[i].chave);
    pos_ordem(raiz->p[i]);
        }
    }
}

void Pesquisa(Registro *x, Apontador Ap)
{
  int i;

  if (Ap == NULL)
  {
    //printf("Erro: Registro nao existe\n");

    return;
  }
  i = 1;
  while (i < Ap->n && x->chave > Ap->r[i - 1].chave)
    i++;
  if (x->chave == Ap->r[i - 1].chave)
  {
    *x = Ap->r[i - 1];
    return;
  }
  if (x->chave < Ap->r[i - 1].chave)
    Pesquisa(x, Ap->p[i - 1]);
  else
    Pesquisa(x, Ap->p[i]);
} /* Pesquisa */

void Reconstitui(Apontador ApPag, Apontador ApPai, int PosPai, int *Diminuiu)
{
  Apontador Aux;
  int DispAux, j;

  if (PosPai < ApPai->n) {  /* Aux = Pagina a direita de ApPag */
    Aux = ApPai->p[PosPai + 1];
    DispAux = (Aux->n - ORDEM + 1) / 2;
    ApPag->r[ApPag->n] = ApPai->r[PosPai];
    ApPag->p[ApPag->n + 1] = Aux->p[0];
    ApPag->n++;
    if (DispAux > 0) {  /* Existe folga: transfere de Aux para ApPag */
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      ApPai->r[PosPai] = Aux->r[DispAux - 1];
      Aux->n -= DispAux;
      for (j = 0; j < Aux->n; j++)
        Aux->r[j] = Aux->r[j + DispAux];
      for (j = 0; j <= Aux->n; j++)
        Aux->p[j] = Aux->p[j + DispAux];
      *Diminuiu = 0;
    }
    else
    { /* Fusao: intercala Aux em ApPag e libera Aux */
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
      free(Aux);
      for (j = PosPai + 1; j < ApPai->n; j++)
      {   /* Preenche vazio em ApPai */
        ApPai->r[j - 1] = ApPai->r[j];
        ApPai->p[j] = ApPai->p[j + 1];
      }
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
  else
  { /* Aux = Pagina a esquerda de ApPag */
    Aux = ApPai->p[PosPai - 1];
    DispAux = (Aux->n - ORDEM + 1) / 2;
    for (j = ApPag->n; j >= 1; j--)
      ApPag->r[j] = ApPag->r[j - 1];
    ApPag->r[0] = ApPai->r[PosPai - 1];
    for (j = ApPag->n; j >= 0; j--)
      ApPag->p[j + 1] = ApPag->p[j];
    ApPag->n++;
    if (DispAux > 0) {  /* Existe folga: transfere de Aux para ApPag */
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
      ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
      ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
      Aux->n -= DispAux;
      *Diminuiu = 0;
    }
    else
    {  /* Fusao: intercala ApPag em Aux e libera ApPag */
      for (j = 1; j <= ORDEM; j++)
        InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
      free(ApPag);
      ApPai->n--;
      if (ApPai->n >= ORDEM)
        *Diminuiu = 0;
    }
  }
}  /* Reconstitui */

void Ret(int Ch, Apontador *Ap, int *Diminuiu)
{
  int Ind, j;
  Apontador WITH;
  Registro Reg;

  if (*Ap == NULL)
  {
    printf("chave nao encontrada: %i\n", Ch);
    *Diminuiu = 0;
    return;
  }
  WITH = *Ap;
  Ind = 1;
  while (Ind < WITH->n && Ch > WITH->r[Ind - 1].chave)
    Ind++;
  if (Ch == WITH->r[Ind - 1].chave)
  {
    Reg.chave = 0;
    Reg.rank = 0;
    WITH->r[Ind -1] = Reg;
    if (WITH->p[Ind - 1] == NULL) {  /* Pagina folha */
      WITH->n--;
      *Diminuiu = WITH->n < ORDEM;
      for (j = Ind; j <= WITH->n; j++)
      {
        WITH->r[j - 1] = WITH->r[j];
        WITH->p[j] = WITH->p[j + 1];
      }
      return;
    }
    Antecessor(*Ap, Ind, WITH->p[Ind - 1], Diminuiu);
    if (*Diminuiu)
      Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
    return;
  }
  if (Ch > WITH->r[Ind - 1].chave)
    Ind++;
  Ret(Ch, &WITH->p[Ind - 1], Diminuiu);
  if (*Diminuiu)
    Reconstitui(WITH->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
}  /* Ret */

void Retira(int Ch, Apontador *Ap)
{
  int Diminuiu;
  Apontador Aux;

  Ret(Ch, Ap, &Diminuiu);
  if (Diminuiu && (*Ap)->n == 0) { /* Arvore diminui na altura */
    Aux = *Ap;
    *Ap = Aux->p[0];
    free(Aux);

  }

}  /* Retira */

void recuperarReg(Apontador *arv, node *LISTA)
{

    FILE* arq;
    Registro Reg[2*ORDEM];
    node *tmp;
    int i=0,j=0, tam;
        arq = fopen(namefile,"rb");
        if (arq == NULL)
            exit(1);
       fseek(arq, 0, SEEK_END);
       tam = ftell(arq);
       rewind(arq);
       fclose(arq);

       arq = fopen(namefile,"rb");
        if (arq == NULL)
            exit(1);
        while (j*(2*ORDEM*sizeof(Registro)) < tam){
            fseek(arq, j*(2*ORDEM*sizeof(Registro)), SEEK_SET);
            fread(Reg, 2*ORDEM*sizeof(Registro),1,arq);

        for(i = 0; i < 2*ORDEM; i++){
           if (Reg[i].chave > 0)
           execut(Reg[i], LISTA);
        }
    j++;
    }
    fclose(arq);
    tmp = LISTA->prox;
    while( tmp != NULL){
        InsertPosFile(tmp->info, arv);
        tmp = tmp->prox;
    }
    free(tmp);
    free(LISTA);
    //Imprime(*arv, 2*ORDEM, 0);
    }

void saveAux(Apontador p, int Nivel)
{
  int i,j;

  if (p == NULL)
    return;
  for (i = 0; i < p->n; i++)
    salvar(p, p->r);
  for (j = 0; j <= p->n; j++)
    saveAux(p->p[j], Nivel + 1);
}

void salvar(Apontador pagina, Registro Reg[])
{

    FILE* arq;
    if (!file_exists(namefile)){
        arq = fopen(namefile,"wb");
        if (arq == NULL)
            exit(1);
        fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
        fwrite(Reg,(2*ORDEM*sizeof(Registro)),1,arq);
        fclose(arq);
    }

    else{
        arq = fopen(namefile,"r+b");
        if (arq == NULL)
            exit(1);
        fseek(arq, pagina->pageNum*(2*ORDEM*sizeof(Registro)), SEEK_SET);
        fwrite(Reg, (2*ORDEM*sizeof(Registro)),1,arq);
        fclose(arq);
        }
    }

//############################### //Estrutura 1 Btree

//############################### //Estrutura 2 lista encadeada
void insereInicio(Registro info, node *LISTA)
{
    node *novo=(node *) malloc(sizeof(node));
    node *tmp = busca(info, LISTA);
    if(LISTA->prox == NULL){
        novo->info = info;
        novo->prox = LISTA->prox;
        LISTA->prox = novo;
    }else{
    novo->prox = tmp->prox;
    novo->info = info;
    tmp->prox = novo;
}
//exibe(LISTA);
}

node *busca(Registro info, node *LISTA)
{
        node *atual,*antNode;
        atual = LISTA->prox;
        antNode = LISTA;
        while(atual != NULL && info.rank > atual->info.rank){
            antNode = atual;
            atual = atual->prox;
        }
        return antNode;
    }

void exibe(node *LISTA)
{
    if(!LISTA){
        return ;
    }
    node *tmp;
    tmp = LISTA->prox;

    while( tmp != NULL){
        printf(" %d ", tmp->info.chave);
        tmp = tmp->prox;
    }
    printf("\n");
}

void execut(Registro info, node *LISTA)
{
    insereInicio(info, LISTA);
    }

void MENU_B(){
	Apontador *arv;
  Registro reg;
  int tecla;
  int chave, i, num = 0;
  arv=(Apontador*) malloc(sizeof(Apontador));
  node *LISTA = (node *) malloc(sizeof(node));
  Inicializa(arv);
  inicia(LISTA);
    if (file_exists(namefile)){
        recuperarReg(arv, LISTA);
        }

  do
  {
  	tecla = menu_b();
	switch(tecla)
    {
    case 1:
      //printf("insercao \n");
        scanf("%d", &chave);
        reg.chave = chave;
        count++;
        reg.rank = count;
        fflush(stdin);
        getchar();
        fgets(reg.nome, sizeof(reg.nome), stdin);
        scanf("%d", reg.idade);
        if (reg.chave <= 0){
            count--;
            break;
        }
        Insere(reg,arv);
      	break;
    case 2:
        scanf("%d", &chave);
		reg.chave = chave;
        Retira(reg.chave, arv);
        remove(namefile);
        saveAux(*arv,2*ORDEM);
        break;
    case 3:
        em_ordem(*arv);
        break;
    case 5:
      	for(i=0; i < 2*ORDEM; i++){
            num = Imprime2(*arv, i, 0, num);
     	}
     	break;
    case 4:
      	getchar();
    	scanf("%d", &chave);
      	reg.chave = chave;
      	Busca(reg,*arv);
      	break;
    case 0:
		break;  	
    default:
		printf("Comando invalido.. tente novamente.");
		break;  	
    }

    fflush(stdin);
  }while(tecla!=0);
}



// Fim da Árvore B ########################################################################################################################################

//Árvore-Binária ##########################################################################################################################################
//enviado por Rafael Sousa (@RafaSou)

typedef struct ARVORE_BINARIA{
	struct ARVORE_BINARIA *dir,*esq;
	int valor;
}NB;

// Menu do Usuario ////////////////////////////////////////////////////////////////////////////////////////
int menu_bi(void){
    int op;
	printf("\n[MENU ARVORE BINARIA]\n");
    printf("1.Inserir NO......................:\n");
    printf("2.Imprimir Arvore Pre-Ordem.......:\n");
    printf("3.Imprimir Arvore Intra-Ordem.....:\n");
    printf("4.Imprimir Arvore Pos-Ordem.......:\n");
    printf("5.Remover NO......................:\n");
    printf("0.Sair do programa ...............:\n\n");
    printf("-OPCAO: "); scanf("%d", &op);
    return op;
}


// Insere os elementos de acordo com a regra de balanceamento /////////////////////////////////////////////
void inserir_bi(NB **ptr, int x){
	NB *p=(NB*)malloc(sizeof(NB));
	p->valor=x;
	p->esq=p->dir=NULL;

	if(*ptr==NULL){
		*ptr=p;
	}else{
		if( x < (*ptr)->valor){
			inserir_bi(&(*ptr)->esq, x);
		}
		if( x > (*ptr)->valor){
			inserir_bi(&(*ptr)->dir, x);
		}
	}
}

// Imprimi a Árvore em Pre-Ordem //////////////////////////////////////////////////////////////////////////
void pre_ordem_bi(NB *arv){
	printf("<");
	if(arv!=NULL){
		printf("%d",arv->valor);
		pre_ordem_bi(arv->esq);
		pre_ordem_bi(arv->dir);
	}
	printf(">");

}
// Imprimi a Árvore em intra-Ordem ////////////////////////////////////////////////////////////////////////
void intra_ordem_bi(NB *arv){
	printf("<");
	if(arv!=NULL){
		intra_ordem_bi(arv->esq);
		printf("%d",arv->valor);
		intra_ordem_bi(arv->dir);
	}
	printf(">");
}

// Imprimi a Árvore em Pós-Ordem //////////////////////////////////////////////////////////////////////////
void pos_ordem_bi(NB *arv){
	printf("<");
	if(arv!=NULL){
		pos_ordem_bi(arv->esq);
		pos_ordem_bi(arv->dir);
		printf("%d",arv->valor);
	}
	printf(">");
}

// Acha o NÓ com o maior valor ////////////////////////////////////////////////////////////////////////////
NB* maximo_bi(NB *arv){
	if(arv!=NULL){
		while(arv->dir!=NULL){
			arv=arv->dir;
		}
	}
	return arv;
}

// Acha o NÓ com o menor valor ////////////////////////////////////////////////////////////////////////////
NB* minimo_bi(NB *arv){
	if(arv!=NULL){
		while(arv->esq!=NULL){
			arv=arv->esq;
		}
	}
	return arv;
}

// Remover um NÓ dá Árvore ////////////////////////////////////////////////////////////////////////////////
void remover_bi(NB **arv, int x){
	NB *aux;
	if((*arv)!=NULL){
		if(x < (*arv)->valor){
			remover_bi(&((*arv)->esq), x); // faz recursividade á esquerda
       }else{
            // se o valor que será removido for maior que o nó atual,
            if(x > (*arv)->valor){
                 remover_bi(&((*arv)->dir), x); // faz recursividade á direita.
             }else{ // encontrou
                // quando o nó a ser removido for encontrado,
                if(((*arv)->esq)!=NULL && ((*arv)->dir)!=NULL ){
                     // verificamos se os nós filhos da esquerda e direita não são null.
                     // se não forem null, buscamos o menor nó a partir do nó da direita.
                     aux = minimo_bi((*arv)->dir);
                     (*arv)->valor = (aux->valor);
                     remover_bi(&(*arv)->dir, (*arv)->valor);
                }else{
                       // caso os nó da direita e da esqueda, ou somente o da direita for null,
                       // precisamos apenas remover
                       aux = *arv;
                       // o nó atual e fazer ajustar os ponteiros
                       if(((*arv)->esq)==NULL){
                           // se o nó da esquerda for vazio
                           // o nó pai do atual, apontará para o filho da direita do nó atual.
                           *arv = (*arv)->dir;
                       }
                       else {
                            // se o nó da esquerda não for vazio.
                            // o nó pai do atual, apontará para o filho da esquerda do nó atual.
                            *arv = (*arv)->esq;
                       }
                free(aux);
                }
            }
       }
    }
}

void MENU_BINARIA(){
	NB *arv=NULL;
	int add,rmv,opc;

	do{
        opc = menu_bi();
        switch (opc){

        case 1:
        	printf("Informe um numero para inserir na Arvore: "); scanf("%d", &add);
        	inserir_bi(&arv, add);
            break;
        case 2:
            printf("Listagem dos ptrs da Arvore em Pre-Ordem: \n\n");
            if(arv != NULL){
                pre_ordem_bi(arv);
            }else printf("Sem Elementos na Arvore. \n");
            break;
        case 3:
            printf("Listagem dos ptrs da Arvore em Intra-Ordem: \n\n");
            if(arv != NULL){
                pre_ordem_bi(arv);
            }else printf("Sem Elementos na Arvore. \n");
            break;
        case 4:
            printf("Listagem dos ptrs da Arvore em Pos-Ordem: \n\n");
            if(arv != NULL){
                pos_ordem_bi(arv);
            }else printf("Sem Elementos na Arvore. \n");
            break;
		case 5:
			printf("Informe um numero para remover da Arvore: "); scanf("%d", &rmv);
        	remover_bi(&arv, rmv);
			break;
		case 0:
			break;	
        default:
      		printf("Comando INVALIDO!!\n");
      		break;
        }
    }
    while (opc != 0);
}


//Fim da Àrvore-Binária ####################################################################################################################################

//Árvore-Rubro-Negra #######################################################################################################################################
//Enviado por Rafael Sousa (@RafaelSou)

typedef struct ARVORE{
    int numero_arvore;
    char cor;
    struct ARVORE *pai, *esquerda, *direita;
}NO;


NO *vazio = NULL;
// Rotação Simples a Esquerda do Nó ///////////////////////////////////////////////////////////////////////
void RSE(NO **Arv, NO *noh){
    NO *y;

    if(((*Arv)->pai == vazio) && (noh->direita != vazio)){
        y = noh->direita;
        noh->direita = y->esquerda;
        y->esquerda->pai = noh;
        y->pai = noh->pai;

        if(noh->pai == vazio){
            *Arv = y;
        }
        else if(noh == noh->pai->esquerda){
            noh->pai->esquerda = y;
        }
        else noh->pai->direita = y;
    }
    y->esquerda = noh;
    noh->pai = y;
    (*Arv)->pai = vazio;
}

//Rotação Simples a Direita do NÓ//////////////////////////////////////////////////////////////////////////
void RSD(NO **Arv, NO *noh){
    NO *y;

    if(((*Arv)->pai == vazio) && (noh->esquerda != vazio)){
        y = noh->esquerda;
        noh->esquerda = y->direita;
        y->direita->pai = noh;
        y->pai = noh->pai;

        if(noh->pai == vazio){
            *Arv = y;
        }
        else if(noh == noh->pai->direita){
            noh->pai->direita = y;
        }
        else noh->pai->esquerda = y;
    }
    y->direita = noh;
    noh->pai = y;
    (*Arv)->pai = vazio;
}

// Insere a Cor do NÓ e faz o balaceamento caso necessário ////////////////////////////////////////////////
void inserirCor(NO **Arv, NO *noh){
    NO *y;

    while(noh->pai->cor == 'V'){
        if(noh->pai == noh->pai->pai->esquerda){
            y = noh->pai->pai->direita;

            if(y->cor == 'V'){
                noh->pai->cor = 'P';
                y->cor = 'P';
                noh->pai->pai->cor = 'V';
                noh = noh->pai->pai;
            }
            else{
                if(noh == noh->pai->direita){
                    noh = noh->pai;
                    RSE(&(*Arv), noh);
                }

                noh->pai->cor = 'P';
                noh->pai->pai->cor = 'V';
                RSD(&(*Arv), noh->pai->pai);
            }
        }
        else{
            y = noh->pai->pai->esquerda;

            if(y->cor == 'V'){
                noh->pai->cor = 'P';
                y->cor = 'P';
                noh->pai->pai->cor = 'V';
                noh = noh->pai->pai;
            }
            else{
                if(noh == noh->pai->esquerda){
                    noh = noh->pai;
                    RSD(&(*Arv), noh);
                }

                noh->pai->cor = 'P';
                noh->pai->pai->cor = 'V';
                RSE(&(*Arv), noh->pai->pai);
            }
        }
    }

    (*Arv)->cor = 'P';
}

// Insere um novo NÓ na Árvore ////////////////////////////////////////////////////////////////////////////
NO* inserir(NO **Arv, NO *noh){ 
    NO *y = vazio;
    NO *x = *Arv;

    if((*Arv)== NULL){// Verifica se a Árvore é NULL
        *Arv = noh;
        (*Arv)->pai = vazio;
        vazio->pai = *Arv;
        (*Arv)->cor = 'P';
        return;
    }

    if((*Arv)->numero_arvore == noh->numero_arvore){
        printf(" Numero duplicado, Coloque outro numero !!\n\n");
        return;
    }

    while(x != vazio){
        y = x;
        if(noh->numero_arvore < x->numero_arvore){
            x = x->esquerda;
        }
        else x = x->direita;
    }

    noh->pai = y;

    if(noh->numero_arvore < y->numero_arvore){
        y->esquerda = noh;
    }
    else if(noh->numero_arvore > y->numero_arvore){
        y->direita = noh;
    }
    else if(noh->numero_arvore == y->numero_arvore){
        printf("Numeros iguais, Tente outro numero !!\n\n");
    }

    noh->esquerda = vazio;
    noh->direita = vazio;
    noh->cor = 'V';

    inserirCor(&(*Arv), noh);
    return noh;
}


// Faz o balaceamento da cor caso tenha alguma incosistencia //////////////////////////////////////////////
void removerCor(NO **Arv, NO *noh){
    NO *aux;

    while(((*Arv) != noh) && (noh->cor == 'P')){
        if(noh == noh->pai->esquerda){
            aux = noh->pai->direita;

            if(aux->cor == 'V'){
                aux->cor = 'P';
                noh->pai->cor = 'V';
                RSE(&(*Arv), noh->pai);
                aux = noh->pai->direita;
            }

            if((aux->esquerda->cor == 'P') && (aux->direita->cor == 'P')){
                aux->cor = 'V';
                noh = noh->pai;
            }else if(aux->direita->cor == 'P'){
                aux->esquerda->cor = 'P';
                aux->cor = 'V';
                RSD(&(*Arv), aux);
                aux = noh->pai->direita;
                aux->cor = noh->pai->cor;
                noh->pai->cor = 'P';
                aux->direita->cor = 'P';
                RSE(&(*Arv), noh->pai);
                noh = *Arv;
            }
        }else{
            aux = noh->pai->esquerda;

            if(aux->cor == 'V'){
                aux->cor = 'P';
                noh->pai->cor = 'V';
                RSD(&(*Arv), noh->pai);
                aux = noh->pai->esquerda;
            }

            if((aux->esquerda->cor == 'P') && (aux->direita->cor == 'P')){
                aux->cor = 'V';
                noh = noh->pai;
            }else if(aux->esquerda->cor == 'P'){
                aux->direita->cor = 'P';
                aux->cor = 'V';
                RSE(&(*Arv), aux);
                aux = noh->pai->esquerda;
                aux->cor = noh->pai->cor;
                noh->pai->cor = 'P';
                aux->esquerda->cor = 'P';
                RSD(&(*Arv), noh->pai);
                noh = *Arv;
            }
        }
    }
    noh->cor = 'P';
}

// Máximo da Árvore ///////////////////////////////////////////////////////////////////////////////////////
NO* maximo(NO *noh){
    while(noh->direita != vazio){
        noh = noh->direita;
    }
    return noh;
}


// Mínimo da Árvore ///////////////////////////////////////////////////////////////////////////////////////
NO* minimo(NO *noh){
    while(noh->esquerda != vazio){
        noh = noh->esquerda;
    }
    return noh;
}


// Sucessor do NÓ para o balaceamento /////////////////////////////////////////////////////////////////////
NO* NO_sucessor(NO *noh){
    NO *aux;

    if(noh->direita != vazio){
        return minimo(noh->direita);
    }

    aux = noh->pai;

    while((aux != vazio) && (noh == aux->direita)){
        noh = aux;
        aux = aux->pai;
    }
	return aux;
}


// Cria o NÓ para a Árvore ////////////////////////////////////////////////////////////////////////////////
NO* criar_NO(){
    NO *novo;
    novo = (NO*)malloc(sizeof(NO));
    printf("Informe um nÚmero para a Árvore...: ");
    scanf("%d", &novo->numero_arvore);
    
    if(novo->numero_arvore < 0){
        printf("!!!Número Invalido!!! Tente Novamente !!!\n");
        return criar_NO();
    }
    novo->cor = 'V'; // todo novo NÓ é vermelho
    novo->pai = vazio;
    novo->direita = vazio;
    novo->esquerda = vazio;

    return novo;
}


// Listagem dos dados /////////////////////////////////////////////////////////////////////////////////////
void imprimir(NO* Arv){
	printf("<");
    if (Arv != vazio){
    	if(Arv->cor == 'V'){
    		//system("color 04");
    		printf("%dV",Arv->numero_arvore);	
		}else{
			//system("color 08");
			printf("%dP",Arv->numero_arvore);	
		}
		imprimir(Arv->esquerda);
        imprimir(Arv->direita);
    }
    printf(">");
}



// Menu do Usuario //////////////////////////////////////////////////////////////////////////////////////// 
int menu(void){
    int op;
	printf("\n[MENU ARVORE RUBRO-NEGRA]\n");
    printf("1.Inserir NO.............:\n");
    printf("2.Imprimir Arvore.........:\n");
    printf("3.Remover NO da Arvore....:\n");
    printf("0.Sair do programa........:\n\n");
    printf("-OPCAO: "); scanf("%d", &op);
    return op;
}


// Localizar o NÓ da Árvore ///////////////////////////////////////////////////////////////////////////////
NO* localizar_NO(NO* ptr, int numero_arvore){
    if ((ptr == vazio) || (ptr->numero_arvore == numero_arvore)){
        return ptr;
    }

    if (numero_arvore < ptr->numero_arvore)
    {
        return localizar_NO(ptr->esquerda, numero_arvore);
    }
    else
    {
        return localizar_NO(ptr->direita, numero_arvore);
    }
}

// Remove um NÓ da Árvore /////////////////////////////////////////////////////////////////////////////////
NO *remover(NO **Arv, NO *ptr){
    NO *y, *x;

    if((ptr->esquerda == vazio) || (ptr->direita == vazio)){
        y = ptr;
    }else y = NO_sucessor(ptr);


    if(y->esquerda != vazio){
        x = y->esquerda;
    }else x = y->direita;

    x->pai = y->pai;

	if(y->pai == vazio){
        *Arv = x;
    }else if(y == y->pai->esquerda){
        y->pai->esquerda = x;
    }else y->pai->direita = x;


    if(y != ptr){
        ptr->numero_arvore = y->numero_arvore;//Aqui os dados são transferidos
    }

    if(y->cor == 'P'){
        if((*Arv)->direita == vazio && (*Arv)->esquerda->direita != vazio){
            RSE(&(*Arv), (*Arv)->esquerda);
            removerCor(&(*Arv), (*Arv)->esquerda);
            RSD(&(*Arv), (*Arv));
        }else{
            if((*Arv)->esquerda == vazio && (*Arv)->direita->esquerda != vazio){
                RSD(&(*Arv), (*Arv)->direita);
                removerCor(&(*Arv), (*Arv)->direita);
                RSE(&(*Arv), (*Arv));
            }
        }
        removerCor(&(*Arv), x);
    }
    return y;
    free(y);
    free(x);
}

// Menu de nterações com o usuário //////////////////////////////////////////////////////////////////////////
void MENU_RN(){
	NO *Arv = NULL; //Ponteiro Principal(Árvore).
	NO *aux;
    
    vazio = (NO*)malloc(sizeof(NO));
    vazio->pai = Arv;
    vazio->direita = vazio->esquerda = vazio;
    
	int valor, altura, opc;
	do{
        opc = menu();
        switch (opc){

        case 1:
            inserir(&Arv, criar_NO());
            break;
        case 2:
            printf("Listagem dos ptrs da Arvore \n\n");
            if(Arv != NULL){
                imprimir(Arv);
            }else printf("Sem Elementos na Arvore \n");
            break;
        case 3:
            if(Arv != NULL){
                printf("Digite o numero para ser removido da Arvore: "); scanf("%d", &valor);
                aux = localizar_NO(Arv, valor);
                if (aux->numero_arvore == valor){
                    imprimir(aux);
                    remover(&Arv, aux);
                }else printf("\nO numero %d nao encontrada!\n\n", valor);
                
                if(Arv == vazio){
                    vazio = (NO*)malloc(sizeof(NO));
                    vazio->pai = Arv = NULL;
                    vazio->direita = vazio->esquerda = vazio;
                }
            }else printf("Sem Elementos na Arvore \n");
            break;
		case 0:
			break;	    
        default:
      		printf("Comando INVALIDO!!\n");
      		break;    
        }
    }
    while (opc != 0);
}

int menu_escolha_de_arvore(){
	int op;
	
	printf("\n[MENU DE ESCOLHA DE ARVORE]\n");
    printf("1.Arvore Binaria.............:\n");
    printf("2.Arvore Rubro Negra.........:\n");
    printf("3.Arvore B...................:\n");
    printf("0.Sair do programa............:\n\n");
    printf("-OPCAO: "); scanf("%d", &op);
    return op;
}
int main(void){
	int x;
	
	do{
		x=menu_escolha_de_arvore();
		switch(x){
			case 1:
				MENU_BINARIA();
				break;
			case 2:
				MENU_RN();
				break;
			case 3:
				MENU_B();
				break;	
			case 0:
				break;		
			default:
				printf("Comando invalido... tente novamente.");
				break;				
		}
	}while(x!=0);
    return 0;
}
