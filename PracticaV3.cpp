//Jose Luis Moreno y Desiree Ventencourt Rosas (1B)
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

typedef enum { nadie, automata, persona } tJugador;

const int META = 31;

//Funciones preestablecidas
tJugador pasaCalculadora();
tJugador quienEmpieza();
bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);
int digitoAleatorio(int intervalo, int inicial); // genera un digito aleatorio;
int digitoAutomata(int ultimo); // devuelve un digito valido;
int digitoPersona(int ultimo); //pide un digito al usuario;

//Funciones adicionales
int menu();
bool actInforme(int nUtilizado, int nJugadas, int nGanadas, int nAbandonos);
bool mostrar(string nombArch);
//Funciones extra
int primerTurno(bool turnoA);
bool empezarPartida();
bool cambioTurno(bool turnoA);

int main(){
	string nombre, nombArch;
	int op, nUtilizado = 0, nJugadas = 0, nGanadas = 0, nAbandonos = 0, ganador;
	bool ok = false, fin = false, creado = false,abrir = false;
	srand((unsigned int)time(NULL));
	ifstream archivo;
	archivo.open("informePC.txt");
	if(!archivo.is_open()){ // Esto se puede implementar con una funcion con referencia 
		cout << "El archivo no se ha encontrado. El archivo se creara al finalizar el programa" << endl;
	}else{
		archivo >> nUtilizado;
		archivo >> nJugadas;
		archivo >> nGanadas;
		archivo >> nAbandonos;
	}
	archivo.close();
	cout << "Bienbenido a Pasa la Calculadora!" << endl;
	cout << "¿Como te llamas?: ";
	getline(cin, nombre);
	cin.sync();
	cout << "Hola " << nombre << endl;
	nUtilizado++;
	cout << "Selecciona una opcion" << endl;
	op = menu();
	while (op != 0){
		switch (op){
		case 1:
			nJugadas++;
			ganador = pasaCalculadora();
			if (ganador == persona) {
				cout << "Has Ganado" << endl;
				nGanadas++;
			}
			if (ganador == automata){
				cout << "Has Perdido" << endl;
			}
			if (ganador == nadie) nAbandonos++;
			break;
		case 2:
			ok = mostrar("versionPC.txt");
			if(!ok) cout << "Error al abrir el archivo versionCB.txt" << endl;
			break;
		}//Switch
		op = menu();
	}//while.
	creado = actInforme(nUtilizado, nJugadas, nGanadas, nAbandonos);
	return 0;
}

int primerTurno(bool turnoA){
	int numero;
	if (turnoA){
		cout << "Empiezo yo" << endl;
		numero = digitoAleatorio(9, 1);
		cout << "Yo pulso " << numero << endl;
	}
	else{
		cout << "Empiezas tu" << endl;
		cout <<setw(20) <<right << "7     8     9" << endl;// Para que muestre el tablero se puede pasar tambien como 
		cout <<setw(20) <<right << "4     5     6" << endl;// un void, para no repetir tantos cout
		cout <<setw(20) <<right << "1     2     3" << endl;
		cout << "Introduce tu digito (0 para abandonar): ";
		cin >> numero;
	}
	return numero;
}

bool empezarPartida(){
	bool comp = false;
	if (quienEmpieza() == 1) comp = true;
	return comp;
}

bool cambioTurno(bool turnoA){
	bool comp = false;
	if (turnoA) comp = false;
	else comp = true;
	return comp;
}

int digitoPersona(int ultimo){
	int nuevo;
	cout <<setw(20) <<right << "7     8     9" << endl;
	cout <<setw(20) <<right << "4     5     6" << endl;
	cout <<setw(20) <<right << "1     2     3" << endl;
	cout << "Introduce tu digito (0 para abandonar): ";
	cin >> nuevo;
	if (nuevo == 0){
		cout << "Volviendo al menu principal!..." << endl;
	}else{
		while (!digitoValido(ultimo, nuevo)){
			cout << "ERROR : tiene que ser distinto de " << ultimo <<" y estar en la misma fila o columna " << endl;
			cout <<setw(20) <<right << "7     8     9" << endl;
			cout <<setw(20) <<right << "4     5     6" << endl;
			cout <<setw(20) <<right << "1     2     3" << endl;
			cout << "Introduce tu digito (0 para abandonar): ";
			cin >> nuevo;
		}
	}
	return nuevo;
}

int digitoAutomata(int ultimo){
	int nuevo;
	nuevo = digitoAleatorio(9, 1);
	while (!digitoValido(ultimo, nuevo)){
		nuevo = digitoAleatorio(9, 1);
	}
	cout << "Yo pulso " << nuevo << endl;
	return nuevo;
}

tJugador pasaCalculadora(){
	tJugador jugador;
	int numMaquina = 0, numJugador = 0, suma, ultimo = 0;
	bool fin = false, turnoA = empezarPartida(); // Si turnoA = false, no empieza la maquina; en el otro caso si
	ultimo = primerTurno(turnoA); // Realizamos el primer turno, en el que se pasa a la funcion quien empieza
	turnoA = cambioTurno(turnoA); // Invertimos el orden del booleano, para pasar el turno al otro jugador
	suma = ultimo;
	do{
		if (ultimo != 0){ // Si el numero es distinto de 0, seguimos jugando
			cout << "Suma: " << suma << endl;
			if (!turnoA){ //turno Jugador;
				numJugador = digitoPersona(ultimo); // Devuelve un digito valido introducido por el Jugador o '0' para abandonar
				if (numJugador == 0){
					fin = true;
					jugador = nadie;
				}else{
					turnoA = true; // Devolvemos el turno a la Maquina
					ultimo = numJugador;
				}
			}else{//turno de la maquina;
				numMaquina = digitoAutomata(ultimo); // Genera un numero aleatorio continuamente hasta que el numero sea valido
				ultimo = numMaquina;
				turnoA = false; // Devolvemos el turno al Jugador
			}
			suma = suma + ultimo;
			if ((suma >= META)&&(ultimo != 0)){
				fin = true;
				if (!turnoA){ // Le toca al Jugador, pero la suma ya es igual o superior a 31. Gana el Jugador
					jugador = persona;
				}else{
					jugador = automata; // Le toca a la Maquina, pero la suma ya es igual o superior a 31. Gana la Maquina
				} // else
			} // if(suma>=META)
		} // if (ultimo != 0)
	} while (!fin);
	return jugador;
}

int digitoAleatorio(int intervalo, int inicial){
	int numero;
	if (inicial == 1){
		numero = (rand() % intervalo) + 1; //Valor entre el 1 e "intervalo"
	}else{
		numero = rand() % (intervalo + 1); //Valor entre el 0 e "intervalo"
	}
	return numero;
}

bool digitoValido(int ultimo, int nuevo){
	bool comp = false;
	if (((mismaColumna(ultimo, nuevo)) || (mismaFila(ultimo, nuevo)))&&(ultimo!=nuevo)) comp = true;
	return comp;
}

bool mismaFila(int ultimo, int nuevo){
	bool ok = false;
	int filaUltimo, filaNueva;
	filaUltimo = (ultimo - 1) / 3;
	filaNueva = (nuevo - 1) / 3;
	if (filaNueva == filaUltimo) ok = true;
	return ok;
}

bool mismaColumna(int ultimo, int nuevo){
	bool ok = false;
	int columnaUltimo, columnaNueva;
	columnaUltimo = (ultimo - 1) % 3;
	columnaNueva = (nuevo - 1) % 3;
	if (columnaNueva == columnaUltimo) ok = true;
	return ok;
}

tJugador quienEmpieza(){
	tJugador jugador;
	int num;
	num = digitoAleatorio(2, 1);
	jugador = tJugador(num);
	return jugador;
}

bool actInforme(int nUtilizado, int nJugadas, int nGanadas, int nAbandonos){ // Hemos metido nUtilizados, ya que se puede abrir el 
	bool ok = false;																													// para solo ver la opcion 2 'Acerca de Pasa la Calculadora'
	ofstream archivo;																												 // por lo que el programa se ha utilizado.
	ifstream leeArchivo;
	leeArchivo.open("informePC.txt"); // Lee primero el archivo para saber si existe
	if (leeArchivo.is_open()){// Devuelve true si el archivo existe
		ok = true;
		leeArchivo.close();
		archivo.open("informePC.txt"); // Se sobreescribe el archivo
		archivo << nUtilizado << endl;
		archivo << nJugadas << endl;
		archivo << nGanadas << endl;
		archivo << nAbandonos << endl;
		archivo << "XXX" << endl;
		archivo.close();
	}else{// Si el archivo no esta se crea y devuelve false
		archivo.open("informePC.txt");
		archivo << nUtilizado << endl;
		archivo << nJugadas << endl;
		archivo << nGanadas << endl;
		archivo << nAbandonos << endl;
		archivo << "XXX" << endl;
		archivo.close();
	}
	return ok;
}

int menu(){
	int op;
	do{
		cout << endl;
		cout << "1 - Jugar" << endl;
		cout << "2 - Acerca de " << endl;
		cout << "0 - Salir" << endl;
		cout << endl;
		cout << "Opcion: ";
		cin >> op;
		if ((op < 0) || (op > 5)) {
			cout << "Opcion no valida!" << endl;
		}
	} while ((op < 0) || (op > 5));
	return op;
}

bool mostrar(string nombArch){
	ifstream archivo;
	bool ok = false;
	archivo.open(nombArch);
	if (archivo.is_open()){
		ok = true;
		getline(archivo, nombArch);
		while (nombArch != "XXX"){ // Utiliza centinela
			cout << nombArch << endl;
			getline(archivo, nombArch);//lee el siguiente
		}
		cout << endl;
		archivo.close();
	}
	return ok;
}