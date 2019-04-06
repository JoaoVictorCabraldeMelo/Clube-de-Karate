/*
Este Código foi feito pelo alunos :
    Nome: Oscar E. B. Madureira da Silva
    Matrícula:17/0112209
    
    Nome: João Victor Cabral de Melo
    Matrícula: 16/0127670
    
    Estes alunos são estudantes da UnB Universidade de Brasília - Brasil
    Este código foi criado como trabalho da matéria de TAG - Teoria e Aplicação de Grafos
*/
#include <bits/stdc++.h>
#define W(x, y) cerr << "\033[31m" << #x << " = " << x << "\033[0m" << y;

using namespace std;

//Criando estrutura para a rede do Clube
typedef vector<int> vertex;
typedef vector<vertex> clubeKarate;

//Lendo do arquivo .gml criando um parser em c++
clubeKarate criaGrafo(clubeKarate clube){
    ifstream file;
    file.open("karate.gml", ios::in);
    string  line, id;
    int source, target, existTargetSource = 0;
    while(getline(file,line)){
        if(line.length() >= 4 && line.substr(4,2) == "id"){ //Se a linha for "id n"
            clube.push_back(vertex()); //cria seu vertice com sua lista
        } else if(line.length() >= 4  && line.substr(4,6) == "source"){//Se a linha for "source n"
            source = stoi(line.substr(11,2)) - 1; //pega valor do source no file                         
            existTargetSource++; //conta se aresta tem vertice de saida e de entrada
        }
        if(line.length() >= 4 && line.substr(4,6) == "target"){//Se a linha for "target"
            target = stoi(line.substr(11,2)) - 1; //pega valor do target no file
            existTargetSource++; //conta se aresta tem vertice de saida e de entrada
        }
        if(existTargetSource == 2){ //se aresta tem vertice de saida e de entrada 
            clube[source].push_back(target);  //coloca a conexão em source
            clube[target].push_back(source);  //coloca a conexão em target
            existTargetSource = 0;            //seta o valor de pares pra 0
        }

    }

    file.close();
    return clube;
}


//Função mostra o grafo no terminal 
void printGrafo(clubeKarate clube){
    int i = 1, grau = 0;
    for(vertex v:clube){ //para cada pessoa no clube faça
        cout << "Vertice : " << setfill(' ') << setw(2) << i ;
        i++;
        for(int numero:v){ //para cada amigo da pessoa no clube faça 
            grau++;        //calculo de amigos
        }
        cout << " tem grau igual a -> " << grau << endl;
        grau = 0;
    }
}

vector<int> uniao(vector<int> &a, vector<int>&b){
    vector<int> res;
    int i = 0, j = 0;
    while(i < a.size() && j < b.size()){
        if(a[i] < b[j]){
            res.push_back(a[i]);
            i++;
        } else if(a[i] > b[j]){
            res.push_back(b[j]);
            j++;
        } else{
            res.push_back(a[i]);
            i++;
            j++;
        }
    }
    while(i < a.size()){
        res.push_back(a[i]);
        i++;
    }
    while(j < b.size()){
        res.push_back(b[j]);
        j++;
    }
    return res;
}

vertex interseccao(vertex &a, vertex&b){
    vertex res;
    int i = 0, j = 0;
    while(i < a.size() && j < b.size()){
        if(a[i] < b[j]){
            i++;
        } else if(a[i] > b[j]){
            j++;
        } else{
            res.push_back(a[i]);
            i++;
            j++;
        }
    }
    return res;
}

void bronKerbosch(vertex conjuntoR, vertex conjuntoP, vertex conjuntoX, vector<vertex> &res, clubeKarate &clube){
    if(conjuntoP.empty() && conjuntoX.empty()){
        res.push_back(conjuntoR);
    }
    while(!conjuntoP.empty()){
        vertex aux(conjuntoP.begin(), conjuntoP.begin()+1);
        bronKerbosch(
            uniao(conjuntoR, aux), // União do conjunto R com o conjunto {i}
            interseccao(conjuntoP, clube[aux[0]]), // Intersecção conjunto P com os vizinhos de i
            interseccao(conjuntoX, clube[aux[0]]), // Intersecção conjunto X com os vizinhos de i
            res, 
            clube); 
        conjuntoP.erase(conjuntoP.begin());
        conjuntoX = uniao(conjuntoX, aux); // União do conjunto R com o conjunto {i}
    }
}

int main(int argc, char const *argv[]){
    clubeKarate clube;
    vertex R, // Conjunto R
        P, // Conjunto P
        X; // Contunto X
    vector<vertex> res;
    vector<vector<int>> resposta; // Resposta Apresentavel
    vector<double> coeficienteAglomeracao(34); // Coeficiente de aglomeração de cada vertice
    double coeficienteAglomeracaoMedio; // Coeviciente de agomeração médio
    clube = criaGrafo(clube);
    printGrafo(clube);


    /* Calcula o coeficiente de agloreração e o coeficiete de aglomeração média
        O Coeficiente de aglomeração média é a média do coeficiente de agloreração de cada vértice
    */
    for(int i = 0; i < clube.size(); i++){
        int numerador = 0;
        int n = clube[i].size();
        /* Nesse laço o número de vizinho tem conexões entre si é contado em dobro Ex.: E[i][j] e E[j][i] são contados */
        for(int j = 0; j < clube[i].size(); j++){
            numerador += interseccao(clube[clube[i][j]], clube[i]).size();
        }
        /* Aqui o coeficiente é calculado */
        coeficienteAglomeracao[i] = (n == 1 ? (double)0.0 : (double)numerador/(n*(n-1))); /* O denominador é n*(n-1), pois, o mesmo vertice é contoado 2 vezes em direções opostas */
        coeficienteAglomeracaoMedio += coeficienteAglomeracao[i];
        cout << "O Coeficiente de aglomeracao para o vertice " << setfill(' ') << setw(2) << i+1 <<  " eh " << coeficienteAglomeracao[i] << endl;
    }
    coeficienteAglomeracaoMedio /= clube.size();
    cout << endl;
    cout << "O Coeficiente de aglomeracao para o vertice medio eh " << coeficienteAglomeracaoMedio << endl;
    
    for(int i = 0; i < clube.size(); i++) P.push_back(i);
    bronKerbosch(R, P, X, res, clube);
    cout << endl;
    cout << "Existem " << res.size() << " cliques maximais." << endl;
    for(int i = 0; i < res.size(); i++){
        auto &p = res[i];
        cout << "Clique maximal " << setfill(' ') << setw(2) << i+1 <<", com " << p.size() << " vertices pertencentes ao conjuto { ";
        for(int j = 0; j < p.size()-1; j++){
            cout << p[j]+1 << ", ";
        }
        cout << p[p.size()-1]+1 << " }" << endl;
    }
    return 0;
}
