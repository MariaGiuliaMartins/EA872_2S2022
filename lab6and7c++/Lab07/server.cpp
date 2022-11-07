/*
EA878L - Tarefa 07
Servidor HTTP Mono--threaded (Programa Principal)
Matheus Souza
RA 071855 -- EC07
*/

//Biblioteca principal
#include "server.hpp"

/*-------------- Processamento de Resposta --------------*/

string code_tab(int code){
//Funcao que retorna o descritor do codigo HTTP
//A tabela foi implementada completamente para
//possíveis ampliacoes futuras

  switch(code){
    //Codigos 1xx: Informativos
    case 100: return  string("Continue"); 			break;
    case 101: return  string("Switching Protocols"); 		break;
    
    //Codigos 2xx: Sucesso
    case 200: return  string("OK"); 				break;
    case 201: return  string("Created"); 			break;
    case 202: return  string("Accepted"); 			break;
    case 203: return  string("Non-Authoritative Information"); 	break;
    case 204: return  string("No Content"); 			break;
    case 205: return  string("Reset Content"); 			break;
    case 206: return  string("Partial Content"); 		break;

    //Codigos 3xx: Redirecionamento
    case 300: return  string("Multiple Choices"); 		break;
    case 301: return  string("Moved Permanently"); 		break;
    case 302: return  string("Found"); 				break;
    case 303: return  string("See Other"); 			break;
    case 304: return  string("Not Modified"); 			break;
    case 305: return  string("Use Proxy"); 			break;
    case 307: return  string("Temporary Redirect"); 		break;
    
    //Codigos 4xx: Erros do Cliente
    case 400: return  string("Bad Request"); 			break;
    case 401: return  string("Unauthorized"); 			break;
    case 402: return  string("Payment Required"); 		break;
    case 403: return  string("Forbidden"); 			break;
    case 404: return  string("Not Found"); 			break;
    case 405: return  string("Method Not Allowed"); 		break;
    case 406: return  string("Not Acceptable"); 		break;
    case 407: return  string("Proxy Authentication Required"); 	break;
    case 408: return  string("Request Timeout"); 		break;
    case 409: return  string("Conflict"); 			break;
    case 410: return  string("Gone"); 				break;
    case 411: return  string("Lenght Required"); 		break;
    case 412: return  string("Precondition Failed"); 		break;
    case 413: return  string("Request Entity Too Large"); 	break;
    case 414: return  string("Request URI Too Long"); 		break;
    case 415: return  string("Unsupported Media Type"); 	break;
    case 416: return  string("Requested Range Not Satisfiable");break;
    case 417: return  string("Expectation Failed"); 		break;

    //Codigos 5xx: Erros do Servidor
    case 500: return  string("Internal Server Error"); 		break;
    case 501: return  string("Not Implemented"); 		break;
    case 502: return  string("Bad Gateway"); 			break;
    case 503: return  string("Service Unavailable"); 		break;
    case 504: return  string("Gateway Timeout"); 		break;
    case 505: return  string("HTTP Version Not Supported"); 	break;

    default:  return  string("Unknown Parameter Value");	break;
  }

}

string resp(int code, string tipo_rec, time_t last_mod, int size){
//Funcao que monta o cabecalho da resposta da requisicao,
//retornando-o em uma string C++
	ostringstream header; 		//Cabecalho
	time_t tempo = time(NULL); 	//Tempo inicializado
	struct tm *tabtempo;		//Estrutura padrao que armazena o tempo
					//em diversas porcoes
	char data[80];			//string que contera a data

//Primeira Linha: Status Line
//Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	header << "HTTP/1.1" << SP << code << SP << code_tab(code) << CR << LF;


//Proxima linha: Data e horario da resposta
//Ex: Date: Tue, 15 Nov 1994 08:12:31 GMT

	tabtempo = gmtime(&tempo);	//Hora padrao usada como GMT
	strftime(data,80, "%a, %d %b %Y %H:%M:%S GMT" , tabtempo);			
					//Monta a string com a data/hora
	header << "Date:" << SP << data << CR << LF;

//Proxima linha: Server
//Ex: Server: CERN/3.0 libwww/2.17

	header << "Server:" << SP << "MYSERVER/5.0" << CR << LF;

//Proxima linha: Data e Horario da ultima modificacao do arquivo
//Ex: Last-Modified: Tue, 15 Nov 1994 08:12:31 GMT
	if (code == 200) { //Apenas impresso quando o arquivo e encontrado
		tabtempo = gmtime(&last_mod);
		strftime(data,80, "%a, %d %b %Y %H:%M:%S GMT" , tabtempo);			
					//Monta a string com a data/hora
		header << "Last-Modified:" << SP << data << CR << LF;
	}

//Proxima linha: tamanho do conteudo
	header << "Content-Length:" << SP << size << CR << LF;

//Proxima linha: tipo de conteudo
	header << "Content-Type:" << SP << tipo_rec << CR << LF << CR << LF << CR << LF;

	return header.str();
}


/*-------------- Gerenciamento de Arquivos --------------*/
//Funcao que acessa os arquivos e imprime seu conteudo
int fileman(char *dir, char *res){
	struct dirent **namelist;
	char *arqEntrada = "index.html", *s, *r;
	string aux;
	char buf[200];
	int m, fd,size,i,nfiles;
	struct stat statinfo; //Estrutura que armazena a informacao
	//Combinando ambas as strings
	m = strlen(dir) + strlen(res);
	s =  new char [m+2];
	s[0] = '\0'; //Anulando a string
	strcat(s,dir);
	strcat(s,res);
	
	//Acesso do arquivo com a funcao stat
	if(stat(s, &statinfo) == -1) {
		delete s;
		return 404;
	}
	//Se a leitura for negada --> Forbidden (403)
	if(access(s, R_OK) != 0){
		delete s;
		return 403;
	}
	//Se for um arquivo --> Ler e imprimir na tela (200)
	if((statinfo.st_mode & S_IFMT) == S_IFREG) {
		if((fd = open(s, O_RDONLY, 0600)) == -1){
			printf("Problemas para abrir o arquivo\n");
			delete s;			
			return 0;
		}
		while((size = read(fd, buf, sizeof(buf))) > 0){
			aux.append(buf,size);
			cont << aux;
			aux = "";
		}
		delete s;		
		return 200;	
	}
	
	//Se for um diretorio

	if((statinfo.st_mode & S_IFMT) == S_IFDIR) {
		//Nao pode ser executado --> Forbidden (403)
		if(access(s, X_OK) != 0){
			delete s;
			return 403;
		}
		//Adicionando index.html a string
		r =(char*)malloc(sizeof(char)*(m+15));
		strcpy(r,s);
		strcat(r,"/");
		strcat(r,arqEntrada);
		//Se o arquivo nao existir
		if(stat(r, &statinfo) == -1) {
		
			nfiles = scandir(s, &namelist, NULL, alphasort);
  			if(nfiles < 0) {
    				printf("Impossivel listar diretorio\n");
				delete s;
    				exit(0);
  			}
			//Lista os arquivos do diretorio, se existirem
			if(nfiles > 0) {
				cont << "Conteudo de " << s << ":\n";
  				for(i = 0; i < nfiles; i++) {
    					cont << namelist[i]->d_name << '\n';
 					free(namelist[i]);
  				}
			delete s;			
			return 200;			
			}
		delete s;
		return 404;
		}
		//Se o arquivo existe
		if((fd = open(r, O_RDONLY, 0600)) == -1){
			printf("Problemas para abrir o arquivo\n");
			delete s;
			return 0;
		}
		while((size = read(fd, buf, sizeof(buf))) > 0){	
			aux.append(buf,size);
			cont << aux;
			aux = "";
		}		
		delete s;
		return 200;
	}


	delete s;
	return 0;


}

/*------------- Processamento de Requisicao -------------*/

/* funcao que le uma linha do cabecalho */
/* separa linha por LF ou CR-LF */
/* retorna linha vazia se encontrar duas linhas em branco (fim do cabecalho) */
/* ou NULL em caso de erro */

linha_cab* read_linha_cab(int file) {
    char buff[256];    /* buffer auxiliar */
    linha_cab *hl;     /* retorno */
    int i = 0;
    int err;

    do {
        err = read(file, &buff[i], 1);
        if(err == -1) return(NULL);  /* erro na chamada read */
        if(err == 1)   /* lido 1 byte */
        {
            if(buff[i] == 13) continue;    /* elimina CR */
            if(i == 0 && buff[i] == 10)    /* linha em branco -- fim de cabecalho */
            {
                hl = new linha_cab;
                hl->tamanho = 0;
                hl->linha = (char *)malloc(1);
                *(hl->linha) = '\0';
                return(hl);
            }

            if(buff[i] != 10)  /* caractere que nao LF */
            {
                i++;
                continue;
            }
            else   /* fim da linha */
            {
                hl = new linha_cab;
                hl->tamanho = i;
                hl->linha = (char *)malloc(i+1);
                strncpy(hl->linha, buff, i);
                *(hl->linha + i) = '\0';
                return(hl);
            }
        }
        /* err == 0: cliente fechou a conexao */
        printf("Connection closed by peer.\n");
        fflush(stdout);
        return(NULL);
    } while(i < 256);
    printf("Linha muito longa.\n");
    return(NULL);
}

/* funcao que processa a primeira linha do cabecalho */
/* preenche uma estrutura http_cabecalho passada como parametro */
/* retorna 0 (OK) ou -1 (erro) */

int primeira_linha(http_cabecalho *h, char *linha) {
  char *op, *res, *ver, *s;
    int i;
    /* obtem operacao */
    while(*linha && *linha == ' ') linha++; /* remove espaco inicial caso exista  */
    if(! *linha) return -1;
    op = linha;
    i = 0;
    while(*linha && *linha != ' ') {linha++; i++;}  /* caminha ate prox. espaco */
    if(i == 0) return(-1);
    h->operacao = (char *)malloc(i + 1);
    strncpy(h->operacao, op, i);
    *(h->operacao + i) = '\0';
    if(strcmp(h->operacao, "GET") != 0 &&
            strcmp(h->operacao, "POST") != 0 &&
            strcmp(h->operacao, "OPTIONS") != 0 &&
            strcmp(h->operacao, "TRACE") != 0) return(-1);

    /* obtem o recurso */
    while(*linha && *linha == ' ') linha++;   /* remove espaco inicial */
    res = linha;
    i = 0;
    while(* linha && *linha != ' ') {linha++; i++;}  /* caminha ate prox. espaco */
    if(i == 0) return(-1);
    h->recurso = (char *)malloc(i + 1);
    strncpy(h->recurso, res, i);
    *(h->recurso + i) = '\0';
    s = h->recurso;
    while(*s && *s != '?') s++;
    if(*s) {
      *s = '\0';
      h->cgi_pars = ++s;
    }
    else h->cgi_pars = NULL;

    /* obtem versao do protocolo */
    while(*linha && *linha == ' ') linha++;   /* remove espaco inicial */
    ver = linha;
    i = 0;
    while(*linha && *linha != ' ') {linha++; i++;}  /* caminha ate prox. espaco */
    if(i == 0) return(-1);
    h->versao = (char *)malloc(i + 1);
    strncpy(h->versao, ver, i);
    *(h->versao + i) = '\0';

    return(0);
}



/* funcao que processa parametros do cabecalho  */
/* adiciona parametros a uma estrutura http_cabecalho */ 
/* retorna 0 (OK) ou -1 (erro) */

int http_par(http_cabecalho *hddr, char *linha) {
    http_parametro *par;
    int i, k;
    char *nome, *str;
    while(*linha && *linha == ' ') linha++; /* remove espaco inicial caso exista  */
    nome = linha;
    i = 0;
    while(*linha && *linha != ':') {linha++; i++;}  /* posiciona em : */
    if(i == 0) return(-1);
    /* aloca celula da lista ligada */
    par = (http_parametro *)malloc(sizeof(http_parametro));
    par->proximo = NULL;
    par->anterior = NULL;
    par->nvalores = 0;
    par->nome = (char *)malloc(i + 1);
    strncpy(par->nome, nome, i);
    *(par->nome + i) = '\0';

    /* conta numero de valores associado ao para¿œmetro */
    linha++;
    str = linha;
    while(*str) {if(*str == ',') par->nvalores++; str++;}
    par->nvalores++;

    /* aloca ponteiro para valores */
    par->valores = (char **)malloc(par->nvalores * sizeof(char*));

    /* busca e armazena  valores */
    for(k = 0; k < par->nvalores; k++) {
        while(*linha && *linha == ' ') linha++;   /* remove espaco, caso exista  */
        str = linha;
        i = 0;
        while(*linha && *linha != ',') {linha++; i++;}  /* posiciona em , */
        if(*linha) linha++;
        par->valores[k] = (char *)malloc(i + 1);
        strncpy(par->valores[k], str, i);
        *(par->valores[k] + i) = '\0';

    }

    /* liga parametro na lista de parametros */
    if(hddr->primeiro == NULL) {   /* lista vazia */
        hddr->primeiro = par;
        hddr->ultimo = par;
    }
    else {
        par->anterior = hddr->ultimo;
        hddr->ultimo->proximo = par;
        hddr->ultimo = par;
    }

    return(0);
}

void parser(int sock, http_cabecalho* hddr) {
//Funcao principal do parser, que monta o cabecalho
//para o programa principal do servidor.
  linha_cab *hl;        /* cabecalho http */
  int numlinha = 0;     /* linhas do cabecalho */


  /* le linha por linha ate fim de arquivo */
  numlinha = 0;  /* zere sempre que iniciar o parsing */
  while((hl = read_linha_cab(sock)) != NULL)
    {
      if(hl->tamanho == 0) {   /* fim do cabecalho */
	delete hl;   /* librea linha lida */
	break;
      }
      if(numlinha == 0) {   /* primeira linha */
	if(primeira_linha(hddr, hl->linha) == -1) {
	  printf("Erro de sintaxe.\n");
	  exit(0);
	}
      }
      else {   /* linhas subsequentes*/
	if(http_par(hddr, hl->linha) == -1) {
	  printf("Erro de sintaxe.\n");
	  exit(0);
	}
      }
      delete hl;   /* librea linha lida */
      numlinha++;
    }


}

void imprime_cabecalho(http_cabecalho *hddr, int file_out) {
    int i;
    http_parametro *par;
    write(file_out, hddr->operacao, strlen(hddr->operacao));
    write(file_out, " ", 1);
    write(file_out, hddr->recurso, strlen(hddr->recurso));
    if(hddr->cgi_pars) {
      write(file_out, "?", 1);
      write(file_out, hddr->cgi_pars, strlen(hddr->cgi_pars));
    }
    write(file_out, " ", 1);
    write(file_out, hddr->versao, strlen(hddr->versao));
    write(file_out, "\n", 1);
    par = hddr->primeiro;
    while(par) {
        write(file_out, par->nome, strlen(par->nome));
        write(file_out, ": ", 2);
        for(i = 0; i < par->nvalores; i++) {
            write(file_out, par->valores[i], strlen(par->valores[i]));
            if(par->nvalores > 1 && i < par->nvalores - 1) write(file_out, ", ", 2);
        }
        write(file_out, "\n", 1);
        par = par->proximo;
    }
}


/*----------------- Tipo de Recurso ------------------*/
bool testType(string recurso, string type) {
  int rlen = recurso.length();
  int tlen = type.length();
  size_t pos = recurso.rfind(type);
  // a ocorrencia deve casar com a ultima extensao do nome
  if(pos == string::npos || pos != (rlen - tlen)) return false;
  return true;
}

string getContentType(string recurso) {
  if(testType(recurso, string(".html"))) return (string("text/html"));
  if(testType(recurso, string(".htm"))) return (string("text/html"));
  if(testType(recurso, string(".xml"))) return (string("text/xml"));
  if(testType(recurso, string(".xhtml"))) return (string("application/xhtml+xml"));
  if(testType(recurso, string(".css"))) return (string("text/css"));
  if(testType(recurso, string(".gif"))) return (string("image/gif"));
  if(testType(recurso, string(".jpeg"))) return (string("image/jpeg"));
  if(testType(recurso, string(".jpg"))) return (string("image/jpeg"));
  if(testType(recurso, string(".png"))) return (string("image/png"));
  if(testType(recurso, string(".mpeg"))) return (string("video/mpeg"));
  if(testType(recurso, string(".mov"))) return (string("video/quicktime"));
  if(testType(recurso, string(".wav"))) return (string("audio/x-wav"));
  if(testType(recurso, string(".js"))) return (string("application/javascript"));
  if(testType(recurso, string(".pdf"))) return (string("application/pdf"));
  if(testType(recurso, string(".swf"))) return (string("application/x-shockwave-flash"));
  return (string("text/plain"));
}

/*----------------- Programa Principal ------------------*/

int main (int argc, char** argv){
	//Variaveis auxiliares para o programa principal
	char *local = "/home/matheus/Desktop/ea878/Lab07";
	http_cabecalho *hddr;
	int code = 0;
	string str, type, str2, res;
	string::iterator it;
	struct stat status;
	time_t last = time(NULL);
    	unsigned short port;       /* port server binds to                  */
    	char buf[1024];            /* buffer for sending and receiving data */
    	struct sockaddr_in client; /* client address information            */
    	struct sockaddr_in server; /* server address information            */
    	int s;                     /* socket for accepting connections      */
    	int ns;                    /* socket connected to client            */
    	int namelen;               /* length of client name         	*/
    	int msglen;                /* length of the received message        */
    	int i;
    	
	port = PORT; //Porta definida 5665

	/*
	* Get a socket for accepting connections.
	*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	perror("Socket()");
	exit(2);
	}

	/*
	* Bind the socket to the server address.
	*/
	server.sin_family = AF_INET;
	server.sin_port   = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
	perror("Bind()");
	exit(3);
	}

	/*
	* Listen for connections. Specify the backlog as 5.
	*/
	if (listen(s, 5) != 0)
	{
	perror("Listen()");
	exit(4);
	}

	/*
	*  Loop forever
	*/
	printf("\n%s accepting connections from HTTP clients.\n", argv[0]);
	fflush(stdout);

	while (1) {

	/*
	* Accept a connection.
	*/
	namelen = sizeof(client);
	if ((ns = accept(s, (struct sockaddr *)&client, (socklen_t*) &namelen)) == -1)
	{
	perror("Accept()");
	break;
	}


	//Processamento da Requisicao
	hddr = new http_cabecalho();

	parser(ns, hddr); //Parsing da mensagem

  	imprime_cabecalho(hddr, 1);

	type = getContentType(string(hddr->recurso)); //Tipo de recurso solicitado

	code = fileman(local,hddr->recurso); //Acesso ao recurso

	if (code == 200) {
		res.append(string(local));
		res.append(string(hddr->recurso));
		stat(res.data(), &status);
		if((status.st_mode & S_IFMT) == S_IFDIR){ //Se for diretorio
			it = res.end();
			if(*it != '/') //Se nao tiver a barra no final, adiciona
				res.append(string("/"));
			res.append(string("index.htm")); //Verifica a existencia da index.htm			
			if(stat(res.data(), &status) != -1)
				type = "text/html";
			res.append(string("l")); //Ou index.html
			if(stat(res.data(), &status) != -1)
				type = "text/html";
		}
		str2 = cont.str();
		str = resp(code, type, status.st_mtime, str2.length());
	} else { //Pagina de erro
		cont << "<!DOCTYPE HTML PUBLIC" << SP <<'"' << "-//IETF//DTD HTML 2.0//EN" << '"' << ">" << CR << LF;
		cont << "<html><head>" << CR << LF;
		cont << "<title>" << code << SP << code_tab(code) <<  "</title>" << CR << LF;
		cont << "</head><body>" << CR << LF;
		cont << "<h1>"<< code << SP << code_tab(code) << "</h1>" << CR << LF;
		cont << "<p>Um erro ocorreu.</p>" << CR << LF;
		cont << "</body></html>"<< CR << LF << CR << LF;
		str2 = cont.str();
		str = resp(code,"text/html",last,str2.length());
	}
	cout << str;
	cout << "CONTEUDO ENVIADO" << '\n';
	cout << str2;
	//Enviando a resposta para o Cliente
	write (ns, str.data(), str.length());

	//Enviando o conteudo para o Cliente
	write (ns, str2.data(), str2.length());


	//Anula todas as variáveis usadas	
	res = "";
	cont.seekp(0);
	cont.str("");
	close(ns);
	delete hddr;
	cout << "\nRequest ended.\n\n";
	}  /* loop ends here */

	close(s); 
	printf("Server ended on error\n");
	exit(5);
	return 0;
}


