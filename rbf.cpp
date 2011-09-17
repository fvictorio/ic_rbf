#include "rbf.h"
#include "util.h"

#include <fstream>

using namespace std;

RBF::RBF (int neuronas_capa_gaussiana, int neuronas_capa_salida, int n_entradas, float eta) {
    capa_gaussiana.resize(neuronas_capa_gaussiana); 
    capa_salida.resize(
            neuronas_capa_salida, 
            neurona(neuronas_capa_gaussiana, eta) // inicializacion de la neurona
    );
    ncg = neuronas_capa_gaussiana;
    ncs = neuronas_capa_salida;
    entradas = n_entradas;

    // Inicializar los pesos de las neuronas de salida
    for (int i = 0; i < neuronas_capa_salida; i++) {
        capa_salida[i].inicializar();
    }
}

vector<float> RBF::calcular_intermedio (vector<float> & entrada) {
    vector<float> intermedio(ncg+1);

    for (int i = 0; i < ncg; i++) {
        intermedio[i] = capa_gaussiana[i].calcular(entrada);
    }
    intermedio[ncg] = 1; // entrada aumentada
    return intermedio;
}

vector<float> RBF::calcular_salida_con_intermedio (vector<float> & intermedio) {
    vector<float> salida(ncs);

    for (int i = 0; i < ncs; i++) {
        salida[i] = capa_salida[i].calcular(intermedio);
    }

    return salida;
}

vector<float> RBF::calcular_salida (vector<float> & entrada) {
    vector<float> salida;
    salida = calcular_intermedio(entrada);
    salida = calcular_salida_con_intermedio(salida);
    return salida;
}

void RBF::read (const char *filename) {
	int n;

    fstream file (filename, fstream::in);    
    file >> n;
    input.resize(n, vector<float>(entradas));
    result.resize(n, vector<float>(ncs));

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

int RBF::entrenar_capa_salida (int cant_epocas, float acierto_minimo) {
    vector<float> intermedio, salida_obtenida;
    int epoca, aciertos;

    for (epoca = 1; epoca <= cant_epocas; epoca++) {
        aciertos = 0;

        for (int i = 0; i < input.size(); i++) { // por cada patron de entrenamiento
            intermedio = calcular_intermedio(input[i]);
            salida_obtenida = calcular_salida_con_intermedio(intermedio);
            if (comparar_vectores(salida_obtenida, result[i])) { // si acierta
                aciertos++;
            } 
            else { // si no son iguales, entreno las neuronas
                for (int j = 0; j < ncs; j++) {
                    capa_salida[j].entrenar(intermedio, result[i][j] - salida_obtenida[j]);
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

int RBF::centroide_mas_cerca (vector<float> & punto) {
    int i_min = 0;
    float dist_min, dist;

    dist_min = capa_gaussiana[0].distancia2(punto);

    for (int i = 1; i < ncg; i++) {
        dist = capa_gaussiana[i].distancia2(punto);
        if (dist < dist_min) {
            dist_min = dist;
            i_min = i;
        }
    }

    return i_min;
}
