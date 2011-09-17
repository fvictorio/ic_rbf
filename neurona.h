#ifdef NEURONA_H
#define NEURONA_H

using namespace std;

class neurona {
public:
    float calcular (vector<float> & entrada);
    void entrenar (vector<float> & obtenido, vector<float> & deseado);
private:
    
};

#endif
