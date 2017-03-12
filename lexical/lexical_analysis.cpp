#include "header.h"

struct Simbolo{
  char c;
  int linha,pos;
  Simbolo(int _linha,int _pos,char _c){
    linha = _linha,pos = _pos, c = _c;
  }
};

AF afd;
map<int,string> finais;
void carregarAFD(){
  ifstream qwer;
  qwer.open("qwer.csv",ifstream::in);
  string line;
  while(getline(qwer,line)){
    int i;
    string est,prod,p_est;
    for(i = 0; line[i] != ','; i++) est.pb(line[i]);
    for(i++; line[i] != ','; i++) prod.pb(line[i]);
    for(i++; i < sz(line); i++) p_est.pb(line[i]);
    int n_est = atoi(est.c_str());
    if(!p_est.compare("#")) finais[n_est] = prod;
    else afd[n_est][prod.c_str()[0]].pb(atoi(p_est.c_str()));
  }
  qwer.close();
}

vector<Simbolo> codigo;
void lerCodigo(char dir[]){
  ifstream qwer;
  qwer.open(dir,ifstream::in);
  int i,j;
  string line;
  for(i = 1; getline(qwer,line); i++){
      for(j = 0; j < sz(line); j++)
        codigo.pb(Simbolo(i,j+1,line[j]));
      codigo.pb(Simbolo(i,sz(line)+1,'\n'));
  }
  codigo.pb(Simbolo(i,j,' '));
  qwer.close();
}

string tokenToString(char c){
  char buff[3];
  buff[0] = c,buff[1] = '\0';
  return string(buff);
}

struct Token{
  int linha;
  string tipo,nome;
  Token(string _tipo,string _nome,int _linha){    
    tipo = _tipo,nome = _nome;
    linha = _linha;
  }
};

vector<Token> fita;
map<string,bool> sep;
bool analisar(){
  sep[" "] = sep["\t"] = sep["\n"] = sep["+"] = sep["-"] = sep["*"] = sep["/"] =
  sep["("] = sep[")"] = sep["{"] = sep["}"] = sep[";"] = sep["<"] =
  sep["="] = sep[">"] = sep["!"] = sep[","] = sep["%"] = true;
  string buff;
  bool aspas = false;
  for(int i = 0,v = S; i < sz(codigo); i++){
    if(codigo[i].c == '"') aspas = !aspas;
    if(sep.find(tokenToString(codigo[i].c)) != sep.end() && !aspas){
      if(sz(buff)){
        if(afd[v].find(EPS) != afd[v].end())
          fita.pb(Token(finais[afd[v][EPS][0]],buff,codigo[i].linha));
        else{
          printf("Separador %c em lugar incorreto na linha %d posição %d.\n",
                  codigo[i].c,codigo[i].linha,codigo[i].pos);
          return false;
        }
      }
      if(codigo[i].c != ' ' && codigo[i].c != '\t' && codigo[i].c != '\n')
        fita.pb(Token(tokenToString(codigo[i].c),tokenToString(codigo[i].c),codigo[i].linha));
      buff.clear();
      v = S;
      continue;
    }
    if(codigo[i].c == '\t' || codigo[i].c == '\n') continue;
    mcviIt t = afd[v].find(codigo[i].c);
    if(t == afd[v].end()){
      printf("Erro na linha %d: caracter na posição %d não reconhecido.\n",
              codigo[i].linha,codigo[i].pos);
      return false;
    }
    buff.pb(codigo[i].c);
    v = afd[v][codigo[i].c][0];
  }
  return true;
}

int main(int argc,char **argv){
  carregarAFD();
  lerCodigo(argv[1]);
  if(analisar()){
    for(int i = 0; i < sz(fita); i++)
      cout << fita[i].tipo << " " << fita[i].nome << " " << fita[i].linha << endl;
  }
  return 0;
}