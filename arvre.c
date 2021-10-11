#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct Nodo *arvre;
arvre *raiz;

struct Nodo {
    int data;                   
    int altura;    
    struct Nodo *dir;
    struct Nodo *esq;    
};

typedef struct Nodo Nodo;

arvre *iniciarArvre();
void testesAVL();
int altura(Nodo *nodo);
int comparar(int x, int y);
Nodo *compararMenor(Nodo *nrTemp);
int calcFB(Nodo *nodo);
int AVL(struct Nodo *nodo);
void freeNodo(Nodo *nodo);
void freeArvre(arvre *raiz);
void RSD(arvre *raiz);
void RSE(arvre *raiz);
void RDD(arvre *raiz);
void RDE(arvre *raiz);
int inserir(arvre *raiz, int elemento);
void listar(Nodo *nodo, int nivel);
void listarArvre(arvre *raiz);
void passos(arvre *raiz, int elemento);

int main() {
    int nodos;
    int verificar;
    int i;
    int numeroRand;
    srand(time(0));

    printf("Informe a quantidade de nodos para inserir: ");
    scanf("%d", &nodos);

	arvre *raiz = iniciarArvre();
	
    for(i = 0; i < nodos; i++) {
		numeroRand = rand() %100;
		inserir(raiz, numeroRand);
    }
    
    listarArvre(raiz);
    verificar = AVL(*raiz);

    if(verificar == 1){
        printf("\n\nE AVL? Sim \n\n");
    }else{
        printf("\n\nE AVL? Não \n\n");
    }
    
    freeArvre(raiz);

    testesAVL();
    return 0;
}

void testesAVL(){
    arvre *raiz = iniciarArvre();
    passos(raiz, 2);
    passos(raiz, 16);
	passos(raiz, 4);
	passos(raiz, 0);
	passos(raiz, 1);
    passos(raiz, 8);
    passos(raiz, 7);
    passos(raiz, 17);
    passos(raiz, 18);
    freeArvre(raiz);
}

void passos(arvre *raiz, int elemento){
    int verificar;

    inserir(raiz, elemento);
	listarArvre(raiz);
    verificar = AVL(*raiz);

    if(verificar == 1){
        printf("\n\nE AVL? Sim \n\n");
    }else{
        printf("\n\nE AVL? Não \n\n");
    }
}

void listar(Nodo *nodo, int nivel) {
    if(nodo != NULL) {
        for (int i = 0; i < nivel; i++){
            printf("  ");
        }
           
		printf("%d\n", nodo->data);
        listar(nodo->esq, nivel + 1); 
        listar(nodo->dir, nivel + 1);
    }
}

void listarArvre(arvre *raiz) {
    if(raiz != NULL) {
		listar(*raiz, 0);
	}
}


arvre *iniciarArvre() {
    arvre *raiz = (arvre *)malloc(sizeof(arvre));

    if(raiz != NULL){
        *raiz = NULL;
    }
      
    return raiz;
}

int altura(Nodo *nodo) {

    if(nodo == NULL){
        return -1;
    }else{
        return nodo->altura;
    }   
       
}

int comparar(int x, int y) {

    if( x > y ){
        return x;
    }else{
        return y;
    }   

}

Nodo *compararMenor(Nodo *nrTemp) {
    Nodo *nodoX = nrTemp;
    Nodo *nodoY = nrTemp->esq;

    while(nodoY != NULL) {
        nodoX = nodoY;
        nodoY = nodoY->esq;
    }

    return nodoX;
}

int calcFB(Nodo *nodo) {

    if(raiz == NULL){
        return 0;
    }

    return (altura(nodo->esq) - altura(nodo->dir));
}

int AVL(struct Nodo *nodo) {
    int FB;
    
	if (nodo == NULL){
        return 1;
    }
		
	if (!AVL(nodo->esq)){
		return 0;
    }

	if (!AVL(nodo->dir)){
        return 0;
    }

	FB = calcFB(nodo);
	
	if (( FB > 1 ) || ( FB < -1 )){
        return 0;
    }else{
        return 1; 
    }
}

void freeNodo(Nodo *nodo) {

    if(nodo != NULL) {
        freeNodo(nodo->esq); 
        freeNodo(nodo->dir);
        free(nodo);
        
        nodo = NULL; 
    }
}

void freeArvre(arvre *raiz) {

    if(raiz != NULL) {
        freeNodo(*raiz);
        free(raiz);
    }    
}

void RSE(arvre *raiz) {
    Nodo *nodo;

    nodo = (*raiz)->dir; 
    (*raiz)->dir = nodo->esq; 
    nodo->esq = *raiz; 

    (*raiz)->altura = comparar(altura((*raiz)->esq), altura((*raiz)->dir)) + 1;
    nodo->altura = comparar(altura(nodo->dir), (*raiz)->altura) + 1;

    *raiz = nodo;
}

void RSD(arvre *raiz) { 
    Nodo *nodo;
    
    nodo = (*raiz)->esq; 
    (*raiz)->esq = nodo->dir; 
    nodo->dir = *raiz;
    
    (*raiz)->altura = comparar(altura((*raiz)->esq), altura((*raiz)->dir)) + 1;
    nodo->altura = comparar(altura(nodo->esq), (*raiz)->altura) + 1;

    *raiz = nodo;
}

void RDD(arvre *raiz) {

    RSE(&(*raiz)->esq);
    RSD(raiz);
}

void RDE(arvre *raiz) {

    RSD(&(*raiz)->dir);
    RSE(raiz);
}

int inserir(arvre *raiz, int elemento) {
    int aux; 

    if(*raiz == NULL) { 

        Nodo *tempNodo;
        tempNodo = (Nodo*) malloc(sizeof(Nodo));

        if(tempNodo == NULL){
            return 0;
        }

        tempNodo->data = elemento;
        tempNodo->altura = 0;
        tempNodo->esq = NULL;
        tempNodo->dir = NULL;
        *raiz = tempNodo;

        return 1;
    }
 
    Nodo *nrTemp = *raiz;

    if(elemento < nrTemp-> data) {
		
        if((aux = inserir(&(nrTemp->esq), elemento)) == 1) {

            if(calcFB(nrTemp) >= 2) {

                if(elemento < (*raiz)->esq->data){
                    RSD(raiz);
                }else{
                    RDD(raiz);
                }   
            }
        }
    } else {
        if(elemento > nrTemp->data) {
			
            if((aux = inserir(&(nrTemp->dir), elemento)) == 1) {
				
                if(calcFB(nrTemp) >= 2) {

                    if((*raiz)->dir->data < elemento){
                        RSE(raiz);
                    }else{
                        RSD(raiz);
                    }
                }
            }

        } else {
            return 0;
        }
    } 

    nrTemp->altura = comparar(altura(nrTemp->esq), altura(nrTemp->dir)) + 1;

    return aux;
}