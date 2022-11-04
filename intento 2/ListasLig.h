#pragma once

class Usuario {
public:

	char nombreUsuario[MAX_PATH];
	char usuario[MAX_PATH];
	char password[MAX_PATH];
	char DireccionTxt[MAX_PATH] = "";

	Usuario();

} login;

Usuario :: Usuario()  {

	ifstream agregarUsuario;
	agregarUsuario.open("Usuario.txt", ios::in);
	if (!agregarUsuario.is_open())
	{
		MessageBox(0, "ERROR AL CARGAR USUARIO", "ERROR", MB_ICONERROR);
		return;
	}
	while (!agregarUsuario.eof()) {
		agregarUsuario >> login.nombreUsuario;
		agregarUsuario >> login.usuario;
		agregarUsuario >> login.password;
	}

	agregarUsuario.close();
}

class Cliente {
public:

	int claveGen = 0;
	char nombre[MAX_PATH] = "";
	char apellidoCl[MAX_PATH] = "";
	char apellidoCl2[MAX_PATH] = "";
	char telCl[MAX_PATH] = "";
	char claveP[MAX_PATH] = "";
	char marca[MAX_PATH] = "";
	char modelo[MAX_PATH] = "";
	char anioVehiculo[MAX_PATH] = "";
	char kilometraje[MAX_PATH] = "";
	char direccion[MAX_PATH] = "";

};

class Paquetes {
public:

	int claveGen = 0;
	char claveP[MAX_PATH] = "";
	char nombre[MAX_PATH] = "";
	char precioP[MAX_PATH] = "";
	char descripcion[MAX_PATH] = "";

	//bool mecPaquete = false;
};

class Citas {
public:

	int claveGen = 0;
	char nombre[MAX_PATH] = "";
	char claveMec[MAX_PATH] = "";
	char nomMecanico[MAX_PATH] = "";
	char especialidadCita[MAX_PATH] = "";
	char claveP[MAX_PATH] = "";
	char turnoTC[MAX_PATH] = "";
	char celularCliente[MAX_PATH] = "";
	char imagenMec[MAX_PATH] = "";
	char fechaCita[MAX_PATH] = "";
	char anioCita[MAX_PATH] = "";
	char mesCita[MAX_PATH] = "";
	char semanaCita[MAX_PATH] = "";
	char diaCita[MAX_PATH] = "";
	char horaCita[MAX_PATH] = "";
	char minutoCita[MAX_PATH] = "";
	char cancelarCita[MAX_PATH] = "";
	bool LunesC;
	bool MartesC;
	bool MiercolesC;
	bool JuevesC;
	bool ViernesC;

};

//un template se la pongo a la funcion xq es una plantilla, xq mi clase es de tipo templeit
//manejo de informacion de cualquier lista ligada
template < class ClaseDinamica > class nodo {
public:
	//tipo de variable de clase que puede ser cualquier clase
	ClaseDinamica datos;

	nodo *sig = 0, *ant = 0;

	//solo pa copiar datos
	void copiardatos(ClaseDinamica datosCopiar);

	nodo(ClaseDinamica datos) {

		//this. puntero para hacer referencia a las variables de la clase
		this->datos = datos;
	}

	~nodo() {

	}
};

 //standard de mi clase xq es un template que pertenece a mi class nodo       //los :: indican que funcion es
template < class ClaseDinamica > void nodo < ClaseDinamica > :: copiardatos( ClaseDinamica datosCopiar) {

	//this: puntero que apunta a las variables de mi clase
	this->datos = datosCopiar;
}

//manejar lista ligada de cualquier clase
template < class NododeCualquierClase, class DatosdelNodoCualquierClase > class ListaLigada {
public:
	
	//direccioncarpeta es donde guardare la direccion de carpeta del archivo
	char DireccionCarpeta[MAX_PATH] = "";
	//punteros origen y  ultimo generalizados
	//ultimo sirve para generar nodos
	NododeCualquierClase * primero, * ultimo;

	NododeCualquierClase* base = 0;

	//cabecera, lo que se defina en el char lo va a tomar como nombre, porque no tiene un nombre, solo va a tener una variable tipo puntero char
	//char* va a recibir el nombre de los archivos
	ListaLigada( char* );
	~ListaLigada();

	void eliminarNodos(NododeCualquierClase*);
	void agregarNodos(DatosdelNodoCualquierClase);
	void llenarCitas(HWND, UINT, UINT, UINT);
	void llenarLByCB(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2);
	void llenarCBP(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2);
	void validarCita(HWND, char[MAX_PATH], char[MAX_PATH], char[MAX_PATH], char[MAX_PATH], char[MAX_PATH]);
	void semanaMes(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2);
	void semanaMesEspecialidad(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2);
	void mecanicoMes(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2);
	void guardarEnTxt(HWND hWnd, char[MAX_PATH], bool);
	//void validarSemanaMes(HWND, char [MAX_PATH], char [MAX_PATH], char[MAX_PATH]);
	//se usa NododeCualquierClase* para obtener la direccion de lo obtenido en *buscar, que es el nodo que busque
	//se puede usar para asignarlo a otros punteros para manejar esa misma direccion de memoria
	NododeCualquierClase* buscar(int);
	NododeCualquierClase* validarDias(char*);
	NododeCualquierClase* buscarValidar(NododeCualquierClase*, char*);
};

template < class NododeCualquierClase, class DatosdelNodoCualquierClase > void ListaLigada < NododeCualquierClase, DatosdelNodoCualquierClase > :: agregarNodos(DatosdelNodoCualquierClase datoAgregar) {

	NododeCualquierClase* aux = 0;
	aux = new NododeCualquierClase(datoAgregar);

	//para evitar errores(llegaba con basura)
	if (aux->datos.claveGen != 0) {
		aux->datos.claveGen = datoAgregar.claveGen;
	}

	if (primero == 0) {
		primero = aux;
		ultimo = aux;

		//cuando le llega un nodo nuevo que no tiene clave
		if (aux->datos.claveGen == 0)
			aux->datos.claveGen = 3030;
	}
	else {

		if (aux->datos.claveGen == 0) {
			aux->datos.claveGen = ultimo->datos.claveGen + 1;
		}

		//lo enlaza
		ultimo->sig = aux;
		aux->ant = ultimo;
		ultimo = aux;
	}
}

template < class NododeCualquierClase, class DatosdelNodoCualquierClase > void ListaLigada < NododeCualquierClase, DatosdelNodoCualquierClase > :: eliminarNodos(NododeCualquierClase* nodoBorrar) {

	if (this->primero == nodoBorrar && this->ultimo == nodoBorrar) {
		this->primero = this->ultimo = NULL;
	}
	else if (this->primero == nodoBorrar) {
		this->primero = nodoBorrar->sig;
		this->primero->ant = NULL;
	}
	else if (this->ultimo == nodoBorrar) {
		this->ultimo = this->ultimo->ant;
		this->ultimo->sig = NULL;
	}
	else {
		nodoBorrar->ant->sig = nodoBorrar->sig;
		nodoBorrar->sig->ant = nodoBorrar->ant;

	}

	delete nodoBorrar;

}

template < class NododeCualquierClase, class DatosdelNodoCualquierClase > ListaLigada < NododeCualquierClase, DatosdelNodoCualquierClase > :: ListaLigada(char * nombreDelArchivo) {

	//obtiene la direccion de la carpeta del programa ejecutado
	GetCurrentDirectory(MAX_PATH, this->DireccionCarpeta);
	//voy a concatenar cadena de caracteres
	strcat(this->DireccionCarpeta, nombreDelArchivo);
	//direccion de donde se ejecuta  ,  //nombre del archivo en concreto, mando el txt

	//DatosdelNodoCualquierClase se usa para los datos de las listas ligadas
	DatosdelNodoCualquierClase dato;

	//cualquierclase se usa para el nodo

	ifstream archivo;
	archivo.open(this->DireccionCarpeta, ios::binary);
	if (archivo.is_open()){  //DatosdelNodoCualquierClase sera la suma de las variables de lo que esta adentro de la clase
		//este primer read significa que por lo menos tiene un(1) nodo
		archivo.read((char*)&dato, sizeof(DatosdelNodoCualquierClase));

		while (!archivo.eof()) {

			agregarNodos(dato);
			//este segundo read es para leer un nuevo nodo
			archivo.read((char*)&dato, sizeof(DatosdelNodoCualquierClase));
		}

		archivo.close();
	}
}

template <class NododeCualquierClase, class DatosdelNodoCualquierClase > ListaLigada < NododeCualquierClase, DatosdelNodoCualquierClase > :: ~ListaLigada() {
	
	NododeCualquierClase* borrar = NULL, * auxi = NULL;
	ofstream guardarArchivo;
	guardarArchivo.open(this->DireccionCarpeta, ios::binary | ios::trunc);
	if (guardarArchivo.is_open())
	{
		auxi = primero;
		while (auxi != 0)
		{
			guardarArchivo.write((char*)&auxi->datos, sizeof(DatosdelNodoCualquierClase));
			borrar = auxi;
			auxi = auxi->sig;
			delete borrar;
		}
		guardarArchivo.close();
	}
}

template <class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada <NododeCualquierClase, DatosdelNodoCualquierClase>:: llenarLByCB (HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {
	
	char bufer[MAX_PATH] = "", numero[10] = "";
	NododeCualquierClase* aux = 0;
	int a = 0;
	aux = this->primero;
	while (aux != 0)
	{
		strcpy(bufer, aux->datos.nombre);
		strcat(bufer, " - ");
		strcat(bufer, aux->datos.claveP);
		a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
		SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);
		aux = aux->sig;
	}
}

template <class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada <NododeCualquierClase, DatosdelNodoCualquierClase>::llenarCBP(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {

	char bufer[MAX_PATH] = "", numero[10] = "";
	NododeCualquierClase* aux = 0;
	int a = 0;
	aux = this->primero;
	while (aux != 0)
	{
		strcpy(bufer, aux->datos.claveP);
		a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
		SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);
		aux = aux->sig;
	}
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase> NododeCualquierClase* ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase> ::buscar(int busca) {
	NododeCualquierClase* aux = 0;
	aux = this->primero;
	while (aux != 0) {
		if (busca == aux->datos.claveGen) {
			//me regresa aux si sí encontro la clave
			return aux;
		}
		aux = aux->sig;
	}
	//si me returna null significa que no encontro la clave buscada
	return NULL;
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase> NododeCualquierClase* ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase> ::validarDias(char* buscar)
{
	return buscarValidar(base, buscar);
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase> NododeCualquierClase* ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase> ::buscarValidar(NododeCualquierClase* nodoBuscar, char* buscaClave) {

	nodoBuscar = 0;

	nodoBuscar = this->primero;
	while (nodoBuscar != 0) {
		if (strcmp(buscaClave, nodoBuscar->datos.claveP) == 0) {
			//me regresa aux si sí encontro la clave
			return nodoBuscar;
		}
		nodoBuscar = nodoBuscar->sig;
	}
	//si me returna null significa que no encontro la clave buscada
	return NULL;
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::llenarCitas(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {
	NododeCualquierClase* aux = 0;
	char bufer[MAX_PATH] = "";
	int a = 0;
	aux = this->primero;
	while (aux != 0)
	{
		strcpy(bufer, aux->datos.clienteCita);
		strcat(bufer, " - ");
		strcat(bufer, aux->datos.mecanicoCita);
		strcat(bufer, " - ");

		a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
		SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);

		aux = aux->sig;
	}
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::validarCita(HWND hWnd, char esp[MAX_PATH], char hora[MAX_PATH], char min[MAX_PATH], char mes[MAX_PATH], char dia[MAX_PATH]) {

	NododeCualquierClase* aux = 0;

	if (GL_validar) {
		aux = this->primero;
		if (aux) {

			while (aux) {

				if (strcmp(mes, aux->datos.mesCita) == 0) {
					if (strcmp(dia, aux->datos.diaCita) == 0) {
						if (strcmp(esp, aux->datos.especialidadCita) == 0) {
							if (strcmp(hora, aux->datos.horaCita) == 0) {
								if (strcmp(min, aux->datos.minutoCita) == 0) {
									MessageBox(hWnd, "No se puede añadir un horario en horario de otra cita ", "HORA y MINUTOS", MB_OK);
									GL_validar = false;
									return;
								}
								else {
									GL_validar = true;
									aux = aux->sig;
								}
							}

							else {
								GL_validar = true;
								aux = aux->sig;
							}
						}
						else {
							GL_validar = true;
							aux = aux->sig;
						}
					}
					else {
						GL_validar = true;
						aux = aux->sig;
					}
				}
				else {
					GL_validar = true;
					aux = aux->sig;
				}
			}
		}
	}
	

}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::semanaMes(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {
	
	NododeCualquierClase* aux = primero;
	int a = 0;
	char bufer[MAX_PATH];
	while (aux != 0)
	{
		if (strcmp(aux->datos.mesCita, mesesito) == 0) {
			if (strcmp(aux->datos.diaCita, "1") == 0 || strcmp(aux->datos.diaCita, "2") == 0 || strcmp(aux->datos.diaCita, "3") == 0 || strcmp(aux->datos.diaCita, "4") == 0 || strcmp(aux->datos.diaCita, "5") == 0 || strcmp(aux->datos.diaCita, "6") == 0 || strcmp(aux->datos.diaCita, "7") == 0) {
				strcpy(semanA, "1");
			}
			else if (strcmp(aux->datos.diaCita, "8") == 0 || strcmp(aux->datos.diaCita, "9") == 0 || strcmp(aux->datos.diaCita, "10") == 0 || strcmp(aux->datos.diaCita, "11") == 0 || strcmp(aux->datos.diaCita, "12") == 0 || strcmp(aux->datos.diaCita, "13") == 0 || strcmp(aux->datos.diaCita, "14") == 0) {
				strcpy(semanA, "2");
			}
			else if (strcmp(aux->datos.diaCita, "15") == 0 || strcmp(aux->datos.diaCita, "16") == 0 || strcmp(aux->datos.diaCita, "17") == 0 || strcmp(aux->datos.diaCita, "18") == 0 || strcmp(aux->datos.diaCita, "19") == 0 || strcmp(aux->datos.diaCita, "20") == 0 || strcmp(aux->datos.diaCita, "21") == 0) {
				strcpy(semanA, "3");
			}
			else if (strcmp(aux->datos.diaCita, "22") == 0 || strcmp(aux->datos.diaCita, "23") == 0 || strcmp(aux->datos.diaCita, "24") == 0 || strcmp(aux->datos.diaCita, "25") == 0 || strcmp(aux->datos.diaCita, "26") == 0 || strcmp(aux->datos.diaCita, "27") == 0 || strcmp(aux->datos.diaCita, "28") == 0) {
				strcpy(semanA, "4");
			}
			else if (strcmp(aux->datos.diaCita, "29") == 0 || strcmp(aux->datos.diaCita, "30") == 0 || strcmp(aux->datos.diaCita, "31") == 0) {
				strcpy(semanA, "5");
			}
			if (strcmp(semanA, semanita) == 0) {

				strcpy(bufer, aux->datos.claveP);
				//SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_ADDSTRING, 0, (LPARAM)aux->datos.claveP);
				a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
				SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);
			}
		}
		aux = aux->sig;
	}
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::semanaMesEspecialidad(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {

	NododeCualquierClase* aux = primero;
	int a = 0;
	char bufer[MAX_PATH];
	while (aux != 0)
	{
		if (strcmp(aux->datos.mesCita, mesesito) == 0) {
			if (strcmp(aux->datos.especialidadCita, especialidadcita) == 0) {
				if (strcmp(aux->datos.diaCita, "1") == 0 || strcmp(aux->datos.diaCita, "2") == 0 || strcmp(aux->datos.diaCita, "3") == 0 || strcmp(aux->datos.diaCita, "4") == 0 || strcmp(aux->datos.diaCita, "5") == 0 || strcmp(aux->datos.diaCita, "6") == 0 || strcmp(aux->datos.diaCita, "7") == 0) {
					strcpy(semanA, "1");
				}
				else if (strcmp(aux->datos.diaCita, "8") == 0 || strcmp(aux->datos.diaCita, "9") == 0 || strcmp(aux->datos.diaCita, "10") == 0 || strcmp(aux->datos.diaCita, "11") == 0 || strcmp(aux->datos.diaCita, "12") == 0 || strcmp(aux->datos.diaCita, "13") == 0 || strcmp(aux->datos.diaCita, "14") == 0) {
					strcpy(semanA, "2");
				}
				else if (strcmp(aux->datos.diaCita, "15") == 0 || strcmp(aux->datos.diaCita, "16") == 0 || strcmp(aux->datos.diaCita, "17") == 0 || strcmp(aux->datos.diaCita, "18") == 0 || strcmp(aux->datos.diaCita, "19") == 0 || strcmp(aux->datos.diaCita, "20") == 0 || strcmp(aux->datos.diaCita, "21") == 0) {
					strcpy(semanA, "3");
				}
				else if (strcmp(aux->datos.diaCita, "22") == 0 || strcmp(aux->datos.diaCita, "23") == 0 || strcmp(aux->datos.diaCita, "24") == 0 || strcmp(aux->datos.diaCita, "25") == 0 || strcmp(aux->datos.diaCita, "26") == 0 || strcmp(aux->datos.diaCita, "27") == 0 || strcmp(aux->datos.diaCita, "28") == 0) {
					strcpy(semanA, "4");
				}
				else if (strcmp(aux->datos.diaCita, "29") == 0 || strcmp(aux->datos.diaCita, "30") == 0 || strcmp(aux->datos.diaCita, "31") == 0) {
					strcpy(semanA, "5");
				}
				if (strcmp(semanA, semanita) == 0) {

					strcpy(bufer, aux->datos.claveP);
					//SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_ADDSTRING, 0, (LPARAM)aux->datos.claveP);
					a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
					SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);
				}
			}
		}
		aux = aux->sig;
	}
}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::mecanicoMes(HWND hWnd, UINT ID_Objeto, UINT Tipo_Mensaje, UINT Tipo_Mensaje2) {

	NododeCualquierClase* aux = primero;
	int a = 0;
	char bufer[MAX_PATH];
	while (aux != 0)
	{
		if (strcmp(aux->datos.mesCita, mesesito) == 0) {
			if (strcmp(aux->datos.claveMec, numMecBin) == 0) {

				strcpy(bufer, aux->datos.claveP);
				//SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_ADDSTRING, 0, (LPARAM)aux->datos.claveP);
				a = SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje, 0, (LPARAM)bufer);
				SendDlgItemMessage(hWnd, ID_Objeto, Tipo_Mensaje2, a, aux->datos.claveGen);
			
			}
		}
		aux = aux->sig;
	}

}

template<class NododeCualquierClase, class DatosdelNodoCualquierClase>void ListaLigada<NododeCualquierClase, DatosdelNodoCualquierClase>::guardarEnTxt(HWND hWnd, char claveG[MAX_PATH], bool SiNo) {

	NododeCualquierClase* lista = primero;
	char txt[MAX_PATH] = "";
	GetCurrentDirectory(MAX_PATH, txt);
	strcat(txt, "\\CitasReporte.txt");
	ofstream archivo;
	archivo.open(txt);

	while (lista != NULL) {

		if (strcmp(lista->datos.claveP, claveG) == 0) {
			if (archivo.is_open())
			{
				char nombre[MAX_PATH] = "";
				char demas[MAX_PATH] = "";
				char masDemas[MAX_PATH] = "";

				strcpy(nombre, lista->datos.nombre);
				strcat(nombre, " ");
				strcat(nombre, lista->datos.nomMecanico);
				strcat(nombre, " ");
				strcat(nombre, lista->datos.especialidadCita);
				strcat(nombre, " ");
				strcat(nombre, lista->datos.claveP);
				strcat(nombre, " ");
				strcat(demas, lista->datos.turnoTC);
				strcat(demas, " ");
				strcat(demas, lista->datos.celularCliente);
				strcat(demas, " ");
				strcat(demas, lista->datos.imagenMec);
				strcat(demas, " ");
				strcat(demas, lista->datos.fechaCita);
				strcat(demas, " ");
				strcat(demas, lista->datos.horaCita);
				strcat(demas, " : ");
				strcat(demas, lista->datos.minutoCita);
				strcat(demas, " ");
				if (SiNo == 1) {
				strcat(masDemas, "Cancelado?");
				strcat(masDemas, ": ");
				strcat(masDemas, "Si");
				strcat(masDemas, " ");
				strcat(masDemas, "Razon:");
				strcat(masDemas, " ");
				strcat(masDemas, lista->datos.cancelarCita);
				}
				
				if (SiNo == 1) archivo << nombre << demas << masDemas << "\n";
				else archivo << nombre << demas << "\n";

				archivo.close();
			}
		}
		lista = lista->sig;
	}
	
}
