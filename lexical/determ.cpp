#include "header.h"

AF AFND;
map<int,int> finais;
vector<string> tokens;
int PALAVRAS_RESERVADAS;
int lerInt(){
  int n;
  cin >> n;
  return n;
}

/*recebe um token e adciona ele a gramatica*/
void addToken(int id,string str){
  int uE = S;
  for(int i = 0; i < sz(str); i++){
    AFND[uE][str[i]].pb(sz(AFND));
    uE = sz(AFND);
    AFND[uE] = mcvi();
  }
  finais[uE] = id;
}

/*faz a leitura dos tokens e os adciona a gramatica*/
void procTokens(){
  string linha;
  PALAVRAS_RESERVADAS = lerInt(); cin.ignore();
  for(int i = 0; i < PALAVRAS_RESERVADAS; i++){
    getline(cin,linha);
    tokens.pb(linha);
    linha.pb(EPS);
    addToken(i,linha);
  }
}

/*verifica se o estado novo ja foi identificado
  na gramatica que esta sendo lida da entrada*/
map<string,int> idNovos;
int adcNovo(string novo){
  if(idNovos.find(novo) == idNovos.end()){
    idNovos[novo] = sz(AFND);
    AFND[idNovos[novo]] = mcvi();
  }
  return idNovos[novo];
}

/*Estrutura para guardar os estados finais
  da gramatica da entrada.*/
struct G_Final{
  char c;
  int id;
  G_Final(char _c,int _id){
    c = _c, id = _id;
  }
};

/*Realiza a leitura de uma gramatica da entrada
 e adciona ao automado AFND*/
void procGram(){
  int g = lerInt();
  while(g--){
    int l = lerInt(); cin.ignore();
    string token;
    getline(cin,token);
    tokens.pb(token);
    idNovos.clear();
    vector<G_Final> g_finais;
    for(bool prim = true; l--; prim = false){
      string linha;
      getline(cin,linha);
      string est;
      int i;
      for(i = 0; linha[i] != '<'; i++);
      for(i++; linha[i] != '>'; i++) est.pb(linha[i]);
      for(i++; linha[i] != '='; i++);
      int eID = prim ? (idNovos[est] = S) : adcNovo(est);
      for(; i < sz(linha); i++){
        char nterm;
        string viz;
        for(i++; linha[i] == ' '; i++);
        for(nterm = ' '; i < sz(linha) && linha[i] != '|' &&
                linha[i] != ' ' && linha[i] != '<'; i++) nterm = linha[i];
        nterm = (nterm == EPS) ? ' ' : nterm;
        if(linha[i] == '<') for(i++; linha[i] != '>'; i++) viz.pb(linha[i]);
        else g_finais.pb(G_Final(nterm,eID));
        for(i++; i < sz(linha) && linha[i] != '|'; i++);
        if(sz(viz)) AFND[eID][nterm].pb(adcNovo(viz));
      }
    }
    int tID = adcNovo(token);
    for(int i = 0; i < sz(g_finais); i++){
      AFND[g_finais[i].id][g_finais[i].c].pb(tID);
      finais[tID] = sz(tokens)-1;
    }
    for(int i = 0; i < sz(g_finais); i++)
      if(AFND[g_finais[i].id].find(EPS) == AFND[g_finais[i].id].end())
        AFND[g_finais[i].id][EPS].pb(tID);
  }
}

struct EstNaoDeterm{
  int o,s;
  vector<int> d;
  EstNaoDeterm(int _o,int _s,vector<int> &_d){
    o = _o, s = _s, d = _d;
    sort(d.begin(),d.end());
  }
  bool operator<(const EstNaoDeterm e)const{
    if(sz(d) != sz(e.d)) return sz(d) < sz(e.d);
    for(int i = 0; i < sz(d); i++)
      if(d[i] != e.d[i]) return d[i] < e.d[i];
    return false;
  }
};

struct Transicao{
  int o,s,d;
  Transicao(int _o,int _s,int _d){
    o = _o, s = _s, d = _d;
  }
  bool operator<(const Transicao nt)const{
    if(o != nt.o) return o < nt.o;
    if(s != nt.s) return s < nt.s;
    return d < nt.d;
  }
};

vector<EstNaoDeterm> estNDT;
bool naoDeterm(){
  estNDT.clear();
  for(AFit i = AFND.begin(); i != AFND.end(); i++)
    for(mcviIt j = i->s.begin(); j != i->s.end(); j++)
      if(sz(j->s) > 1) estNDT.pb(EstNaoDeterm(i->f,j->f,j->s));
  return (sz(estNDT) > 0);
}

map<Transicao,bool> checarTrans;
map<EstNaoDeterm,int> checarEst;
void determ(){
  while(naoDeterm()){
    for(int i = 0; i < sz(estNDT); i++){
      AFND[estNDT[i].o][estNDT[i].s].clear();
      EstNaoDeterm dummy = EstNaoDeterm(0,0,estNDT[i].d);
      if(checarEst.find(dummy) != checarEst.end()){
        AFND[estNDT[i].o][estNDT[i].s].pb(checarEst[dummy]);
        continue;
      }
      int ne = sz(AFND);
      AFND[ne] = mcvi();
      checarEst[dummy] = ne;
      //printf("%d %c %d\n",estNDT[i].o,estNDT[i].s,ne);
      AFND[estNDT[i].o][estNDT[i].s].pb(ne);
      for(int j = 0; j < sz(estNDT[i].d); j++){
        mcvi &tj = AFND[estNDT[i].d[j]];

        //printf("  %d %d\n",estNDT[i].d[j],finais.find(estNDT[i].d[j]) != finais.end());
        if(finais.find(estNDT[i].d[j]) != finais.end()){
          //printf("  %d-%d : %s\n",ne,estNDT[i].d[j],tokens[finais[estNDT[i].d[j]]].c_str());
          if(finais[estNDT[i].d[j]] < PALAVRAS_RESERVADAS){
            AFND[ne].erase(EPS);
            finais[ne] = finais[estNDT[i].d[j]];
            //printf("buceta:%d %d\n",finais[estNDT[i].d[j]],PALAVRAS_RESERVADAS);
          }
          else AFND[ne][EPS].pb(estNDT[i].d[j]);
          continue;
        }
        for(mcviIt t = tj.begin(); t != tj.end(); t++){
          for(int k = 0; k < sz(t->s); k++){
            Transicao nt = Transicao(ne,t->f,t->s[k]);
            if(checarTrans.find(nt) == checarTrans.end()){
              checarTrans[nt] = true;
              AFND[ne][t->f].pb(t->s[k]);
            }
          }
        }
      }
      for(mcviIt t = AFND[ne].begin(); t != AFND[ne].end(); t++){
        for(int k = 0; k < sz(t->s); k++){
          if(t->s[k] == ne){
            t->s.clear();
            t->s.pb(ne);
            break;
          }
        }
      }
    }
  }
}

AF afd;
map<int,bool> vis,inutil;
bool dfs(int u){
  vis[u] = true;
  bool achou = false;
  for(mcviIt i = AFND[u].begin(); i != AFND[u].end(); i++)
    if(vis.find(i->s[0]) == vis.end()) achou |= dfs(i->s[0]);
  return achou | (finais.find(u) != finais.end());
}

void minimizar(){
  dfs(S);
  for(AFit e = AFND.begin(); e != AFND.end(); e++)
    if(vis.find(e->f) == vis.end()) inutil[e->f] = true;
  for(AFit e = AFND.begin(); e != AFND.end(); e++){
    if(inutil.find(e->f) != inutil.end()) continue;
    vis.clear();
    if(!dfs(e->f)) inutil[e->f] = true;
  }
  for(AFit e = AFND.begin(); e != AFND.end(); e++){
    if(inutil.find(e->f) != inutil.end()) continue;
    afd[e->f] = AFND[e->f];
  }
}

string intString(int n){
	string str;
	do{
		str.pb((n % 10)+'0');
		n /= 10;
	}while(n);
  reverse(str.begin(),str.end());
	return str;
}

void csv(){
  ofstream file;
  file.open("qwer.csv",ofstream::trunc);
  char b[10];
  for(AFit e = afd.begin(); e != afd.end(); e++){
    if(finais.find(e->f) != finais.end()){
      string buff = intString(e->f) + string(",") + tokens[finais[e->f]] + string(",#\n");
      file.write(buff.c_str(),sz(buff));
      continue;
    }
    for(mcviIt i = e->s.begin(); i != e->s.end(); i++){
      for(int j = 0; j < sz(i->s); j++){
        b[0] = i->f,b[1] = '\0';
        string buff = intString(e->f) + string(",") + string(b) + string(",") + intString(i->s[j]) + string("\n");
        file.write(buff.c_str(),sz(buff));
      }}}
  file.close();
}

void printAF(AF & af){
  for(AFit i = af.begin(); i != af.end(); i++){
    printf("%d %d ::= ",finais.find(i->f) != finais.end(),i->f);
    if(finais.find(i->f) != finais.end()){
      printf("%s |\n",tokens[finais[i->f]].c_str());
      continue;
    }
    for(mcviIt j = i->s.begin(); j != i->s.end(); j++){
      printf("%c<",j->f);
      for(int k = 0; k < sz(j->s); k++){
        if(k) printf(",");
        printf("%d",j->s[k]);
      }
      printf("> | ");
    }
    printf("\n");
  }
}

int main(void){
  AFND[S] = mcvi();
  procTokens();
  procGram();
  printAF(AFND); printf("\n");
  determ();
  minimizar();
  printAF(afd);
  csv();
  return 0;
}