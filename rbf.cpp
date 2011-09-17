#include "rbf.h"
#include "util.h"

#include <fstream>

using namespace std;

RBF::RBF (int neuronas_capa_gaussiana, int neuronas_capa_salida, int n_entradas) {
    capa_gaussiana.resize(neuronas_capa_gaussiana); //TODO inicializar neurona, segundo argumento
    capa_salida.resize(neuronas_capa_salida);
    ncg = neuronas_capa_gaussiana;
    ncs = neuronas_capa_salida;
    entradas = n_entradas;
}

vector<float> RBF::calcular_salida (vector<float> & entrada) {
    vector<float> intermedio(ncg), salida(ncs);

    for (int i = 0; i < ncg; i++) {
        intermedio[i] = capa_gaussiana[i].calcular(entrada);
    }
    for (int i = 0; i < ncs; i++) {
        salida[i] = capa_salida.calcular(intermedio);
    }

    return salida;
}

void RBF::read (const char *filename) {
	int n;

    fstream file (filename, fstream::in);    
    file >> n;
    input.resize(n, vector(entradas));
    result.resize(n, vector(ncs));

	for(int K=0; K<n; ++K){
		for(int L=0; L<entradas; ++L){
			file >> input[K][L];
			file.ignore(); //csv o ssv funciona
		}
		
		for(int L=0; L<ncs; ++L){
			file >> result[K][L];
			file.ignore(); //csv o ssv funciona
		}
	}
}

void RBF::inicializar () {
    int ind;

    for (int i = 0; i < ncg; i++) {
        ind = randomize(0, input.size() - 1);
        capa_gaussiana[i].set_centroid(input[ind]);
    }
}

void RBF::entrenar_capa_gaussiana () {
    int ind;
    bool cambio;

    while (true) {
        vector<vector<float> > centroides_acumulados(ncg, vector<float>(entradas, 0));
        vector<int> cantidad_centroides(ncg, 0);

        for (int i = 0; i < input.size(); i++) {
            ind = centroide_mas_cerca(input[i]);
            centroides_acumulados[ind] = sumar_vectores(centroides_acumulados[ind], input[i]);
            cantidad_centroides[ind]++;
        }

        cambio = false;
        for (int i = 0; i < ncg; i++) {
            centroides_acumulados[i] = dividir_vector(centroides_acumulados[i], cantidad_centroides[i]);
            // si el centroide cambia, actualizo la bandera
            if (capa_gaussiana[i].set_centroid(centroides_acumulados[i])) cambio = true;
        }

        if (!cambio) break;
    }
    //TODO varianza
}

int entrenar_capa_salida (int cant_epocas, float acierto_minimo) {
    vector<float> salida_obtenida;
    int epoca;

    for (epoca = 1; epoca <= cant_epocas; epoca++) {
        aciertos = 0;

        for (int i = 0; i < input.size(); i++) { // por cada patron de entrenamiento
            salida_obtenida = calcular_salida(input[i]);
            if (comparar_vectores(salida_obtenida, result[i])) { // si acierta
                aciertos++;
            } 
            else { // si no son iguales, entreno las neuronas
                for (j = 0; j < ncs; j++) {
                    capa_salida[j].entrenar(salida_obtenida, result[i]);
                }
            }
        }

        // si el porcentaje de aciertos es el deseado, salgo
        if ( (float(aciertos)/input.size()) >= acierto_minimo) break;
    }

    if (epoca <= cant_epocas) { // si convergio
        return epoca;
    }
    else {
        return -1;
    }
}
