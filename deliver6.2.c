#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

int contador;

//Estructura necesaria para acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *AtenderCliente (void *socket)
{
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	//int socket_conn = * (int *) socket;
	
	char peticion[512];
	char respuesta [512];
	int ret;


	int terminar = 0;
	
	while (terminar == 0)
	{
	
	ret=read (sock_conn,peticion, sizeof(peticion));
	printf ("Recibido\n");
	
	peticion[ret]='\0';
	
	printf ("Peticion: %s\n",peticion);
	
	char *p = strtok( peticion, "/");
	int codigo = atoi (p);

	char nombre [20];
	
	if ((codigo != 0) && (codigo != 4))
	{
		p = strtok( NULL, "/");
		
		strcpy (nombre, p);
		printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
	}
	
	if (codigo == 0) //Peticion de desconexion
		terminar = 1;
	else if (codigo == 4)
		sprintf (respuesta,"%d", contador);
	else if (codigo == 1) 
		sprintf (respuesta, "%d", strlen (nombre));
	else if (codigo == 2)
		if((nombre [0]=='M') || (nombre [0] == 'S'))
		strcpy (respuesta, "SI");
		else
			strcpy (respuesta, "NO");
		else
		{
			p = strtok( NULL, "/");
			float altura = atof (p);
			if (altura > 1.70)
				sprintf (respuesta, "%s: eres alto", nombre);
			else
				sprintf (respuesta, "%s: eresbajo",nombre);
		}
		
		if (codigo != 0)
		{	
			printf ("Respuesta: %s\n", respuesta);
			write (sock_conn, respuesta, strlen(respuesta));
		}
			
		if ((codigo == 1) || (codigo == 2) || (codigo == 3))
		{
			pthread_mutex_lock( &mutex );
			contador = contador + 1;
			pthread_mutex_unlock( &mutex);
		}
	
	}
	close(sock_conn);
			
}