/*EA878 - 2s2010
* ATIVIDADE 06
* Processador de Respostas HTTP
* Matheus Souza - RA071855
*/
#include <string>
#include <iostream>

using namespace std;

string code_tab(int code){
//Funcao que retorna o descritor do codigo
  switch(code){
    //Codigos 1xx: Informativos
    case 100: return  string("Continue"); break;
    case 101: return  string("Switching Protocols"); break;
    
    //Codigos 2xx: Sucesso
    case 200: return  string("OK"); break;
    case 201: return  string("Created"); break;
    case 202: return  string("Accepted"); break;
    case 203: return  string("Non-Authoritative Information"); break;
    case 204: return  string("No Content"); break;
    case 205: return  string("Reset Content"); break;
    case 206: return  string("Partial Content"); break;

    //Codigos 3xx: Redirecionamento
    case 300: return  string("Multiple Choices"); break;
    case 301: return  string("Moved Permanently"); break;
    case 302: return  string("Found"); break;
    case 303: return  string("See Other"); break;
    case 304: return  string("Not Modified"); break;
    case 305: return  string("Use Proxy"); break;
    case 307: return  string("Temporary Redirect"); break;
    
    //Codigos 4xx: Erros do Cliente
    case 400: return  string("Bad Request"); break;
    case 401: return  string("Unauthorized"); break;
    case 402: return  string("Payment Required"); break;
    case 403: return  string("Forbidden"); break;
    case 404: return  string("Not Found"); break;
    case 405: return  string("Method Not Allowed"); break;
    case 406: return  string("Not Acceptable"); break;
    case 407: return  string("Proxy Authentication Required"); break;
    case 408: return  string("Request Timeout"); break;
    case 409: return  string("Conflict"); break;
    case 410: return  string("Gone"); break;
    case 411: return  string("Lenght Required"); break;
    case 412: return  string("Precondition Failed"); break;
    case 413: return  string("Request Entity Too Large"); break;
    case 414: return  string("Request URI Too Long"); break;
    case 415: return  string("Unsupported Media Type"); break;
    case 416: return  string("Requested Range Not Satisfiable"); break;
    case 417: return  string("Expectation Failed"); break;

    //Codigos 5xx: Erros do Servidor
    case 500: return  string("Internal Server Error"); break;
    case 501: return  string("Not Implemented"); break;
    case 502: return  string("Bad Gateway"); break;
    case 503: return  string("Service Unavailable"); break;
    case 504: return  string("Gateway Timeout"); break;
    case 505: return  string("HTTP Version Not Supported"); break;

  }

}


string resp(int code, string tipo_rec, time_t last_mod, int size){
//Funcao que monta o cabecalho da resposta da requisicao,
//retornando-o em uma string C++

}

int main(int argc, char **argv) {
     int N = 0;
     cin >> N;
     cout << code_tab(N);

return 0;
}
