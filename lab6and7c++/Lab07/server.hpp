/*
EA878L - Tarefa 07
Servidor HTTP Mono--threaded (Biblioteca)
Matheus Souza
RA 071855 -- EC07
*/

//Bibliotecas de Apoio
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>

//Caracteres especiais
#define SP ' ' //Espaco
#define CR '\r' //Retorno
#define LF '\n' //Nova linha
#define PORT 5665

using namespace std;

//Variavel global que conterá o conteúdo do arquivo requisitado
ostringstream cont;

//Estruturas do Parser HTTP

/* parametro http */
struct http_parametro {
    http_parametro();            /* construtor */ 
    ~http_parametro();           /* destrutor */
    char *nome;                  /* nome do parametro */
    int nvalores;                /* numero de valores associados */
    char **valores;              /* array de valores */
    http_parametro *proximo;     /* ponteiro para o proximo parametro */
    http_parametro *anterior;    /* ponteiro para parametro anterior */
};


/* cabecalho http */
struct http_cabecalho {
    http_cabecalho();            /* construtor */ 
    ~http_cabecalho();           /* destrutor */ 
    char *operacao;              /* operacao: GET, POST, ... */
    char *recurso;               /* recurso (arquivo requisitado) */
    char *cgi_pars;              /* parametros cgi */
    char *versao;                /* versao do protocolo HTTP */
    http_parametro *primeiro;    /* ponteiro para o primeiro parametro */
    http_parametro *ultimo;      /* ponteiro para o ultimo parametro */
};


/* linha do cabecalho */
struct linha_cab
{
    linha_cab();
    ~linha_cab();
    
    char *linha;                 /* linha */
    int tamanho;                 /* tamanho da linha (sem \0) */
};

/* construtores e destrutores - Parser HTTP */
http_parametro::http_parametro()
{
    nome = NULL;
    valores = NULL;
    proximo = NULL;
    anterior = NULL;
    nvalores = 0;
}

http_parametro::~http_parametro()
{
  for(int i = 0; i < nvalores; i++) 
    {
      free(valores[i]);
    }
  free(valores);
  free(nome);
}

http_cabecalho::http_cabecalho()
{
  operacao = NULL;
  recurso = NULL; 
  versao = NULL;  
  primeiro = NULL;
  ultimo = NULL;    
}

http_cabecalho::~http_cabecalho()
{
    if (operacao != NULL) free(operacao);
    if (recurso != NULL) free(recurso);
    if (versao != NULL) free(versao);
    
    http_parametro *http_next, *http_current = primeiro;
    while(http_current) 
      {
        http_next = http_current->proximo;
        delete http_current;
        http_current = http_next;
      }
}

linha_cab::linha_cab()
{
  linha = NULL;
  tamanho = 0;
}

linha_cab::~linha_cab()
{
  if (linha != NULL)
    {
      free(linha);
    }
}


