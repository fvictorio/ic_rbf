#ifndef NEURONA_H
#define NEURONA_H

#include <vector>

using namespace std;

class neurona {
public:
    neurona (int cant_entradas, float eta);
    float calcular (vector<float> & entrada);
    void entrenar (vector<float> & entradas, float error);
    void inicializar ();
private:
    float eta;
    vector<float> weights;    
};

#endif
