# Checar se ply esta' instalado, senao execute: 
# sudo apt install ply
#
# Uso: python3 parser.py <arquivo>
#

#blibotecas necessarias
import comandos as comandos #import da lista ligada
import ply.lex as lex
import ply.yacc as yacc
import collections
import re
import sys

#valores estaticos no programa
#funcionam como um #define em C
RESET  = "\033[0m"
RED    = "\033[31m"
YELLOW = "\033[33m"

#booleana para verificar erro de sintaxe
sintError = False

#inicializando a lista ligada
listaLigada = comandos.listaLigada() #lista vazia criada com utlimo elemento none


tokens = (
'NEWLINE',
'COMMENT',
'SECBEGIN',
'SECEND',
'EQUAL',
'TEXT'
)

def t_COMMENT(t):
   r'^;(.)*\n' 
   pass

def t_SECBEGIN(t): 
   r'(?m)^\['
   return t;

def t_SECEND(t):
   r'\]'
   return t

def t_NEWLINE(t):
   r'\n'
   t.lexer.lineno += t.value.count("\n")
   return t

def t_TEXT(t):
   r'[^\[\]=\n\x0D]+'
   return t

def t_EQUAL(t):
   r'='
   return t

def t_error(t):
    print("Caractere ilegal: {} (linha {})".format(t.value[0], t.lexer.lineno))
    t.lexer.skip(1)

# caracteres ignorados
t_ignore = "\x0D"  


# Analisador lexico
lexer = lex.lex()

# Simbolo nao terminal inicial
start = 'sections'

# Producoes
def p_sections(t):
    '''sections : section
                |  sections section'''

def p_section(t):
     'section : sectionid keywords'

def p_sectionid(t):
     'sectionid : SECBEGIN TEXT SECEND NEWLINE'
 
def p_keywords(t):
    '''keywords : keyword
                | keywords keyword'''

def p_keyword(t):
     '''keyword : NEWLINE
                | TEXT EQUAL NEWLINE
                | TEXT EQUAL TEXT NEWLINE'''

# Erro de sintaxe
def p_error(t):
    global sintError
    print("Erro de sintaxe na linha {}".format(t.lexer.lineno))
    sintError = True


# Analisador sintatico
parser = yacc.yacc()


if __name__ == "__main__":
   if len(sys.argv) != 2:
     print("uso: python3 parser.py arquivo");
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




