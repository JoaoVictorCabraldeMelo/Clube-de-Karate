#include <bits/stdc++.h>

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
        cout << "Vertice : " << i ;
        i++;
        for(int numero:v){ //para cada amigo da pessoa no clube faça 
            grau++;        //calculo de amigos
        }
        cout << " tem grau igual a -> " << grau << endl;
        grau = 0;
    }
}

void bronKerbosch(bitset<34> conjuntoR, bitset<34> conjuntoP, bitset<34> conjuntoX, vector<bitset<34>> &res, vector<bitset<34>> &vizinhos){
    if(conjuntoP.none() && conjuntoX.none()){
        res.push_back(conjuntoR);
    }
    for(int i = 0; i < 35; ++i){
        if(conjuntoP[i]){
            bronKerbosch(
                conjuntoR | bitset<34>().set(i, 1), // União do conjunto R com o conjunto {i}
                conjuntoP & vizinhos[i], // Intersecção conjunto P com os vizinhos de i
                conjuntoX & vizinhos[i], // Intersecção conjunto X com os vizinhos de i
                res, 
                vizinhos); 
            conjuntoP[i] = 0; // Complementar conjuntoP em relação ao conjunto {i}
            conjuntoX.set(i); // União do conjunto R com o conjunto {i}
        }
    }
}

int main(int argc, char const *argv[]){
    clubeKarate clube;
    bitset<34> R, // Conjunto R
        P, // Conjunto P
        X; // Contunto X
    vector<bitset<34>> vizinhos;
    vector<bitset<34>> res;
    vector<vector<int>> resposta; // Resposta Apresentavel
    clube = criaGrafo(clube);
    printGrafo(clube);
    
    vizinhos.resize(clube.size());
    for(int i = 0; i < clube.size(); i++){
        for(int j = 0; j < clube[i].size(); j++){
            vizinhos[i].set(clube[i][j]);
        }
    }
    for(int i = 0; i < vizinhos.size(); i++){
        int numerador = 0;
        int n = vizinhos[i].count();
        for(int j = 0; j < 34; j++){
            if(vizinhos[i][j] && j != i){
                numerador += ((vizinhos[j] & vizinhos[i]) | bitset<34>().set(1)).count();
            }
        }
        cout << (double)numerador/((n+2)*(n-1)) << endl;
        //cout << numerador << ' ' << ((n+2)*(n+1)) << endl;

    }
    P.set();
    bronKerbosch(R, P, X, res, vizinhos);
    resposta.resize(res.size());
    for(int i = 0; i < res.size(); i++){
        for(int j = 0; j < 34; ++j){
            if(res[i][j]){
                resposta[i].push_back(j);
            }
        }
    }
    cout << endl << "Existem " << res.size() << " cliques maximais." << endl;
    for(int i = 0; i < resposta.size(); i++){
        auto &p = resposta[i];
        cout << "Clique maximal " << i+1 <<"\t, com " << p.size() << " e vertice pertencentes ao conjuto { ";
        for(int j = 0; j < p.size()-1; j++){
            cout << p[j]+1 << ", ";
        }
        cout << p[p.size()-1]+1 << " }" << endl;
    }
    return 0;
}

