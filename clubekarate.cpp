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

int main(int argc, char const *argv[]){
    clubeKarate clube;
    clube = criaGrafo(clube);
    printGrafo(clube);
    return 0;
}

