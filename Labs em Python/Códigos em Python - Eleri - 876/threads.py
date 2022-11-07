# Ilustra o uso de threads, mutexes e variaveis de condicao
# EA879/2020

import threading   # suport a threads
import time

# define um buffer global para comunicacao inter-threads
requests = []

# mutex (locker) e variavel de condicao para proteger o buffer
# Python combina mutex e variavel de condicao na mesma estrutura Condition
condvar = threading.Condition()

# define uma thread que ira receber requisicoes
def threadMestre():
    while True:
        # recebe uma requisicao
        req = input("Digite uma requisicao: ")
        # armazena a requisicao
        # REGIAO CRITICA: INICIO
        condvar.acquire()
        requests.append(req)
        # sinalize uma thread trabalhadora
        condvar.notify()
        condvar.release()
        # REGIAO CRITICA: FIM
    
# define thread trabalhadora
def threadTrab(id):
    ident = id
    while True:
        # acessa uma requisicao
        # REGIAO CRITICA: INICIO
        condvar.acquire()       
        if len(requests) == 0:
            condvar.wait()      # buffer vazio: bloqueia
        # lista nao vazia
        req = requests[0]
        requests.pop(0)         # remove primeiro elemento
        condvar.release()       # nao esqueca de liberar o mutex
        # REGIAO CRITICA: FIM
        # trabalha na requisicao
        time.sleep(3)
        print("Processamento da requisicao {} concluida pela thread {}"\
              .format(req, ident));

# programa principal
if __name__ == "__main__":
    # cria uma thread mestre
    thm = threading.Thread(target=threadMestre, args=())
    thm.start()
    # cria 3 thread trabalhadoras
    tht1 = threading.Thread(target=threadTrab, args=('T1',))
    tht1.start()
    tht2 = threading.Thread(target=threadTrab, args=('T2',))
    tht2.start()
    tht3 = threading.Thread(target=threadTrab, args=('T3',))
    tht3.start()

    # aguarda thread mestre terminar para sair
    thm.join()
