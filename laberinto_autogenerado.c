#include <stdio.h>
#include <time.h> /* Nueva librería necesaria para la función srand */

// Cola para BFS
typedef struct {
    int x, y;
} Nodo;

void imprimirLabResuelto(int fila, int col, int laberinto[fila][col], int fila_inicio, int col_inicio){
	int i, j;
	for(i = 0; i< fila+2; i++) {
		printf("\x1b[30;42m||\033[0m");
	}
	printf("\n");
	
	for(i = 0; i< fila; i++) {
		printf("\x1b[30;42m||\033[0m");
		for(j = 0; j< col; j++) {
			if(laberinto[i][j] == 1)
				printf("\x1b[30;42m||\033[0m");
			if(laberinto[i][j] == 0)
				printf("\x1b[30;47m::\033[0m");
			if(laberinto[i][j] == 2)
				printf("\x1b[30;43m::\033[0m");
			if(laberinto[i][j] == 3)
				printf("\x1b[30;41mE>\033[0m");
			if(laberinto[i][j] == 4)
				printf("\x1b[30;41mS>\033[0m");
		}
		printf("\x1b[30;42m||\033[0m");
		printf("\n");
	}
	
	for(i = 0; i< fila+2; i++) {
		printf("\x1b[30;42m||\033[0m");
	}
	printf("\n");
}

void reconstruir_camino(int fila, int col, int fila_fin, int col_fin,
						int laberinto[fila][col], int fila_inicio, int col_inicio,
						Nodo padre[][col]){
	int len = 0;
	Nodo cur = (Nodo){fila_fin, col_fin};
	
    while (!(cur.x == fila_inicio && cur.y == col_inicio)) {
        laberinto[cur.x][cur.y] = 2;
	    cur = padre[cur.x][cur.y];
	}
		   
	return;
}

void limpiar_entradas(int fila, int col, int laberinto[fila][col], int fila_inicio, int col_inicio) {
	
	int fila_final = fila - 1 ;
	int col_final = col -1 ;
	
	laberinto[fila_final][col_final] = 0;
	laberinto[fila_final-1][col_final] = 0;
	laberinto[fila_final][col_final-1] = 0;
	
	laberinto[fila_inicio][col_inicio] = 0;
	laberinto[fila_inicio][col_inicio+1] = 0;
	laberinto[fila_inicio+1][col_inicio] = 0;
	
	return;
	
}

void encolar(int x, int y, Nodo cola[], int *fin) {
    cola[*fin].x = x;
    cola[*fin].y = y;
    (*fin)++;
}

Nodo desencolar(Nodo cola[], int *frente) {
    return cola[(*frente)++];
}

int bfs(int fila_inicio, int col_inicio, int fila_fin, int col_fin,
        Nodo cola[], int *frente, int *fin,
        int fila, int col, Nodo padre[][col],
        int laberinto[fila][col],
        int visitado[fila][col],
        int dist[fila][col],
        int dx[], int dy[]) {
    
    int i;
    encolar(fila_inicio, col_inicio, cola, fin);
    
    visitado[fila_inicio][col_inicio] = 1;
    dist[fila_inicio][col_inicio] = 0;

    while (*frente < *fin) {
    	
        Nodo actual = desencolar(cola, frente);

        if (actual.x == fila_fin && actual.y == col_fin) {		    
            return dist[fila_fin][col_fin];
        }

        for (i = 0; i < 4; i++) {
        	
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];

            if (nx >= 0 && nx < fila && ny >= 0 && ny < col) {
            	
                if (laberinto[nx][ny] == 0 && !visitado[nx][ny]) {
                    encolar(nx, ny, cola, fin);
                    visitado[nx][ny] = 1;
                    dist[nx][ny] = dist[actual.x][actual.y] + 1;
                    padre[nx][ny] = actual;
                }
            }
        }
    }
    return -1; 
}

// Funcion para generar el laberinto
void crear_laberinto(int fila, int col, int laberinto[fila][col]) {
	int i, j, aux;
	
	for(i = 0; i< fila; i++) {
		for(j = 0; j< col; j++) {
			aux = rand() % 3;
			if(aux>1) {
				aux = 0;
			}
			laberinto[i][j] = aux;
		}
	}
	
	return;
}

int main() {
	
	srand(time(NULL)); 
	
	// Para realizar movimeintos
	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	
	// Crear cola y datos para manejo
	int frente = 0, fin = 0;
	
	int i, j, aux;
	char opcion;
	// Posiciones iniciales
	int fila_inicial= 0;
	int col_inicial = 0;
	
	// Variables para contidad de fila y columnas
	int fila_taman, col_taman;
	
	// Solicitar tamaño del laberinto
	printf("Ingrese el tamaño del laberinto (NUMxNUM): ");
	scanf("%2dx%2d", &fila_taman, &col_taman);
	
	// Definir tamaño del laberinto y Cola
	int laberinto[fila_taman][col_taman];
	Nodo cola[fila_taman * col_taman];
	Nodo padre[fila_taman][col_taman];
	
	int fila_fin = fila_taman - 1;
	int col_fin = col_taman - 1;
	
	// Matriz de visitados y distancias
	int visitado[fila_taman][col_taman];
	int dist[fila_taman][col_taman];
	
	
	for (i = 0; i < fila_taman; i++) {
	    for (j = 0; j < col_taman; j++) {
	        visitado[i][j] = 0;
	        dist[i][j] = -1;
	    }
	}
	
	// crear el laberinto
	crear_laberinto(fila_taman,col_taman, laberinto);
	
	// limpiar entradas
	limpiar_entradas(fila_taman, col_taman, laberinto, fila_inicial, col_inicial);
	
	int pasos;
	pasos = bfs(fila_inicial, col_inicial,
            	fila_fin, col_fin,
            	cola, &frente, &fin,
            	fila_taman, col_taman, padre,
            	laberinto, visitado, dist, dx, dy);
    
    int intentos = 0;
	while(pasos <0 && intentos <3){
		// crear el laberinto
		crear_laberinto(fila_taman,col_taman, laberinto);
		
		// limpiar entradas
		limpiar_entradas(fila_taman, col_taman, laberinto, fila_inicial, col_inicial);
		
		for (i = 0; i < fila_taman; i++) {
		    for (j = 0; j < col_taman; j++) {
		        visitado[i][j] = 0;
		        dist[i][j] = -1;
		    }
		}
		// Crear cola y datos para manejo
		int frente = 0, fin = 0;
		pasos = bfs(fila_inicial, col_inicial,
            	fila_fin, col_fin,
            	cola, &frente, &fin,
            	fila_taman, col_taman, padre,
            	laberinto, visitado, dist, dx, dy);
        
        intentos++;
        printf("Intento %d\n", intentos);
	}
	
	laberinto[fila_inicial][col_inicial] = 3;
	laberinto[fila_fin][col_fin] = 4;
	
	
	/*
	for(i = 0; i< fila_taman; i++) {
		for(j = 0; j< col_taman; j++) {
			printf("%d", laberinto[i][j]);
		}
		printf("\n");
	}*/
	
	imprimirLabResuelto(fila_taman, col_taman, laberinto, fila_inicial, col_inicial);
	
	int ban = 0;
	while(ban == 0){
		printf("¿Desea ver el camino resuelto? (S/N): ");
		scanf(" %c", &opcion);
		if(opcion == 'S' || opcion == 'N')
			break;
	}
	
	if(opcion == 'S'){
		reconstruir_camino(fila_taman, col_taman, fila_fin, col_fin,
						laberinto, fila_inicial, col_inicial, padre);
		imprimirLabResuelto(fila_taman, col_taman, laberinto, fila_inicial, col_inicial);
		printf("Cant. pasos: %d\n", pasos);
	}
	
	
	
	return 0;
}

