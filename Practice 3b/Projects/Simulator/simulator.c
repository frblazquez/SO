#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_PARADAS    20 // número de paradas de la ruta
#define EN_RUTA      0  // autobús en ruta
#define EN_PARADA    1  // autobús en la parada
#define MAX_USUARIOS 20 // capacidad del autobús
#define USUARIOS     10 // Número usuarios del servicio
#define DELAY        1  // Segundos que trascurren entre paradas

// Variables de estado:
int estado = EN_RUTA;
int parada_actual = 0;
int n_ocupantes = 0;

int esperando_parada[N_PARADAS];    //= {0,0,...0};
int esperando_bajar[N_PARADAS];     //= {0,0,...0};

pthread_mutex_t mutex_usuarios;
pthread_cond_t  autobus_parada;
pthread_cond_t  movimiento_usuario;


void  Autobus_En_Parada();
void  Conducir_Hasta_Siguiente_Parada();
void* thread_autobus(void * args);
void  Subir_Autobus(int id_usuario, int origen);
void  Bajar_Autobus(int id_usuario, int destino);
void  Usuario(int id_usuario, int origen, int destino);
void* thread_usuario(void * arg);


int main(int argc, char* argv[])
{   
    int i;
    for(i=0; i < N_PARADAS; i++){
        esperando_parada[i]=0;
        esperando_bajar[i]=0;
    }    
    
    pthread_t autobus;
    pthread_t usuario[USUARIOS];

    pthread_mutex_init(&mutex_usuarios,    NULL);
    pthread_cond_init(&autobus_parada,     NULL);
    pthread_cond_init(&movimiento_usuario, NULL);
     

    pthread_create(&autobus,  NULL, thread_autobus, NULL);
    for(i=0; i< USUARIOS; i++){
        pthread_create(&usuario[i], NULL, thread_usuario, i);
    }

    pthread_join(autobus,  NULL);
    for(i=0; i<USUARIOS; i++){    
        pthread_join(usuario[i], NULL);
    }

    pthread_mutex_destroy(&mutex_usuarios);
    pthread_cond_destroy(&autobus_parada);
    pthread_cond_destroy(&movimiento_usuario);

	return 0;
}

/* Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
quieran bajar y/o subir la parada actual. Después se pone en marcha */
void Autobus_En_Parada() {

    pthread_mutex_lock(&mutex_usuarios);
    estado = EN_PARADA;
    printf("El autobús para en la parada %d\n", parada_actual);

    pthread_cond_broadcast(&autobus_parada);

    while(esperando_bajar[parada_actual]!=0 || esperando_parada[parada_actual]!=0) {
        pthread_cond_wait(&movimiento_usuario, &mutex_usuarios);
    }
    
    pthread_mutex_unlock(&mutex_usuarios);
}

/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
void Conducir_Hasta_Siguiente_Parada(){
    estado = EN_RUTA;
    printf("Autobús en marcha\n");
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

    pthread_mutex_lock(&mutex_usuarios);
    esperando_parada[origen]++;    

    while (parada_actual!=origen || estado!=EN_PARADA){
        pthread_cond_wait(&autobus_parada, &mutex_usuarios);
    }
    
    esperando_parada[origen]--;
    n_ocupantes++;
    printf("El usuario %d se sube al autobus\n", id_usuario);
    
    pthread_cond_signal(&movimiento_usuario);
    pthread_mutex_unlock(&mutex_usuarios);    
}

/* El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
proporcionar información de depuración */
void Bajar_Autobus(int id_usuario, int destino){

    pthread_mutex_lock(&mutex_usuarios);
    esperando_bajar[destino]++;
    

    while (parada_actual!=destino || estado!=EN_PARADA){
        pthread_cond_wait(&autobus_parada, &mutex_usuarios);
    }
    
    esperando_bajar[destino]--;
    n_ocupantes--;
    printf("El usuario %d se baja del autobus\n", id_usuario);
    
    pthread_cond_signal(&movimiento_usuario);
    pthread_mutex_unlock(&mutex_usuarios);
}

void Usuario(int id_usuario, int origen, int destino) {
    // Esperar a que el autobus esté en parada origen para subir
    Subir_Autobus(id_usuario, origen);
    // Bajarme en estación destino
    Bajar_Autobus(id_usuario, destino);
}

void* thread_usuario(void * arg) {
    int id_usuario = (int)arg;
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


