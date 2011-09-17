#include "util.h"
#include "neurona.h"

using namespace std;

neurona::neurona (int cant_entradas, float eta) {
    weights.resize(cant_entradas + 1); // entrada aumentada
    this->eta = eta;
}

float neurona::calcular (vector<float> & entrada) {
    return signo(multiplicar_vectores(weights, entrada));
}

void neurona::entrenar (vector<float> & entradas, float error) {
    for (int i = 0; i < weights.size(); i++) {
        weights[i] += eta * error * entradas[i];
    }
}

void neurona::inicializar () {
    for (int i = 0; i < weights.size(); i++) {
        weights[i] = randomize(-0.5, 0.5);
    }
}
