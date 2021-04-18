#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50
sem_t mutex;
sem_t mutex1;
sem_t mutex2;

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
	//poner demas semaforos aqui
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_hayLugar;
	sem_t sem_empanar;
	sem_t sem_freir;
	sem_t sem_hornear;
	sem_t sem_panHorneado;
	sem_t sem_terminoExtra;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
    pthread_exit(NULL);
}

void* cortarExtra(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortarExtra";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_terminoExtra);
    pthread_exit(NULL);
}

void* mezclar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "mezclar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//espero la señal del semafor de mezclar
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita salar)
	sem_post(&mydata->semaforos_param.sem_salar);
	//sem_post(&mydata->semaforos_param.sem_hayLugar);
    pthread_exit(NULL);
}

void* salar(void *data) {
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//esperando señal de la accion anterior
	sem_wait(&mydata->semaforos_param.sem_salar);
	// esperando señal del semaforo compartido
	sem_wait(&mutex);  
	printf("\tEquipo %d -Salando mezcla-\n",mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal del semaforo compartido
	sem_post(&mutex);
	//doy señanl a la siguiente accion (salar me habilita a agregarCarne)
	sem_post(&mydata->semaforos_param.sem_agregarCarne);  
    pthread_exit(NULL);
	
}

void* agregarCarne(void *data) {
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//esperando señal del semaforo de agregarCarne
	sem_wait(&mydata->semaforos_param.sem_agregarCarne);
	//imprimo por pantalla
	printf("\tEquipo %d -agregarCarne-\n",mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita a empanar)
	sem_post(&mydata->semaforos_param.sem_empanar);
    pthread_exit(NULL);
}

void* empanar(void *data) {
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//esperando señal del semaforo empanar
	sem_wait(&mydata->semaforos_param.sem_empanar);
	//imprimo por pantalla
	printf("\tEquipo %d -Empanando la Carne-\n",mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (empanar me habilita freir)
	sem_post(&mydata->semaforos_param.sem_freir);
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    pthread_exit(NULL);
}

void* freir(void *data) {
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//esperando la señal del semaforo freir
	sem_wait(&mydata->semaforos_param.sem_freir);
	//imprimo por pantalla
	printf("\tEquipo %d -Esperando para Freir-\n",mydata->equipo_param);
	//esperando señal del semaforo compartido
	sem_wait(&mutex);
	//imprimir por pantalla
	printf("\tEquipo %d -Freir-\n",mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep(20000);
	//doy la señala al semaforo compartido
	sem_post(&mutex);
	//imprimo por pantalla
	printf("\tEquipo %d -Termino de Freir-\n",mydata->equipo_param);
	//doy la señal a la siguiente accion (me habilita hornear pan)
	sem_post(&mydata->semaforos_param.sem_hornear);
    pthread_exit(NULL);
}

void* hornear(void *data){
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
	//esperando señal del semaforo hornear
	sem_wait(&mydata->semaforos_param.sem_hornear);
	//esperando la señal del semaforo compartido
	sem_wait(&mutex1);
	//Imprimo por pantalla
	printf("\tEquipo %d -Horneado Pan-\n",mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep(20000);
	//doy la señal al semaforo compartido
	sem_post(&mutex1);
	//doy la señal al semaforo compartido
	sem_post(&mutex1);
	//imprimo por pantalla
	printf("\tEquipo %d -Termino de Hornear Pan-\n",mydata->equipo_param);
	//doy la señal a la siguiente accion (me habilita hornear pan horneado)
	sem_post(&mydata->semaforos_param.sem_panHorneado);
	pthread_exit(NULL);
}

void* armarMilanesa (void *data){
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_terminoExtra);
	sem_wait(&mydata->semaforos_param.sem_panHorneado);
	printf("\tEquipo %d -ArmandoMilanesa-\n",mydata->equipo_param);
	printf("\tEquipo %d -Gano-\n",mydata->equipo_param);
	pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	//crear variables semaforos aqui
	sem_t sem_salar;
	sem_t sem_empanar;
	sem_t sem_agregarCarne;
	sem_t sem_freir;
	sem_t sem_hornear;
	sem_t sem_panHorneado;
	sem_t sem_terminoExtra;
	
	//variables hilos
	pthread_t p1; 
	//crear variables hilos aqui
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
	pthread_data->semaforos_param.sem_freir = sem_freir;
	pthread_data->semaforos_param.sem_hornear = sem_hornear;
	pthread_data->semaforos_param.sem_panHorneado = sem_panHorneado;
	pthread_data->semaforos_param.sem_terminoExtra = sem_terminoExtra;
	
	

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
     	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	    strcpy(pthread_data->pasos_param[0].ingredientes[0], "primer diente de ajo");
		strcpy(pthread_data->pasos_param[0].ingredientes[1], "segudno diente de ajo");
		strcpy(pthread_data->pasos_param[0].ingredientes[2], "perejil");


	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], "primero diente de ajo");
		strcpy(pthread_data->pasos_param[1].ingredientes[1], "segundo diento de ajo");
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], "perejil");
		strcpy(pthread_data->pasos_param[1].ingredientes[3], "huevo");
		
		strcpy(pthread_data->pasos_param[1].accion, "cortarExtra");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], "Lechuga Fresca");
		strcpy(pthread_data->pasos_param[1].ingredientes[1], " Tomate");
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], "Cebolla Morada");
		strcpy(pthread_data->pasos_param[1].ingredientes[3], "Pepinos");
	
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	//inicializar demas semaforos aqui
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_freir),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_hornear),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_panHorneado),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_terminoExtra),0,0);
	


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	
	//crear demas hilos aqui
	
	rc = pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortarExtra,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	
	rc= pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
										
	rc= pthread_create(&p3,                           //identificador unico
                            NULL,                          //atributos del thread
                                salar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
								
	rc= pthread_create(&p4,                           //identificador unico
                            NULL,                          //atributos del thread
                                agregarCarne,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
								
	rc= pthread_create(&p5,                           //identificador unico
                            NULL,                          //atributos del thread
                                empanar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
								
	rc= pthread_create(&p6,                           //identificador unico
                            NULL,                          //atributos del thread
                                freir,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

	rc= pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                hornear,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
								
	rc= pthread_create(&p8,                           //identificador unico
                            NULL,                          //atributos del thread
                                armarMilanesa,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
								
								
	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);

	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos 
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_freir);
	sem_destroy(&sem_hornear);
	sem_destroy(&sem_panHorneado);
	sem_destroy(&sem_terminoExtra);
	
	//salida del hilo
	 pthread_exit(NULL);
}

int main ()
{
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
	//faltan equipos
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre3 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;
	//faltan hilos
	
	//inicializo los semaforo comparitdo
	sem_init(&mutex, 0, 1);
	sem_init(&mutex1, 0, 2);
	sem_init(&mutex2, 0, 1);
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 
    
    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
  //faltan inicializaciones
	rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
								
	rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	
//.. faltan joins
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
