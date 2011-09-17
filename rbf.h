#ifdef RBF_H
#define RBF_H

#include <iostream>
#include <vector>

using namespace std;

class RBF {
public:
    RBF (int neuronas_capa_gaussiana, int neuronas_capa_salida, int n_entradas);
    vector<float> calcular_salida (vector<float> & entrada);
    void read (const char *filename);
    void inicializar ();
    void entrenar_capa_gaussiana ();
    int entrenar_capa_salida (int cant_epocas, float acierto_minimo);

private:
    vector<neurona_rbf> capa_gaussiana;
    vector<neurona> capa_salida;
    vector<vector<float> > input, result;
    int entradas;
    int ncg, ncs; // numero de neuronas de cada capa
};

#endif
