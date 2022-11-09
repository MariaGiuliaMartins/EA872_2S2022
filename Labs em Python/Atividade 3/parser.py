#Atividade 3
#referencias:
##https://github.com/dabeaz/ply
##https://pemtajo.github.io/exemplo_ply/
##http://www.dabeaz.com/ply/ply.html
##https://ericknavarro.io/2020/02/10/24-Mi-primer-proyecto-utilizando-PLY/


#blibotecas e programas necessarios
import comandos as c #import da lista ligada
import sys                  #para pegar os comandos
import ply.yacc as yacc
import ply.lex as lex	    #arquivo que define os erros
import collections
import re

#Funcoes utilizadas no codigo
def concatenar(char1, char2, char3):
	'''
	Funcao que concatena 3 caracteres
	Inputs:
		char1, char2, char3 -> caracteres a serem concatenados
	Output:
		caracteres do Inputs concatenados
'''
	return char1 + char2 + char3


#booleana para verificar erro de sintaxe
sintError = False

#inicializando nos e lista ligada
listaLigadaComandos = c.listaLigada() #lista vazia criada com utlimo elemento none para os comandos
noComandos = c.NoLista(None) #novo no' para comandos
listaLigadaParametros = c.listaLigada() #lista vazia criada com utlimo elemento none para os parametros
noParametros = c.NoLista(None) #novo no' para parametros


tokens = (
'COMANDOS',
'COMANDOHTTP',
'PARAMETROS',
'DOISPONTOS',
'COMMA',
'SPACE',
'NEWLINE'
)

'''
tokens na ordem descrita acima:
comandos são as palavras quem vêm antes dos :
parametros são as palavras que vêm depois dos :
sinal de dois pontos :
sinal de vírgula ,
espaço 
qubra de lina \n
comantários #
'''

#regras
def t_COMANDOS(t):
	#r'[a-zA-Z_][a-zA-Z_0-9]*' #pode ser qualquer palavra ou número
	
def t_COMANDOHTTP(t):
	r'[a-zA-Z_][a-zA-Z_0-9]*' #pode ser qualquer palavra ou número

def t_PARAMETROS(t):
	r'[a-zA-Z_][a-zA-Z_0-9]*' #pode ser qualquer palavra ou número
   
def t_NEWLINE(t):
	r'\n+'
	t.lexer.lineno += t.value.count("\n")
	return t

def t_error(t):
    	print("Caractere ilegal: {} (linha {})".format(t.value[0], t.lexer.lineno))
    	t.lexer.skip(1)

def t_COMMENT(t):
   r'\#.*'
   pass


# Analisador lexico
lexer = lex.lex()


#################################################
#Producoes

# Simbolo nao terminal inicial para as produções
start = 'comandos'

def p_comandos(t):
     	'''comandos : COMANDOS DOISPONTOS'''
     	if listaLigadaComandos == None: #se a lista estiver vazia, adiciona-se os comandos novos
     		c.insere_no_inicio(listaLigadaComandos, sys.argv[1])
     	else:
     		c.insere_no_fim(listaLigadaComandos, noComandos, sys.argv[1])

def p_comandohttp(t):
     	'''comandohttp : COMANDOHTTP SPACE'''
     	if listaLigadaComandos == None: #se a lista estiver vazia, adiciona-se os comandos novos
     		c.insere_no_inicio(listaLigadaComandos, sys.argv[1])
     	else:
     		c.insere_no_fim(listaLigadaComandos, noComandos, sys.argv[1])
 
def p_parametros(t):
	'''parametros : PARAMETROS COMMA PARAMETROS
		      | PARAMETROS SPACE
		      | PARAMETROS NEWLINE'''
	#t[0] = concatenar(sys.argv[1], ",", sys.argv[3])
	#t[0] = concatenar(t[1], ",", t[3])

def p_error(t):
	global sintError
	print("Erro de sintaxe na linha {}".format(t.lexer.lineno))
	sintError = True
    		
# Analisador sintatico
parser = yacc.yacc()


if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("uso: python3 parser.py arquivo")
		exit()
	try:
		f = open(sys.argv[1], "r")
		s = f.read()
		# analise sintatica
		parser.parse(s)
		if sintError == False:
			print("Arquivo sintaticamente correto.")
		else:
			print("Arquivo com sintaxe incorreta.")
		f.close();
	except Exception:
		print("{}: arquivo nao encontrado.".format(sys.argv[1]));
		exit()
