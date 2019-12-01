#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_PARADAS    5  // número de paradas de la ruta
#define EN_RUTA      0  // autobús en ruta
#define EN_PARADA    1  // autobús en la parada
#define MAX_USUARIOS 40 // capacidad del autobús
#define USUARIOS     4  // Número usuarios del servicio
#define DELAY        2  // Segundos que trascurren entre paradas

// Variables de estado:
int estado = EN_RUTA;
int parada_actual = 0;
int n_ocupantes = 0;

int esperando_parada[N_PARADAS];    //= {0,0,...0};
int esperando_bajar[N_PARADAS];     //= {0,0,...0};

pthread_mutex_t mutex_esperando_parada;
pthread_mutex_t mutex_esperando_subida;
pthread_mutex_t mutex_usuarios;


void  Autobus_En_Parada();
void  Conducir_Hasta_Siguiente_Parada();
void* thread_autobus(void * args);
void  Subir_Autobus(int id_usuario, int origen);
void  Bajar_Autobus(int id_usuario, int destino);
void  Usuario(int id_usuario, int origen, int destino);
void* thread_usuario(void * arg);


int main(int argc, char* argv[])
{
	int i=0;

    //TODO: INICIALIZAR LOS MUTEXES!!

	return 0;
}

/* Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
quieran bajar y/o subir la parada actual. Después se pone en marcha */
void Autobus_En_Parada() {
    estado = EN_PARADA;

    // TODO: AVISAMOS A LOS PASAJEROS DE QUE ESTAMOS EN PARADA

    while(esperando_bajar[parada_actual]!=0 || esperando_parada[parada_actual]!=0) {
        // TODO: CUANDO CADA USUARIO SE BAJE NOS AVISA Y RECOMPROBAMOS LA CONDICIÓN
    }
}

/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
void Conducir_Hasta_Siguiente_Parada(){
    estado = EN_RUTA;
    sleep(DELAY);
    parada_actual = (parada_actual+1)%N_PARADAS;   
}

void* thread_autobus(void * args) {
    while (1) {
        // esperar a que los viajeros suban y bajen
        Autobus_En_Parada();
    
        // conducir hasta siguiente parada
        Conducir_Hasta_Siguiente_Parada();
    }
}

/* El usuario indicará que quiere subir en la parada ’origen’, esperará a que
el autobús se pare en dicha parada y subirá. El id_usuario puede utilizarse para
proporcionar información de depuración */
void Subir_Autobus(int id_usuario, int origen){

    pthread_mutex_lock(&mutex_esperando_subida);
    esperando_subida[origen]++;
    pthread_mutex_unlock(&mutex_esperando_subida);
    

    while (parada_actual!=origen || estado!=EN_PARADA){
        //TODO: ESPERAMOS A QUE LLEGUE EL BUS
    }

    
    pthread_mutex_lock(&mutex_esperando_subida);
    esperando_subida[origen]--;
    pthread_mutex_unlock(&mutex_esperando_subida);
    
    pthread_mutex_lock(&mutex_usuarios);
    n_ocupantes++;
    pthread_mutex_unlock(&mutex_usuarios);
    
    //TODO: AVISAMOS AL BUS DE QUE NOS HEMOS SUBIDO
}

/* El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
proporcionar información de depuración */
void Bajar_Autobus(int id_usuario, int destino){

    pthread_mutex_lock(&mutex_esperando_parada);
    esperando_parada[destino]++;
    pthread_mutex_unlock(&mutex_esperando_parada);
    

    while (parada_actual!=destino || estado!=EN_PARADA){
        //TODO: ESPERAMOS A QUE LLEGUE EL BUS
    }
    
    pthread_mutex_lock(&mutex_esperando_parada);
    esperando_parada[destino]--;
    pthread_mutex_unlock(&mutex_esperando_parada);
    
    
    pthread_mutex_lock(&mutex_usuarios);
    n_ocupantes--;
    pthread_mutex_unlock(&mutex_usuarios);

    //TODO: AVISAMOS AL BUS DE QUE NOS HEMOS BAJADO

}

void Usuario(int id_usuario, int origen, int destino) {
    // Esperar a que el autobus esté en parada origen para subir
    Subir_Autobus(id_usuario, origen);
    // Bajarme en estación destino
    Bajar_Autobus(id_usuario, destino);
}

void* thread_usuario(void * arg) {
    int id_usuario = (int)arg;                                                   // REVISAR ESTO!!
    // obtener el id del usario
    int a,b;   
    while (1) {
        a=rand() % N_PARADAS;
        do{
            b=rand() % N_PARADAS;
        } while(a==b);
        Usuario(id_usuario,a,b);
    }
}


