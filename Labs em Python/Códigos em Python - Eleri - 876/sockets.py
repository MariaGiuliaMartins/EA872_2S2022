# EA879/2020
# Veja https://wiki.python.org.br/SocketBasico

import socket
import os

HOST = ''              # '' significa endereco local
PORT = 5000       # porta que o servidor aguarda conexoes

# cria um socket TCP/IP tipo servidor
serverSoc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
orig = (HOST, PORT)
serverSoc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serverSoc.bind(orig)
serverSoc.listen(1)    # 1: tamanho maximo da fila de conexões pendentes
# aceita e processa requisicoes
while True:
    connection, cliente = serverSoc.accept()
    print('\n\nConectado por {}'.format(cliente))

    # processa a requisicao
    # a partir daqui esta tarefa seria executada por uma thread trabalhadora

    req = connection.recv(1024)     # le 1024 bytes do socket
    if not req:     # cliente fechou a conexao
        connection.close()
        continue
    req = req.decode('ascii')
    print(req)
    reqLines = req.split('\r\n')
    print('Primeira Linha: {}'.format(reqLines[0]))
    reqParts = reqLines[0].split()
    if reqParts[1] == '/':
        resource = './web/index.html'
    else:
        resource = './web' + reqParts[1]
    print('Recurso requisitado: {}'.format(resource))
    # evia o recurso ao cliente
    try:     # envia recurso
        file = open(resource, 'r')
        size = str(os.path.getsize(resource))
        # cabecalho de retorno
        cab = 'HTTP/1.1 200 OK\r\n'
        # content-type depende da extensao do recurso requisitado:
        # text/html, text/plain, image/jpeg, image/png, image/gif, etc.
        cab = cab + 'Content-Type: text/html\r\n'
        cab = cab + 'Content-Length: ' + size + '\r\n\r\n'
        connection.send(cab.encode('utf-8'))
        # conteudo de retorno
        connection.send(bytes(file.read(), 'utf-8'))
        file.close()
    except:     # recurso nao existe
        line = 'HTTP/1.1 404 Not Found\r\n\r\n'
        connection.send(line.encode('utf-8'))
    print('Finalizando conexao.')
    connection.close()
