#teste

import comandos as comandos

lista = comandos.ListaLigada()
print("Lista vazia:", lista)

comandos.insere_no_inicio(lista, 5)
print("Lista contém um único elemento:", lista)

comandos.insere_no_inicio(lista, 10)
print("Inserindo um novo elemento:", lista)

