'''
Ex5 lab 3
Programa que cria uma lista ligada conforme instrucoes do exercicio

Para a criacao dessa estrutura em Python, sera' necessaria a criacao de uma classe para um no' e otura para a lista, pois a linguagem e' orientada a objetos e fortemente tipada entao, por si so', ja trabalha com ponteiros. Sendo assim, para criar a estrutra, forcaremos a criacao de classes.

As estruturas aqui representadas foram baseadas em codigos presentes em https://algoritmosempython.com.br/cursos/algoritmos-python/estruturas-dados/listas-encadeadas/

'''

'''
Criacao de um no' na lista ligada.
Cada no' da lista possui um campo 'dado' e um campo 'proximo'. O campo 'proximo' é um apontador para o próximo no' da lista.
'''
class NoLista:
    def __init__(self, dado=0, proximo_no = None):
        self.dado = dado
        self.proximo = proximo_no

    def __repr__(self):
        return '%s -> %s' % (self.dado, self.proximo)


'''
Criacao da lista ligada.
Contém apenas um apontador para o primeiro elemento da lista, pois o ultimo elemento da lista possui um ponteiro para 'None'.
'''
class ListaLigada:
    def __init__(self):
        self.cabeca = None

    def __repr__(self):
        return "[" + str(self.cabeca) + "]"
     
############################################
#Criadas as classes, cria-se entao as      #
#funcoes que as manipulam                  #
############################################   
'''
Insercao na lista no inicio.
Inputs:
	lista -> lista ligada criada
	novo_dado -> novo dado a ser adcionado na lista
'''
def insere_no_inicio(lista, novo_dado):
    #cria um no' novo
    novo_no = NoLista(novo_dado)

    #no' novo vira a cabeca da lista
    novo_no.proximo = lista.cabeca

    #cabeca aponta para o no' novo
    lista.cabeca = novo_no



####executar isso aqui depois pra ver se tá rolando certo
'''
lista = ListaLigada()
print("Lista vazia:", lista)

insere_no_inicio(lista, 5)
print("Lista contém um único elemento:", lista)

insere_no_inicio(lista, 10)
print("Inserindo um novo elemento:", lista)

Lista vazia: [None]
Lista contém um único elemento: [5 -> None]
Inserindo um novo elemento: [10 -> 5 -> None]'''

