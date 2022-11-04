#pragma once

class Mecanicos {
public:

	int claveGen = 0;
	char nombre[MAX_PATH] = "";
	char apPaterno[MAX_PATH] = "";
	char apMaterno[MAX_PATH] = "";
	char telefono[MAX_PATH] = "";
	char especialidad[MAX_PATH] = "";
	char numeroEmpleado[MAX_PATH] = "";
	char turnoTrabajo[MAX_PATH] = "";
	char chFileName[MAX_PATH] = "";
	bool Lunes;
	bool Martes;
	bool Miercoles;
	bool Jueves;
	bool Viernes;

	Mecanicos* izquierda = 0, * derecha = 0, * padre = 0;
};

class Arbol : Mecanicos {
public:
	char archivo[254] = "";
	Mecanicos* raiz = 0;

	Arbol(char*);
	~Arbol();
	void Agregar_Usuario(Mecanicos);
	void Agregar(Mecanicos**, Mecanicos*);
	void Borrar_Usuario(Mecanicos*);
	void Llenar_Lista_Preorden(HWND, UINT, UINT, UINT, Mecanicos*);
	void LlenarNumEmp(HWND, UINT, UINT, UINT, Mecanicos*);
	void AñadirMec(Mecanicos*, HWND, UINT, char[MAX_PATH], UINT, UINT);
	void idSemana(Mecanicos*, char[MAX_PATH], HWND, char[MAX_PATH]);
	void mananaTarde(Mecanicos*, char[MAX_PATH], char[MAX_PATH], HWND, char[MAX_PATH]);
	void ValidarDias(Mecanicos*, char[MAX_PATH], char[MAX_PATH], char[MAX_PATH], bool, bool, bool, bool, bool, HWND);
	void Copiar_Datos(Mecanicos*, Mecanicos);
	void Copiar_Data(Mecanicos*, Mecanicos*);
	void Guardar(Mecanicos*, ofstream*);
	void Reemplazar(Mecanicos*, Mecanicos*);
	Mecanicos* minimo(Mecanicos*);
	Mecanicos* Buscar(char*);
	Mecanicos* BuscarSi(char*);
	Mecanicos* BuscaNodo(Mecanicos*, char*);
	Mecanicos* BuscaNodor(Mecanicos*, char*);
	Mecanicos* Iniciar(char[], char[]);
	Mecanicos* Iniciar_B(char[], char[], Mecanicos*);
};

Arbol::~Arbol() {
	ofstream guardar;
	Mecanicos* temp = 0;
	guardar.open(archivo, ios::binary | ios::trunc);
	if (guardar.is_open())
	{
		if (raiz)
		{
			Guardar(raiz, &guardar);
		}
		guardar.close();
	}
}

void Arbol::Guardar(Mecanicos* nodo, ofstream* guardar) {
	if (nodo)
	{
		guardar->write((char*)nodo, sizeof(Mecanicos));
		Guardar(nodo->izquierda, guardar);
		Guardar(nodo->derecha, guardar);
	}
}

Arbol::Arbol(char* archivo) {
	GetCurrentDirectory(254, this->archivo);
	strcat(this->archivo, archivo);
	Mecanicos temp;
	ifstream leertxt;
	leertxt.open(this->archivo, ios::binary);
	if (leertxt.is_open())
	{
		leertxt.read((char*)&temp, sizeof(Mecanicos));
		while (!leertxt.eof())
		{
			Agregar_Usuario(temp);
			leertxt.read((char*)&temp, sizeof(Mecanicos));
		}
	}
}

void Arbol::Agregar_Usuario(Mecanicos dato) {
	Mecanicos* nuevo = new Mecanicos;

	Copiar_Datos(nuevo, dato);

	if (raiz == 0)
	{
		if (nuevo->claveGen == 0)
			nuevo->claveGen = 3040;
		raiz = nuevo;
	}
	else
	{
		Agregar(&raiz, nuevo);
		if (nuevo->claveGen == 0)
			nuevo->claveGen = nuevo->padre->claveGen + 1;
	}
	
}

void Arbol::Agregar(Mecanicos** node, Mecanicos* dato) {
	Mecanicos* aux = *node;
	//si el auxiliar == 0 significa que no hay raiz, o sea que no hay datos, asi que se agregan
	if (aux == 0)
	{
		*node = dato;
	}
	else
	{
		dato->padre = aux;
		if (strcmp(aux->numeroEmpleado, dato->numeroEmpleado) != 0)
		{
			//si es mayor que cero significa que el hijo(aux) es mayor que el padre(dato)
			if (strcmp(dato->numeroEmpleado, aux->numeroEmpleado) > 0)
				Agregar(&(aux->derecha), dato);
			if (strcmp(dato->numeroEmpleado, aux->numeroEmpleado) < 0)
				Agregar(&(aux->izquierda), dato);
		}
	}
}

//

Mecanicos* Arbol::minimo(Mecanicos* nodo)
{
	if (nodo == 0)
		return NULL;
	if (nodo->izquierda)
		return minimo(nodo->izquierda);
	else
	{
		return nodo;
	}
}

//

void Arbol::Borrar_Usuario(Mecanicos* borrar) {
	Mecanicos* temp;
	if (borrar->izquierda && borrar->derecha)
	{
		temp = minimo(borrar->derecha);
		Copiar_Data(borrar, temp);
		Borrar_Usuario(temp);
	}
	else if (borrar->izquierda)
	{
		temp = borrar->izquierda;
		Copiar_Data(borrar, borrar->izquierda);
		if (borrar->izquierda != NULL) {
			borrar->derecha = borrar->izquierda->derecha;
			borrar->izquierda = borrar->izquierda->izquierda;
		}
		if (borrar->izquierda != NULL)
			borrar->izquierda->padre = borrar;
		if (borrar->derecha != NULL)
			borrar->derecha->padre = borrar;
		delete temp;
		//Reemplazar(borrar, borrar->izquierda);
		//destruirnodo(borrar);
		//delete borrar->izquierda;


	}
	else if (borrar->derecha)
	{
		temp = borrar->derecha;
		Copiar_Data(borrar, borrar->derecha);
		if (borrar->derecha != NULL) {
			borrar->izquierda = borrar->derecha->izquierda;
			borrar->derecha = borrar->derecha->derecha;

		}
		if (borrar->izquierda != NULL)
			borrar->izquierda->padre = borrar;
		if (borrar->derecha != NULL)
			borrar->derecha->padre = borrar;
		delete temp;

		//Reemplazar(borrar, borrar->derecha);
		//destruirnodo(borrar); */
			//delete borrar->derecha;
	}
	else
	{

		if (borrar->padre) {
			if (borrar->padre->izquierda == borrar) {
				borrar->padre->izquierda = 0;
				delete borrar;
			}
			else if (borrar->padre->derecha == borrar) {
				borrar->padre->derecha = 0;
				delete borrar;
			}
		}
		else {
			delete borrar;
			raiz = 0;
		}

		//Reemplazar(borrar, NULL);
		//destruirnodo(borrar);
	}
}

void Arbol::Reemplazar(Mecanicos* borrar, Mecanicos* reemplazar)
{
	if (borrar->padre)//significa que no es raiz
	{
		//if(borrar->padre->izquierda)
		if (borrar->padre->izquierda) {
			if (borrar->claveGen == borrar->padre->izquierda->claveGen)
			{
				//reemplazar->padre = borrar->padre;
				borrar->padre->izquierda = reemplazar;
				Copiar_Data(borrar, reemplazar);
			}
		}
			
		if (borrar->padre->derecha) {
			if (borrar->padre->derecha || borrar->claveGen == borrar->padre->derecha->claveGen) {
				borrar->padre->derecha = reemplazar;
				//reemplazar->padre = borrar->padre;
				Copiar_Data(borrar, reemplazar);
			}
		}
			
	}
	if (reemplazar)
	{
		//reemplazar->padre = 0;
		reemplazar->padre = borrar->padre;
		Copiar_Data(borrar, reemplazar);
	}
}

void Arbol::Copiar_Datos(Mecanicos* nodo, Mecanicos cop) {
	strcpy(nodo->nombre, cop.nombre);
	strcpy(nodo->apPaterno, cop.apPaterno);
	strcpy(nodo->apMaterno, cop.apMaterno);
	strcpy(nodo->telefono, cop.telefono);
	strcpy(nodo->especialidad, cop.especialidad);
	strcpy(nodo->numeroEmpleado, cop.numeroEmpleado);
	strcpy(nodo->turnoTrabajo, cop.turnoTrabajo);
	strcpy(nodo->chFileName, cop.chFileName);
	nodo->Lunes = cop.Lunes;
	nodo->Martes = cop.Martes;
	nodo->Miercoles = cop.Miercoles;
	nodo->Jueves = cop.Jueves;
	nodo->Viernes = cop.Viernes;

}

void Arbol::Copiar_Data(Mecanicos* borrar, Mecanicos* temp)
{
	strcpy(borrar->nombre, temp->nombre);
	strcpy(borrar->apPaterno, temp->apPaterno);
	strcpy(borrar->apMaterno, temp->apMaterno);
	strcpy(borrar->telefono, temp->telefono);
	strcpy(borrar->especialidad, temp->especialidad);
	strcpy(borrar->numeroEmpleado, temp->numeroEmpleado);
	strcpy(borrar->turnoTrabajo, temp->turnoTrabajo);
	strcpy(borrar->chFileName, temp->chFileName);
	borrar->Lunes = temp->Lunes;
	borrar->Martes = temp->Martes;
	borrar->Miercoles = temp->Miercoles;
	borrar->Jueves = temp->Jueves;
	borrar->Viernes = temp->Viernes;
}

void Arbol::Llenar_Lista_Preorden(HWND hWnd, UINT Objeto, UINT Mensaje1, UINT Mensaje2, Mecanicos* r) {
	int cursor = 0;
	char Tagregar[MAX_PATH] = "";
	if (r != NULL && r->claveGen != 0) {
		
		strcpy(Tagregar, r->numeroEmpleado);
		strcat(Tagregar, " - ");
		strcat(Tagregar, r->nombre);
		strcat(Tagregar, " ");
		strcat(Tagregar, r->apPaterno);
		cursor = SendDlgItemMessage(hWnd, Objeto, Mensaje1, 0, (LPARAM)Tagregar);
		SendDlgItemMessage(hWnd, Objeto, Mensaje2, cursor, (LPARAM)r->claveGen);
		Llenar_Lista_Preorden(hWnd, Objeto, Mensaje1, Mensaje2, r->izquierda);
		Llenar_Lista_Preorden(hWnd, Objeto, Mensaje1, Mensaje2, r->derecha);
	}
}

void Arbol::LlenarNumEmp(HWND hWnd, UINT Objeto, UINT Mensaje1, UINT Mensaje2, Mecanicos* r) {
	int cursor = 0;
	char Tagregar[MAX_PATH] = "";
	if (r != NULL && r->claveGen != 0) {

		strcpy(Tagregar, r->numeroEmpleado);
		cursor = SendDlgItemMessage(hWnd, Objeto, Mensaje1, 0, (LPARAM)Tagregar);
		SendDlgItemMessage(hWnd, Objeto, Mensaje2, cursor, (LPARAM)r->claveGen);
		LlenarNumEmp(hWnd, Objeto, Mensaje1, Mensaje2, r->izquierda);
		LlenarNumEmp(hWnd, Objeto, Mensaje1, Mensaje2, r->derecha);
	}

}

void Arbol::AñadirMec(Mecanicos* r, HWND hWnd, UINT Objeto, char esp[MAX_PATH], UINT Mensaje1, UINT Mensaje2) {
	int cursor = 0;
	char Tagregar[MAX_PATH] = "";
	if (r != NULL) {

		if (strcmp(r->especialidad, esp) == 0) {
			strcpy(Tagregar, r->numeroEmpleado);
			strcat(Tagregar, " - ");
			strcat(Tagregar, r->nombre);
			strcat(Tagregar, " ");
			strcat(Tagregar, r->apPaterno);
			cursor = SendDlgItemMessage(hWnd, Objeto, Mensaje1, 0, (LPARAM)Tagregar);
			SendDlgItemMessage(hWnd, Objeto, Mensaje2, cursor, (LPARAM)r->claveGen);
		}
		if(r->izquierda != NULL) AñadirMec(r->izquierda, hWnd, Objeto, esp, Mensaje1, Mensaje2);
		AñadirMec(r->derecha, hWnd, Objeto, esp, Mensaje1, Mensaje2);
	}

}

Mecanicos* Arbol::Buscar(char* buscar)
{
	return BuscaNodo(raiz, buscar);
}

Mecanicos* Arbol::BuscaNodo(Mecanicos* nodo, char* buscar)
{
	if (nodo == 0)
	{
		return 0;
	}
	else
	{
		if (strcmp(buscar, nodo->numeroEmpleado) == 0)
		{
			return nodo;
		}
		else
		{
			if (strcmp(buscar, nodo->numeroEmpleado) > 0)
				return BuscaNodo(nodo->derecha, buscar);
			if (strcmp(buscar, nodo->numeroEmpleado) < 0)
				return BuscaNodo(nodo->izquierda, buscar);
		}
	}
}

Mecanicos* Arbol::BuscarSi(char* buscar)
{
	return BuscaNodor(raiz, buscar);
}

Mecanicos* Arbol::BuscaNodor(Mecanicos* nodo, char* buscar)
{
	if (nodo == 0)
	{
		return 0;
	}
	else
	{
		if (nodo != NULL) {
			if (strcmp(buscar, nodo->numeroEmpleado) == 0)
			{
				return nodo;
			}
			if (nodo->izquierda != NULL) BuscaNodor(nodo->izquierda, buscar);
			BuscaNodor(nodo->derecha, buscar);
		}
	}
}

Mecanicos* Arbol::Iniciar(char user[], char cont[]) {
	return Iniciar_B(user, cont, this->raiz);
}

Mecanicos* Arbol::Iniciar_B(char user[], char cont[], Mecanicos* nodo) {
	if (nodo == 0)
	{
		return 0;
	}
	else
	{
		if (strcmp(user, nodo->numeroEmpleado) == 0 && strcmp(cont, nodo->numeroEmpleado) == 0)
		{
			return nodo;
		}
		else
		{
			if (strcmp(user, nodo->numeroEmpleado) > 0)
				return Iniciar_B(user, cont, nodo->derecha);
			if (strcmp(user, nodo->numeroEmpleado) < 0)
				return Iniciar_B(user, cont, nodo->izquierda);
		}
	}
}

void Arbol::mananaTarde(Mecanicos* r, char turno[MAX_PATH], char mecanico[MAX_PATH], HWND hWnd, char hora[MAX_PATH]) {
	if (r != NULL) {

		if(r->izquierda != NULL) mananaTarde(r->izquierda, turno, mecanico, hWnd, hora);

		if (GL_validar) {
			if (strcmp(mecanico, r->numeroEmpleado) == 0) {

				if (strcmp(hora, "06") == 0 || strcmp(hora, "07") == 0 || strcmp(hora, "08") == 0 || strcmp(hora, "08") == 0 || strcmp(hora, "09") == 0 || strcmp(hora, "10") == 0 || strcmp(hora, "11") == 0 || strcmp(hora, "12") == 0 || strcmp(hora, "13") == 0 || strcmp(hora, "14") == 0) {
					if (strcmp(turno, "Matutino: 6am - 2pm") == 0) {
						GL_validar = true;
					}
					else {
						GL_validar = false;
						MessageBox(hWnd, "No se puede añadir un horario en horario de MAÑANA ya que el Mecanico trabaja en la TARDE ", "HORA y MINUTOS", MB_OK);

					}

				}
				else {
					if (strcmp(turno, "Vespertino: 3pm - 11pm") == 0) {
						GL_validar = true;
					}
					else {
						GL_validar = false;
						MessageBox(hWnd, "No se puede añadir un horario en horario de TARDE ya que el Mecanico trabaja en la MAÑANA ", "HORA y MINUTOS", MB_OK);
					}

				}
			}
			mananaTarde(r->derecha, turno, mecanico, hWnd, hora);
		}
	}
}

void Arbol::idSemana(Mecanicos* r, char cedula[MAX_PATH], HWND hWnd, char semana[MAX_PATH]) {

	if (r != NULL) {
		if(r->izquierda != NULL) idSemana(r->izquierda, cedula, hWnd, semana);
		if (strcmp(cedula, r->numeroEmpleado) == 0) {
			if (strcmp(semana, "lunes") == 0) {
				if (r->Lunes)
					GL_validar = true;
				else GL_validar = false;
			}
			else if (strcmp(semana, "martes") == 0) {
				if (r->Martes)
					GL_validar = true;
				else GL_validar = false;
			}
			else if (strcmp(semana, "miércoles") == 0) {
				if (r->Miercoles)
					GL_validar = true;
				else GL_validar = false;
			}
			else if (strcmp(semana, "jueves") == 0) {
				if (r->Jueves)
					GL_validar = true;
				else GL_validar = false;
			}
			else if (strcmp(semana, "viernes") == 0) {
				if (r->Viernes)
					GL_validar = true;
				else GL_validar = false;
			}
			else if (strcmp(semana, "sábado") == 0) {
				GL_validar = false;

			}
			else if (strcmp(semana, "domingo") == 0) {
				GL_validar = false;
			}
			if (GL_validar == false) {
				MessageBox(hWnd, "No se puede añadir mecanico en un dia el cual no trabaja el mecanico", "Mecanicos", MB_OK);
				GL_validar = false;
			}
		}
		idSemana(r->derecha, cedula, hWnd, semana);
	}
}

void Arbol::ValidarDias(Mecanicos* r, char turnoD[MAX_PATH], char espeP[MAX_PATH], char numE[MAX_PATH], bool ckLunes, bool ckMartes, bool ckMiercoles, bool ckJueves, bool ckViernes, HWND hWnd) {

	if (r != NULL) {
		if(r->izquierda != NULL) ValidarDias(r->izquierda, turnoD, espeP, numE, ckLunes, ckMartes, ckMiercoles, ckJueves, ckViernes, hWnd);
		if (strcmp(r->turnoTrabajo, turnoD) == 0) {
			if (strcmp(r->especialidad, espeP) == 0) {

				if (ckLunes == r->Lunes && ckLunes == true && strcmp(numE, r->numeroEmpleado) != 0) {
					MessageBox(hWnd, "No se puede añadir Mecanico en LUNES en el mismo horario con la misma especialidad", "Mecanico", MB_OK);
					GL_validar = false;
				}
				else if (ckMartes == r->Martes && ckMartes == true && strcmp(numE, r->numeroEmpleado) != 0) {
					MessageBox(hWnd, "No se puede añadir Mecanico en MARTES en el mismo horario con la misma especialidad", "Mecanico", MB_OK);
					GL_validar = false;
				}
				else if (ckMiercoles == r->Miercoles && ckMiercoles == true && strcmp(numE, r->numeroEmpleado) != 0) {
					MessageBox(hWnd, "No se puede añadir Mecanico en MIERCOLES en el mismo horario con la misma especialidad", "Mecanico", MB_OK);
					GL_validar = false;
				}
				else if (ckJueves == r->Jueves && ckJueves == true && strcmp(numE, r->numeroEmpleado) != 0) {
					MessageBox(hWnd, "No se puede añadir Mecanico en JUEVES en el mismo horario con la misma especialidad", "Mecanico", MB_OK);
					GL_validar = false;
				}
				else if (ckViernes == r->Viernes && ckViernes == true && strcmp(numE, r->numeroEmpleado) != 0) {
					MessageBox(hWnd, "No se puede añadir Mecanico en VIERNES en el mismo horario con la misma especialidad", "Mecanico", MB_OK);
					GL_validar = false;
				}
			}
		}
	
		if (GL_validar) {
			GL_validar = true;
		}
		ValidarDias(r->derecha, turnoD, espeP, numE, ckLunes, ckMartes, ckMiercoles, ckJueves, ckViernes, hWnd);

	}
}
