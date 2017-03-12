import sys
from math import *
import matplotlib.pyplot as plt

class RuleInf:
    def __init__(self,_id,_parent,_rule):
        self.id = _id
        self.parent = _parent
        self.rule = _rule        
        symb = self.rule.split(" ")        
        self.len = 0
        for s in symb:
            if len(s.strip()) > 0:
                self.len += 1
                
rules_inf = []
def read_grammar():
    print("Lendo informacoes da gramatica...")
    id = 0
    with open("aGramatica.txt","r") as G:
        for line in G:
            line = line.split("::=")
            parent = line[0].strip()           
            rules = str.split(line[1]," | ")
            for r in rules:            
                rules_inf.append(RuleInf(id,parent,r.strip()))
                id += 1

automata = {}
def read_LALR():
    print("Lendo tabela LALR...")
    with open("LALRstates.txt") as LALR:
        stt = ""
        for line in LALR:
            line = line.strip()
            if len(line) == 0 or line.find("::=") != -1: continue            
            if line.find("State") != -1:
                stt = line.split(" ")[1].strip()            
                automata[stt] = {}
            else: 
                transition = line.split(" ")
                for i in range(0,len(transition)):
                    transition[i] = transition[i].strip()
                if transition[0].find("'") != -1:
                    transition[0] = transition[0].split("'")[1]
                automata[stt][transition[0]] = transition            

strip = []                
def read_strip():
    print("Lendo a fita de tokens...")
    lline = "-1"
    with open("fita.txt") as stripfile:
        for line in stripfile:
            line = line.strip().split(" ")
            lline = line[2].strip()
            strip.append((line[0].strip(),line[1].strip(),lline))    
    strip.append(("$","$",str(int(lline)+1)))
    strip.append(("(EOF)","$",str(int(lline)+1)))
        
stk = []
vars_using = []
vars_value = []
def analysis():
    print("Analise sintatica...")
    tk = 0
    stk.append("0")
    while True: 
        if tk == len(strip):
            print("Esperavamos o fim de uma expressao apos o token " + strip[-1] + " na linha: " + strip[tk][2])
            return False
        if strip[tk][0] not in automata[stk[-1]]:
            print("O token: " + strip[tk][0] + " nao era esperado na linha: " + strip[tk][2])
            return False
        if len(stk) == 0:
            print("Uma expressao e necessaria antes do token: " + strip[tk][0] + " na linha: " + strip[tk][2])
            return False
        k = automata[stk[-1]][strip[tk][0]]        
        if k[1] == 'a': return True        
        elif k[1] == 's':
            stk.append(strip[tk][0])
            stk.append(k[2])        
            if strip[tk][0] == "variavel":
                vars_value.append((strip[tk][1],strip[tk][2]));    
            tk += 1
        elif k[1] == 'r':    
            variables = []
            rule = int(k[2])
            reduct = rules_inf[rule].len*2                  
            for i in range(0,reduct):
                top = stk.pop()
                if top == "variavel":
                    top2 = vars_value.pop()
                    vars_using.append((rules_inf[rule].parent,top2[0],top2[1]))
            stt = stk[-1]
            stk.append(rules_inf[rule].parent)            
            stk.append(automata[stt][rules_inf[rule].parent][2])    
    return False

decl_vars = {}                                           
def main():    
    read_LALR()
    read_strip()
    read_grammar()
    if analysis():
        ac = True
        print("Analise semantica...")
        print("")
        for vu in vars_using:            
            if vu[0] == "<vardec>":
                if vu[1] in decl_vars:
                    ac = False
                    print("A variavel '" + vu[1] + "' foi declarada na linha: " + decl_vars[vu[1]])                
                    print("Mas esta sendo declarada novamente na linha: " + vu[2])
                else:
                    decl_vars[vu[1]] = vu[2]
            elif vu[1] not in decl_vars:
                ac = False
                print("Variavel '" + vu[1] + "' nao foi declarada. Mas esta sendo utilizada na linha: " + vu[2])
        print("Codigo aceito." if ac else "Codigo nao aceito.")
    else: print("\nO codigo nao foi aceito.")
        
if __name__ == "__main__":
	main()