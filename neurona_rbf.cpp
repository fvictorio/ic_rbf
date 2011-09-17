#include <cmath>
#include "util.h"
#include "neurona_rbf.h"

using namespace std;

float neurona_rbf::calcular (vector<float> & entrada) {
    return float(42);
}

bool neurona_rbf::set_centroid (vector<float> & new_centroid) {
    // Devuelve falso si el nuevo centroide no es distinto al actual
    
    if (comparar_vectores(centroid, new_centroid)) {
        return false; // no cambia
    }
    centroid = new_centroid;
    return true;
}

float gaussian (float x) {
    return exp(-((x*x)/(2*varianza)));
}
