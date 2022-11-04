#include <windows.h> 
#include "resource.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <wctype.h>
#include <fstream>
#pragma warning(disable:4996)

using namespace std;

char GL_txtEsp[MAX_PATH] = "\\Especialidad.txt";
char GL_txtCl[MAX_PATH] = "\\Cliente.txt";
char GL_txtMeca[MAX_PATH] = "\\Mecanico.txt";
char GL_txtCitas[MAX_PATH] = "\\Citas.txt";
char mesesito[MAX_PATH] = "", semanita[MAX_PATH] = "", semanA[MAX_PATH] = "", especialidadcita[MAX_PATH] = "";
char GL_turnoTrabajo[MAX_PATH], GL_CBesp[MAX_PATH], horaC[MAX_PATH] = "", fechaC[MAX_PATH] = "", busqBin[MAX_PATH] = "", numMecBin[MAX_PATH] = "";
int  i, GL_cant, GL_contPunto, numeCita = 3030, buscar = 0;
bool GL_validar = false, validarNumEmp = false, valDiasEmp = false, valCitas = false, valDobleCita = false, valDCita = false, validarSem = false;
string dia, mes, año;

const char* Turno[] = { "Matutino: 6am - 2pm",
						"Vespertino: 3pm - 11pm",
};

#include "ListasLig.h"
#include "Arbolito.h"

HWND cbTurnoDeTrabajo;
HWND cbEspecialidad;
HWND hVentanaPrincipal;
HWND ed_Precio;
static HBITMAP hLogo;
static nodo <Citas>* auxiliarRep = NULL;
Mecanicos* ordenar, * guardarPos;
HBITMAP bmp;
HINSTANCE hGlobalInst;
void OrdenarHeap(HWND);
void agregarOrdenar(Mecanicos*);
void quickSort(Mecanicos*, Mecanicos*);
Mecanicos* partition(Mecanicos*, Mecanicos*);
void swap2(Mecanicos*, Mecanicos*);
Mecanicos* middle(Mecanicos*, Mecanicos*);
Mecanicos* binarySearch(char[MAX_PATH]);
//void metodoBurbuja(Mecanicos*);
void heapSort(Cliente*, int);
void heapify(Cliente*, int, int);

BOOL CALLBACK IniciarSesion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VentanaPrincipal(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroMecanicos(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroPaquetes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CitasxMecanico(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CitasxNumero(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CitasxPaquete(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CitasxSemana(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Clientes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RegistroCitas(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReporteCitas(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReporteClientes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReporteMecanicos(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Salida(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//nodo trabaja con datos, listaligada tambien trabaja con datos
//crear x listas ligadas con una sola clase
//GL_XXXX es una variable para controlar, editar, etc... la lista ligada
//template < class NododeCualquierClase < ClaseDinamica >, class DatosdelNodoCualquierClase > class ListaLigada
ListaLigada < nodo <Paquetes>, Paquetes > GL_listaPaquetes(GL_txtEsp);
ListaLigada < nodo <Cliente>, Cliente > GL_listaCliente(GL_txtCl);
ListaLigada < nodo <Citas>, Citas > GL_listaCitas(GL_txtCitas);
Arbol GL_ArbolMecanicos(GL_txtMeca);

Mecanicos* inicio = 0, * final = 0, * aux = 0, * auxi = 0;

void OrdenarHeap(HWND hWnd)
{
	int tamaño = 0, filtrar = 0, indice = 0, ciclo = 0;

	char nombre[MAX_PATH] = "";
	Cliente* lista = NULL, * plista = 0;
	nodo<Cliente>* pCliente = GL_listaCliente.primero;
	while (pCliente != 0)
	{
		tamaño++;
		pCliente = pCliente->sig;
	}
	pCliente = GL_listaCliente.primero;
	lista = new Cliente[tamaño];
	plista = lista;
	while (ciclo < tamaño)
	{
		lista[ciclo] = pCliente->datos;
		ciclo++;
		pCliente = pCliente->sig;
	}
	heapSort(plista, tamaño);
	for (int i = 0; i < tamaño; i++)
	{
		char nombre[60] = "";
		strcpy(nombre, lista[i].apellidoCl);
		strcat(nombre, " ");
		strcat(nombre, lista[i].apellidoCl2);
		strcat(nombre, " ");
		strcat(nombre, lista[i].nombre);

		SendDlgItemMessage(hWnd, IDC_LBCLIENTESR, LB_ADDSTRING, 0, (LPARAM)nombre);
	}
	char txt[MAX_PATH] = "";
	GetCurrentDirectory(MAX_PATH, txt);
	strcat(txt, "\\ClientesReporte.txt");
	ofstream archivo;
	archivo.open(txt);
	if (archivo.is_open())
	{
		for (int i = 0; i < tamaño; i++)
		{
			char nombre[MAX_PATH] = "";
			char demas[MAX_PATH] = "";
			strcpy(nombre, lista[i].apellidoCl);
			strcat(nombre, " ");
			strcat(nombre, lista[i].apellidoCl2);
			strcat(nombre, " ");
			strcat(nombre, lista[i].nombre);
			strcat(nombre, " ");
			strcat(nombre, lista[i].telCl);
			strcat(nombre, " ");
			strcat(nombre, lista[i].claveP);
			strcat(nombre, " ");
			strcat(demas, lista[i].marca);
			strcat(demas, " ");
			strcat(demas, lista[i].modelo);
			strcat(demas, " ");
			strcat(demas, lista[i].anioVehiculo);
			strcat(demas, " ");
			strcat(demas, lista[i].kilometraje);
			strcat(demas, " ");
			strcat(demas, lista[i].direccion);

			archivo << nombre << demas << "\n";
		}
		archivo.close();
		delete[] lista;
	}
}

void heapSort(Cliente* arreglo, int tamaño)
{
	for (int i = tamaño / 2 - 1; i >= 0; i--)
		heapify(arreglo, tamaño, i);
	for (int i = tamaño - 1; i >= 0; i--)
	{
		swap(arreglo[0], arreglo[i]);
		heapify(arreglo, i, 0);
	}
}

void heapify(Cliente* arreglo, int tamaño, int indice)
{
	int largest = indice;
	int l = 2 * indice + 1;
	int r = 2 * indice + 2;

	if (l < tamaño && strcmp(arreglo[l].apellidoCl, arreglo[largest].apellidoCl)>0)
		largest = l;

	if (r < tamaño && strcmp(arreglo[r].apellidoCl, arreglo[largest].apellidoCl)>0)
		largest = r;

	if (largest != indice)
	{
		swap(arreglo[indice], arreglo[largest]);

		heapify(arreglo, tamaño, largest);
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	hVentanaPrincipal = CreateDialog(
		hInst,
		MAKEINTRESOURCE(IDD_LOGIN),
		NULL,
		IniciarSesion
	);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	ShowWindow(hVentanaPrincipal, SW_SHOW);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL CALLBACK VentanaPrincipal(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMUSURIO);
		SetWindowText(Nombre, login.nombreUsuario);

		hLogo = (HBITMAP)
			LoadImage(NULL, "AUTOCAR.bmp", IMAGE_BITMAP, 600, 350, LR_LOADFROMFILE);
		SendDlgItemMessage(hWnd, BM_AUTOCAR, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogo);
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
			HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
			ShowWindow(RegMecanicos, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}
	return FALSE;

}

BOOL CALLBACK IniciarSesion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

	}

	case WM_COMMAND:
		if (LOWORD(wParam) == BTN_LOGIN && HIWORD(wParam) == BN_CLICKED) {
			char usuario[MAX_PATH] = "";
			HWND ValidacionUsuario = GetDlgItem(hWnd, IDC_USUARIOLOGIN);
			int length = (int)GetWindowTextLength(ValidacionUsuario);
			GetWindowText(ValidacionUsuario, (LPSTR)usuario, length + 1);

			if (length == 0) {
				MessageBox(hWnd, "No se coloco ningún Usuario", "INICIAR SESION", MB_OK);
				GL_validar = false;
			}
			else {
				GL_validar = true;
			}
			if (GL_validar == true) {
				for (i = 0; i < length; i++) {
					if (isspace(login.usuario[i]))
						GL_validar = false;
					else
						GL_validar = true;
				}
			}
			if (GL_validar == true) {
				if (GL_validar == true) {
					for (int i = 0; i < length; i++) {
						if (usuario[i] == 32) {
							GL_validar = false;
						}/*
						else {
							Validar = true;
						}*/
					}
					if (GL_validar == false)
						MessageBox(hWnd, "NO ESPACIOS", "Usuario", MB_OK);
				}
				if (strcmp(usuario, login.usuario) == 0) {
					GL_validar = true;
				}
				else {
					MessageBox(hWnd, "No se coloco el usuario correcto", "INICIAR SESION", MB_OK);
					GL_validar = false;
				}

				if (GL_validar == true) {
					char password[MAX_PATH] = "";
					HWND ValidacionContrasena = GetDlgItem(hWnd, IDC_CONTRASENALOGIN);
					int length = (int)GetWindowTextLength(ValidacionContrasena);
					GetWindowText(ValidacionContrasena, (LPSTR)password, length + 1);
					if (length = 0) {
						MessageBox(hWnd, "No se coloco ninguna contraseña", "INICIAR SESION", MB_OK);
						GL_validar = false;
					}
					else {
						GL_validar = true;
					}
					if (GL_validar == true) {
						if (strcmp(password, login.password) == 0) {
							HWND VenP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_VENPRIN), NULL, VentanaPrincipal);
							ShowWindow(VenP, SW_SHOW);
							ShowWindow(hWnd, SW_HIDE);
						}
						else {
							MessageBox(hWnd, "No se coloco la contraseña correcta", "INICIAR SESION", MB_OK);
							GL_validar = false;

						}
					}
				}
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(10);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);

		break;
	}
	return FALSE;
}

BOOL CALLBACK RegistroMecanicos(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static char chFileName[MAX_PATH] = "";
	static Mecanicos temp, * temPunt;
	static nodo<Paquetes>* tempEsp, * auxEsp;
	static bool MAceptar = true;
	string bitmap;

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMBRE);
		SetWindowText(Nombre, login.nombreUsuario);

		cbTurnoDeTrabajo = GetDlgItem(hWnd, TURNO_TRABAJO);
		GL_cant = (sizeof(Turno) / (sizeof(char*)));
		for (i = 0; i < GL_cant; i++) {
			SendMessage(cbTurnoDeTrabajo, CB_ADDSTRING, 0, (LPARAM)Turno[i]);
		}
		SendMessage(cbTurnoDeTrabajo, CB_SETCURSEL, 0, 0);

		GL_listaPaquetes.llenarCBP(hWnd, COMBO_ESPECIALIDAD, CB_ADDSTRING, CB_SETITEMDATA);
		GL_ArbolMecanicos.Llenar_Lista_Preorden(hWnd, IDC_LBMODMECANICO, LB_ADDSTRING, LB_SETITEMDATA, GL_ArbolMecanicos.raiz);
		return true;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == BTN_FOTO && HIWORD(wParam) == BN_CLICKED) {

			char Temporal[MAX_PATH] = "";
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "BMP\0*.BMP\0";
			ofn.lpstrFile = Temporal;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Busca un archivo";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if (GetOpenFileNameA(&ofn) == true && strcmp(Temporal, "") != 0)
			{
				strcpy(temp.chFileName, Temporal);
				bmp = (HBITMAP)LoadImage(NULL, temp.chFileName, IMAGE_BITMAP, 100, 120, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, IMAGEN_MECANICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				ShowWindow(GetDlgItem(hWnd, IMAGEN_MECANICO), true);
			}

		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {

			temp.Lunes = false;
			temp.Martes = false;
			temp.Miercoles = false;
			temp.Jueves = false;
			temp.Viernes = false;
			bitmap = temp.chFileName;
			char numEmpT[MAX_PATH] = "";

			//Cedula
			string numeroEmpleado;
			SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, WM_GETTEXT, sizeof(temp.numeroEmpleado), (LPARAM)temp.numeroEmpleado);
			
			numeroEmpleado = temp.numeroEmpleado;
			if (numeroEmpleado.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en la sección ""Numero de Empleado""", "Numero del Mecanico", MB_OK);
				GL_validar = false;
			}
			else {
				GL_validar = true;
			}
			if (GL_validar == true) {
				for (int i = 0; i < numeroEmpleado.size(); i++) {
					if (numeroEmpleado[i] == 32) {
						GL_validar = false;
					}
				}
				if (GL_validar == false)
					MessageBox(hWnd, "NO ESPACIOS", "Numero de Empleado", MB_OK);
			}
			if (GL_validar == true) {
				for (int i = 0; i < numeroEmpleado.size(); i++) {
					if (numeroEmpleado[i] > 57 || numeroEmpleado[i] < 48) {
						GL_validar = false;
					}
				}
				if (GL_validar == false)
					MessageBox(hWnd, "Coloca solo numeros en el numero de Empleado", "Numero de Empleado", MB_OK);
			}
			
			
			if (validarNumEmp == true) {
				 
				if (GL_validar) {

					if (GL_ArbolMecanicos.BuscarSi(temp.numeroEmpleado) == 0) {
						MessageBox(hWnd, "Numero de Empleado no igual", "Alerta", MB_OK);
						valDiasEmp = true;
						GL_validar = false;
					}
					else GL_validar = true;
				}
			}

			//NOMBRE
			string nombreMecanico;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_NOMMECANICO, WM_GETTEXT, sizeof(temp.nombre), (LPARAM)temp.nombre);
				nombreMecanico = temp.nombre;
				if (nombreMecanico.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Nombre del Mecanico""", "Nombre del Mecanico", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}
			if (GL_validar == true) {
				for (int i = 0; i < nombreMecanico.size(); i++) {
					if ((nombreMecanico[i] < 65 || nombreMecanico[i] > 90) && (nombreMecanico[i] < 97 || nombreMecanico[i] > 122) && (nombreMecanico[i] != 32)) {
						GL_validar = false;
					}
				}
				if (GL_validar == false) {
					MessageBox(hWnd, "Coloca solo letras en ""Nombre del Mecanico""", "Nombre del Mecanico", MB_OK);
				}
			}

			//apellido Paterno
			string apPatMecanico;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_EDBAPPA, WM_GETTEXT, sizeof(temp.apPaterno), (LPARAM)temp.apPaterno);
				apPatMecanico = temp.apPaterno;
				if (apPatMecanico.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Apellido Paterno del Mecanico""", "Apellido Paterno del Mecanico", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}
			if (GL_validar == true) {
				for (int i = 0; i < apPatMecanico.size(); i++) {
					if ((apPatMecanico[i] < 65 || apPatMecanico[i] > 90) && (apPatMecanico[i] < 97 || apPatMecanico[i] > 122) && (apPatMecanico[i] != 32)) {
						GL_validar = false;
					}
				}
				if (GL_validar == false) {
					MessageBox(hWnd, "Coloca solo letras en ""Apellido Paterno del Mecanico""", "Nombre del Mecanico", MB_OK);
				}
			}

			//apellido Materno
			string apMatMecanico;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_EDBAPMA, WM_GETTEXT, sizeof(temp.apMaterno), (LPARAM)temp.apMaterno);
				apMatMecanico = temp.apMaterno;
				if (apMatMecanico.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Apellido Materno del Mecanico""", "Apellido Materno del Mecanico", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}
			if (GL_validar == true) {
				for (int i = 0; i < apMatMecanico.size(); i++) {
					if ((apMatMecanico[i] < 65 || apMatMecanico[i] > 90) && (apMatMecanico[i] < 97 || apMatMecanico[i] > 122) && (apMatMecanico[i] != 32)) {
						GL_validar = false;
					}
				}
				if (GL_validar == false) {
					MessageBox(hWnd, "Coloca solo letras en ""Apellido Materno del Mecanico""", "Nombre del Mecanico", MB_OK);
				}
			}

			SendDlgItemMessage(hWnd, TURNO_TRABAJO, WM_GETTEXT, sizeof(temp.turnoTrabajo), (LPARAM)temp.turnoTrabajo);
			SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, WM_GETTEXT, sizeof(temp.especialidad), (LPARAM)temp.especialidad);

			if (GL_validar) {
				if (SendDlgItemMessage(hWnd, TURNO_TRABAJO, CB_GETCURSEL, 0, 0) == -1) {
					MessageBox(hWnd, "No has elegido nada del turno de trabajo", "Turno Trabajo", MB_OK);
					GL_validar = false;
				}
				else GL_validar = true;
			}

			if (GL_validar) {
				if (SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_GETCURSEL, 0, 0) == -1) {
					MessageBox(hWnd, "No has elegido nada de especialidad", "Especialidad", MB_OK);
					GL_validar = false;
				}
				else GL_validar = true;
			}

			////////////Telefono
			string NumeroTelefono;
			if (GL_validar == true) {

				SendDlgItemMessage(hWnd, CELULAR_MECANICO, WM_GETTEXT, sizeof(temp.telefono), (LPARAM)temp.telefono);
				NumeroTelefono = temp.telefono;
				if (NumeroTelefono.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Numero de Telefono""", "Telefono del Mecanico", MB_OK);
					GL_validar = false;
				}
				else {
					if (NumeroTelefono.size() == 8) {
						GL_validar = true;
					}
					else {
						MessageBox(hWnd, "Debe colocarse solo 8 numeros en el telefono", "Telefono del Mecanico", MB_OK);
						GL_validar = false;
					}
				}

				if (GL_validar == true) {
					for (int i = 0; i < NumeroTelefono.size(); i++) {
						if (NumeroTelefono[i] > 57 || NumeroTelefono[i] < 48) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "Solo numeros", "Numero de Telefono", MB_OK);
				}
			}

			if (GL_validar == true) {
				//length = 0;
				if (IsDlgButtonChecked(hWnd, IDC_CKLUNES) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_CKMARTES) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_CKMIERCOLES) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_CKJUEVES) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_CKVIERNES) == BST_CHECKED) {
					if (IsDlgButtonChecked(hWnd, IDC_CKLUNES) == BST_CHECKED) {
						temp.Lunes = true;
						GL_validar = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_CKMARTES) == BST_CHECKED) {

						GL_validar = true;
						temp.Martes = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_CKMIERCOLES) == BST_CHECKED) {

						GL_validar = true;
						temp.Miercoles = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_CKJUEVES) == BST_CHECKED) {

						GL_validar = true;
						temp.Jueves = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_CKVIERNES) == BST_CHECKED) {

						GL_validar = true;
						temp.Viernes = true;
					}
				}
				else {
					GL_validar = false;
					MessageBox(hWnd, "No se selecciono ninguna opción en días de la semana", "Mecanicos", MB_OK);
				}
			}

			if (valDiasEmp == false) {
				if (GL_validar) {

					GL_ArbolMecanicos.ValidarDias(GL_ArbolMecanicos.raiz, temp.turnoTrabajo, temp.especialidad, temp.numeroEmpleado, temp.Lunes, temp.Martes, temp.Miercoles, temp.Jueves, temp.Viernes, hWnd);
					//GL_validar = turnoDiasVal;
				}

			}

			if(GL_validar == true) {
				tempEsp = GL_listaPaquetes.buscar(SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_GETITEMDATA, SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_GETCURSEL, 0, 0), 0));
				if (MAceptar) {
					if (GL_ArbolMecanicos.Buscar(temp.numeroEmpleado) != 0) {
						MessageBox(hWnd, "Numero de Empleado repetido", "Alerta", MB_OK);
					}
					else {
						if (bitmap.size() != 0) {
							GL_ArbolMecanicos.Agregar_Usuario(temp);
							MessageBox(hWnd, "Nuevo Mecanico guardado", "Mecanico", MB_OK);
							valDiasEmp = false;
							validarNumEmp = false; //fin limpieza
						}
						else {
							MessageBox(hWnd, "Hace falta una imagen", "Foto de Mecanicos", MB_OK);
							GL_validar = false;
						}
					}
				}
				else {
					GL_ArbolMecanicos.Copiar_Datos(temPunt, temp);
					MAceptar = true;
				}
			
				//inicio limpieza
			SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, WM_SETTEXT, 0,(LPARAM) "");
			SendDlgItemMessage(hWnd, IDC_NOMMECANICO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_EDBAPPA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_EDBAPMA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, CELULAR_MECANICO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, TURNO_TRABAJO, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, IDC_CKLUNES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKMARTES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKMIERCOLES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKJUEVES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKVIERNES, BM_SETCHECK, BST_UNCHECKED, 0);
			temp.Lunes = false;
			temp.Martes = false;
			temp.Miercoles = false;
			temp.Jueves = false;
			temp.Viernes = false;

			strcpy(temp.chFileName, "");
			GL_validar = true;
			bmp = (HBITMAP)LoadImage(NULL, temp.chFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, IMAGEN_MECANICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
			ShowWindow(GetDlgItem(hWnd, IMAGEN_MECANICO), false);
			SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_RESETCONTENT, 0, 0); //reset listbox
			GL_ArbolMecanicos.Llenar_Lista_Preorden(hWnd, IDC_LBMODMECANICO, LB_ADDSTRING, LB_SETITEMDATA, GL_ArbolMecanicos.raiz);
			}
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNLIMPIARMECANICO && HIWORD(wParam) == BN_CLICKED) {
		
			SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_NOMMECANICO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_EDBAPPA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_EDBAPMA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, CELULAR_MECANICO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, TURNO_TRABAJO, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, IDC_CKLUNES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKMARTES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKMIERCOLES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKJUEVES, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_CKVIERNES, BM_SETCHECK, BST_UNCHECKED, 0);
			temp.Lunes = false;
			temp.Martes = false;
			temp.Miercoles = false;
			temp.Jueves = false;
			temp.Viernes = false;
			valDiasEmp = false;
			validarNumEmp = false; //fin limpieza

			strcpy(temp.chFileName, "");

			bmp = (HBITMAP)LoadImage(NULL, temp.chFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, IMAGEN_MECANICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
			ShowWindow(GetDlgItem(hWnd, IMAGEN_MECANICO), false);
			SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_RESETCONTENT, 0, 0);
			GL_ArbolMecanicos.Llenar_Lista_Preorden(hWnd, IDC_LBMODMECANICO, LB_ADDSTRING, LB_SETITEMDATA, GL_ArbolMecanicos.raiz);
			MAceptar = true;
			//tempEsp->datos.mecPaquete = true;
			//return true;
		}
		if (LOWORD(wParam) == IDC_BTNMODMECANICO && HIWORD(wParam) == BN_CLICKED) {
			string busc;
			char buscarUs[MAX_PATH] = "";
			if (SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETCURSEL, 0, 0) != -1) {
				SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETTEXT, SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETCURSEL, 0, 0), (LPARAM)buscarUs);
				busc = buscarUs;
				int pos = busc.find_first_of("-");
				pos--;
				strcpy(buscarUs, busc.substr(0, pos).c_str());
				temPunt = GL_ArbolMecanicos.Buscar(buscarUs);
				SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, WM_SETTEXT, 0, (LPARAM)temPunt->numeroEmpleado);
				SendDlgItemMessage(hWnd, IDC_NOMMECANICO, WM_SETTEXT, 0, (LPARAM)temPunt->nombre);
				SendDlgItemMessage(hWnd, IDC_EDBAPPA, WM_SETTEXT, 0, (LPARAM)temPunt->apPaterno);
				SendDlgItemMessage(hWnd, IDC_EDBAPMA, WM_SETTEXT, 0, (LPARAM)temPunt->apMaterno);
				SendDlgItemMessage(hWnd, CELULAR_MECANICO, WM_SETTEXT, 0, (LPARAM)temPunt->telefono);
				SendDlgItemMessage(hWnd, TURNO_TRABAJO, CB_SELECTSTRING, -1, (LPARAM)temPunt->turnoTrabajo);
				SendDlgItemMessage(hWnd, COMBO_ESPECIALIDAD, CB_SELECTSTRING, -1, (LPARAM)temPunt->especialidad);
				if (temPunt->Lunes == 1) SendDlgItemMessage(hWnd, IDC_CKLUNES, BM_SETCHECK, BST_CHECKED, 0);
				if (temPunt->Martes == 1) SendDlgItemMessage(hWnd, IDC_CKMARTES, BM_SETCHECK, BST_CHECKED, 0);
				if (temPunt->Miercoles == 1) SendDlgItemMessage(hWnd, IDC_CKMIERCOLES, BM_SETCHECK, BST_CHECKED, 0);
				if (temPunt->Jueves == 1) SendDlgItemMessage(hWnd, IDC_CKJUEVES, BM_SETCHECK, BST_CHECKED, 0);
				if (temPunt->Viernes == 1) SendDlgItemMessage(hWnd, IDC_CKVIERNES, BM_SETCHECK, BST_CHECKED, 0);

				bmp = (HBITMAP)LoadImage(NULL, temPunt->chFileName, IMAGE_BITMAP, 100, 120, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, IMAGEN_MECANICO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				strcpy(temp.chFileName, temPunt->chFileName);
				ShowWindow(GetDlgItem(hWnd, IMAGEN_MECANICO), true);
				auxEsp = GL_listaPaquetes.buscar(SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, CB_GETITEMDATA, SendDlgItemMessage(hWnd, NUMERO_EMPLEADO, CB_GETCURSEL, 0, 0), 0));
				//auxEsp->datos.mecPaquete = false;
				MAceptar = false;
				validarNumEmp = true;
			}
		}
		if (LOWORD(wParam) == IDC_BTNDELETEELIMINAR && HIWORD(wParam) == BN_CLICKED) {
			string busc;
			char buscarUs[50] = "";
			if (SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETCURSEL, 0, 0) != -1) {
				SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETTEXT, SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETCURSEL, 0, 0), (LPARAM)buscarUs);
				busc = buscarUs;
				int pos = busc.find_first_of("-");
				pos--;
				strcpy(buscarUs, busc.substr(0, pos).c_str());

				tempEsp = GL_listaPaquetes.buscar(SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETITEMDATA, SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_GETCURSEL, 0, 0), 0));
				//tempEsp->datos.mecPaquete = false;
				GL_ArbolMecanicos.Borrar_Usuario(GL_ArbolMecanicos.Buscar(buscarUs));
				SendDlgItemMessage(hWnd, IDC_LBMODMECANICO, LB_RESETCONTENT, 0, 0);
				GL_ArbolMecanicos.Llenar_Lista_Preorden(hWnd, IDC_LBMODMECANICO, LB_ADDSTRING, LB_SETITEMDATA, GL_ArbolMecanicos.raiz);

			}
		}
		if (LOWORD(wParam) == IDC_BTNVERREPORTE && HIWORD(wParam) == BN_CLICKED) {
			
			HWND repMec = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTEMECANICOS), NULL, ReporteMecanicos);
			ShowWindow(repMec, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK Clientes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static nodo<Cliente>* aux = NULL, * auxBorrar = NULL;
	static bool aceptarB = true;

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMUSURU);
		SetWindowText(Nombre, login.nombreUsuario);

		SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_RESETCONTENT, 0, 0);
		GL_listaCliente.llenarLByCB(hWnd, IDC_LBMODCLIENTE, LB_ADDSTRING, LB_SETITEMDATA);
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNREPORTE && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCl = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECLIENTES), NULL, ReporteClientes);
			ShowWindow(RepCl, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {

			//APELLIDO Pat
			Cliente temporalCl;
			string apellidoPa;
			SendDlgItemMessage(hWnd, IDC_APCLIENTE, WM_GETTEXT, sizeof(temporalCl.apellidoCl), (LPARAM)temporalCl.apellidoCl);
			apellidoPa = temporalCl.apellidoCl;
			if (apellidoPa.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en ""Apellido Paterno del Cliente""", "Apellidos", MB_OK);
				GL_validar = false;
			}
			else {
				GL_validar = true;
			}
			if (GL_validar == true) {
				for (int i = 0; i < apellidoPa.size(); i++) {
					if ((apellidoPa[i] < 65 || apellidoPa[i] > 90) && (apellidoPa[i] < 97 || apellidoPa[i] > 122) && (apellidoPa[i] != 32)) {
						GL_validar = false;
					}
				}
				if (GL_validar == false) {
					MessageBox(hWnd, "Coloca solo letras en ""Apellido Paterno de Cliente""", "Apellidos", MB_OK);
				}
			}

			//APELLIDO Mat
			string apellidoMa;
			SendDlgItemMessage(hWnd, IDC_APCLIENTE2, WM_GETTEXT, sizeof(temporalCl.apellidoCl2), (LPARAM)temporalCl.apellidoCl2);
			apellidoMa = temporalCl.apellidoCl2;
			if (apellidoMa.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en ""Apellido Materno del Cliente""", "Apellidos", MB_OK);
				GL_validar = false;
			}
			else {
				GL_validar = true;
			}
			if (GL_validar == true) {
				for (int i = 0; i < apellidoMa.size(); i++) {
					if ((apellidoMa[i] < 65 || apellidoMa[i] > 90) && (apellidoMa[i] < 97 || apellidoMa[i] > 122) && (apellidoMa[i] != 32)) {
						GL_validar = false;
					}
				}
				if (GL_validar == false) {
					MessageBox(hWnd, "Coloca solo letras en ""Apellido Materno de Cliente""", "Apellidos", MB_OK);
				}
			}

			///NOMBRE 
			string nombrePa;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTE, WM_GETTEXT, sizeof(temporalCl.nombre), (LPARAM)temporalCl.nombre);
				nombrePa = temporalCl.nombre;
				if (nombrePa.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Nombre del Cliente""", "Nombre del Cliente", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
				if (GL_validar == true) {
					for (int i = 0; i < nombrePa.size(); i++) {
						if ((nombrePa[i] < 65 || nombrePa[i] > 90) && (nombrePa[i] < 97 || nombrePa[i] > 122) && (nombrePa[i] != 32)) {
							GL_validar = false;
						}
					}
					if (GL_validar == false) {
						MessageBox(hWnd, "Coloca solo letras en ""Nombre de Cliente""", "Nombre de Cliente", MB_OK);
					}
				}
			}

			////////////Telefono
			string numTelPac;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_TELEFONOCLIENTE, WM_GETTEXT, sizeof(temporalCl.telCl), (LPARAM)temporalCl.telCl);
				numTelPac = temporalCl.telCl;
				if (numTelPac.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Numero de Telefono""", "Telefono del Cliente", MB_OK);
					GL_validar = false;
				}
				else {
					if (numTelPac.size() == 8) {
						GL_validar = true;
					}
					else {

						MessageBox(hWnd, "Deben colocarse 8 numeros", "Telefono del Cliente", MB_OK);
						GL_validar = false;
					}
				}

				if (GL_validar == true) {
					for (int i = 0; i < numTelPac.size(); i++) {
						if (numTelPac[i] > 57 || numTelPac[i] < 48) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "Coloca solo numeros en ""Telefono""", "Numero de Telefono", MB_OK);
				}
			}

			///////////Direccion
			string direccionCl;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_DIRECCIONCLIENTE, WM_GETTEXT, sizeof(temporalCl.direccion), (LPARAM)temporalCl.direccion);
				direccionCl = temporalCl.direccion;
				if (direccionCl.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en Direccion", "CLIENTES", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}

			////////////Placas del vehiculo
			string numPlacas;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_PLACAVEHICULO, WM_GETTEXT, sizeof(temporalCl.claveP), (LPARAM)temporalCl.claveP);
				numPlacas = temporalCl.claveP;
				if (numPlacas.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Placas del vehiculo""", "Placas del vehiculo", MB_OK);
					GL_validar = false;
				}
				else {
					if (numPlacas.size() == 6 || numPlacas.size() == 7) {
						GL_validar = true;
					}
					else {

						MessageBox(hWnd, "Debe colocarse solo 6 o 7 caracteres", "Placas del vehiculo", MB_OK);
						GL_validar = false;
					}
				}
				if (GL_validar == true) {
					for (int i = 0; i < numPlacas.size(); i++) {
						if (numPlacas[i] == 32) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "NO ESPACIOS", "Placas del vehiculo", MB_OK);
				}

				if (GL_validar == true) {
					for (int i = 0; i < numPlacas.size(); i++) {
						if ((numPlacas[i] > 57 || numPlacas[i] < 48) && (numPlacas[i] < 65 || numPlacas[i] > 90)) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "Coloca solo numeros o letras mayusculas en ""Placas del vehiculo""", "Placas del vehiculo", MB_OK);

				}
			}

			////Marca del vehiculo
			string marcaVehiculo;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_MARCAVEHICULO, WM_GETTEXT, sizeof(temporalCl.marca), (LPARAM)temporalCl.marca);
				marcaVehiculo = temporalCl.marca;
				if (marcaVehiculo.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Marca del Vehiculo""", "Marca del Vehiculo", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}

			////Modelo del vehiculo
			string modeloVehiculo;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_MODELOVEHICULO, WM_GETTEXT, sizeof(temporalCl.modelo), (LPARAM)temporalCl.modelo);
				modeloVehiculo = temporalCl.modelo;
				if (modeloVehiculo.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Modelo del Vehiculo""", "Modelo del Vehiculo", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}

			////Año del modelo
			string yearModelo;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_ANIOVEHICULO, WM_GETTEXT, sizeof(temporalCl.anioVehiculo), (LPARAM)temporalCl.anioVehiculo);
				yearModelo = temporalCl.anioVehiculo;
				if (yearModelo.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Año del Modelo""", "Año del Modelo", MB_OK);
					GL_validar = false;
				}
				else {
					if (yearModelo.size() == 4) {
						GL_validar = true;
					}
					else {

						MessageBox(hWnd, "Deben colocarse 4 numeros", "Año del Modelo", MB_OK);
						GL_validar = false;
					}
				}

				if (GL_validar == true) {
					for (int i = 0; i < yearModelo.size(); i++) {
						if (yearModelo[i] > 57 || yearModelo[i] < 48) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "Coloca solo numeros en ""Año del Modelo""", "Año del Modelo", MB_OK);
				}
			}

			/////Kilometraje 
			string kmVehiculo;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_KMVEHICULO, WM_GETTEXT, sizeof(temporalCl.kilometraje), (LPARAM)temporalCl.kilometraje);
				kmVehiculo = temporalCl.kilometraje;
				if (kmVehiculo.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en ""Kilometraje""", "Kilometraje", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}

				if (GL_validar == true) {
					for (int i = 0; i < kmVehiculo.size(); i++) {
						if (kmVehiculo[i] > 57 || kmVehiculo[i] < 48) {
							GL_validar = false;
						}
					}
					if (GL_validar == false)
						MessageBox(hWnd, "Coloca solo numeros en ""Kilometraje""", "Kilometraje", MB_OK);
				}
			}

			if (GL_validar == true) {

				if (aceptarB) {
					if (GL_listaCliente.validarDias(temporalCl.claveP) != 0) {
						MessageBox(hWnd, "Placa de Cliente repetido", "Alerta", MB_OK);
					}
					else {
						GL_listaCliente.agregarNodos(temporalCl);
						MessageBox(hWnd, "Se agrego nuevo Cliente", "Cliente", MB_OK);

					}
				}
				else {
					aux->copiardatos(temporalCl);
					aceptarB = true;
				}

				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTE, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_APCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_APCLIENTE2, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_TELEFONOCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_DIRECCIONCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_PLACAVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_MARCAVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_MODELOVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_KMVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_ANIOVEHICULO, WM_SETTEXT, 0, (LPARAM)""); //aqui acaba la limpieza

				SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_RESETCONTENT, 0, 0);//recargar lisbox
				GL_listaCliente.llenarLByCB(hWnd, IDC_LBMODCLIENTE, LB_ADDSTRING, LB_SETITEMDATA);//recargar lisbox
				return true;
			}
			else {
				GL_validar = false;
			}
		}

		if (LOWORD(wParam) == IDC_BTNMODCLIENTE && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_GETITEMDATA, cursel, 0);
				aux = GL_listaCliente.buscar(nuevaClave);
				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTE, WM_SETTEXT, 0, (LPARAM)aux->datos.nombre);
				SendDlgItemMessage(hWnd, IDC_APCLIENTE, WM_SETTEXT, 0, (LPARAM)aux->datos.apellidoCl);
				SendDlgItemMessage(hWnd, IDC_APCLIENTE2, WM_SETTEXT, 0, (LPARAM)aux->datos.apellidoCl2);
				SendDlgItemMessage(hWnd, IDC_TELEFONOCLIENTE, WM_SETTEXT, 0, (LPARAM)aux->datos.telCl);
				SendDlgItemMessage(hWnd, IDC_DIRECCIONCLIENTE, WM_SETTEXT, 0, (LPARAM)aux->datos.direccion);
				SendDlgItemMessage(hWnd, IDC_PLACAVEHICULO, WM_SETTEXT, 0, (LPARAM)aux->datos.claveP);
				SendDlgItemMessage(hWnd, IDC_MARCAVEHICULO, WM_SETTEXT, 0, (LPARAM)aux->datos.marca);
				SendDlgItemMessage(hWnd, IDC_MODELOVEHICULO, WM_SETTEXT, 0, (LPARAM)aux->datos.modelo);
				SendDlgItemMessage(hWnd, IDC_KMVEHICULO, WM_SETTEXT, 0, (LPARAM)aux->datos.kilometraje);
				SendDlgItemMessage(hWnd, IDC_ANIOVEHICULO, WM_SETTEXT, 0, (LPARAM)aux->datos.anioVehiculo);

				aceptarB = false;
			}
			else {

				MessageBox(hWnd, "Elija una opcion del listbox", "Especialidad", MB_OK);
			}
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNLIMPIARCLIENTE && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_APCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_APCLIENTE2, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_TELEFONOCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_DIRECCIONCLIENTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_PLACAVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_MARCAVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_MODELOVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_KMVEHICULO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_ANIOVEHICULO, WM_SETTEXT, 0, (LPARAM)""); //aqui acaba la limpieza
			aceptarB = true;
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNDELETECLIENTE && HIWORD(wParam) == BN_CLICKED) {

			int posicionLBCB = 0, claveTempo = 0;
			//obtener la posicion de lo que se tiene seleccionao
			posicionLBCB = SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_GETCURSEL, 0, 0);

			if (posicionLBCB != -1) {
				claveTempo = SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_GETITEMDATA, (WPARAM)posicionLBCB, 0);
				nodo <Cliente>* tempoBorrar;
				//en tempoBorrar se reserva la direccion del nodo que se busco con la clave
				tempoBorrar = GL_listaCliente.buscar(claveTempo);

				GL_listaCliente.eliminarNodos(tempoBorrar);

				SendDlgItemMessage(hWnd, IDC_LBMODCLIENTE, LB_RESETCONTENT, 0, 0);
				GL_listaCliente.llenarLByCB(hWnd, IDC_LBMODCLIENTE, LB_ADDSTRING, LB_SETITEMDATA);
			}
			else {
				MessageBox(hWnd, "Elija una opcion del listbox", "Especialidad", MB_OK);
			}
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK RegistroPaquetes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static nodo<Paquetes>* aux = NULL, * auxBorrar = NULL;
	static bool aceptarB = true;

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMBREUS);
		SetWindowText(Nombre, login.nombreUsuario);

		SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_RESETCONTENT, 0, 0);
		GL_listaPaquetes.llenarLByCB(hWnd, IDC_LBPAQUETES, LB_ADDSTRING, LB_SETITEMDATA);

		return true;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {

			Paquetes temporal;
			//Clave
			string claveEsp;
			SendDlgItemMessage(hWnd, IDC_CLAVEPROD, WM_GETTEXT, sizeof(temporal.claveP), (LPARAM)temporal.claveP);
			claveEsp = temporal.claveP;
			if (claveEsp.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en la sección ""Clave de Especialidad""", "Especialidad", MB_OK);
				GL_validar = false;
			}
			else {
				GL_validar = true;
			}
			if (GL_validar == true) {
				for (int i = 0; i < claveEsp.size(); i++) {
					if (claveEsp[i] == 32) {
						GL_validar = false;
					}
				}
				if (GL_validar == false)
					MessageBox(hWnd, "NO ESPACIOS", "Especialidad", MB_OK);
			}
			if (GL_validar == true) {
				for (int i = 0; i < claveEsp.size(); i++) {
					if (claveEsp[i] > 57 || claveEsp[i] < 48) {
						GL_validar = false;
					}
				}
				if (GL_validar == false)
					MessageBox(hWnd, "Coloca solo numeros en la clave de Especialidad", "Especialidad", MB_OK);
			}

			//Nombre
			string nombrePaq;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_NOMPAQ, WM_GETTEXT, sizeof(temporal.nombre), (LPARAM)temporal.nombre);
				nombrePaq = temporal.nombre;
				if (nombrePaq.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Nombre de la especialidad""", "Especialidad", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}

			//validacion costo
			SendDlgItemMessage(hWnd, IDC_PRECIO, WM_GETTEXT, sizeof(temporal.precioP), (LPARAM)temporal.precioP);
			string validarPrecio;
			GL_contPunto = 0;
			validarPrecio = temporal.precioP;

			if (validarPrecio.size() != 0) {
				for (i = 0; i < validarPrecio.size(); i++) {
					if (isdigit(validarPrecio[i]) == 0)
						GL_validar = false;
				}
				for (i = 0; i < validarPrecio.size(); i++) {
					if (validarPrecio[i] == 46) {
						GL_contPunto++;
					}
				}
				if (GL_contPunto == 1) GL_validar = true;
				if (GL_validar == false && GL_contPunto != 1) MessageBox(NULL, "Costo no valido", "Costo", MB_ICONERROR);
			}
			else {
				MessageBox(hWnd, "Costo no valido, esta vacio", "Costo", MB_ICONERROR);
			}

			//descripcion
			string descripcion;
			if (GL_validar == true) {
				SendDlgItemMessage(hWnd, IDC_DESCRIPCION, WM_GETTEXT, sizeof(temporal.descripcion), (LPARAM)temporal.descripcion);
				descripcion = temporal.descripcion;

				if (descripcion.size() == 0) {
					MessageBox(hWnd, "No se coloco nada en la sección ""Descripcion""", "Especialidad", MB_OK);
					GL_validar = false;
				}
				else {
					GL_validar = true;
				}
			}

			if (GL_validar == true) {

				if (aceptarB) {
					if (GL_listaPaquetes.validarDias(temporal.claveP) != 0) {
						MessageBox(hWnd, "Numero de Paquete repetido", "Alerta", MB_OK);
					}
					else {
						GL_listaPaquetes.agregarNodos(temporal);
						MessageBox(hWnd, "Se agrego nueva Especialidad", "Especialidad", MB_OK);

					}
				}
				else {

					aux->copiardatos(temporal);
					aceptarB = true;
				}

				SendDlgItemMessage(hWnd, IDC_CLAVEPROD, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_NOMPAQ, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_PRECIO, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hWnd, IDC_DESCRIPCION, WM_SETTEXT, 0, (LPARAM)""); //aqui acaba la limpieza

				SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_RESETCONTENT, 0, 0);//recargar lisbox
				GL_listaPaquetes.llenarLByCB(hWnd, IDC_LBPAQUETES, LB_ADDSTRING, LB_SETITEMDATA);//recargar lisbox
			}
		}

		if (LOWORD(wParam) == IDC_BTNMODESP && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_GETITEMDATA, cursel, 0);
				aux = GL_listaPaquetes.buscar(nuevaClave);
				SendDlgItemMessage(hWnd, IDC_CLAVEPROD, WM_SETTEXT, 0, (LPARAM)aux->datos.claveP);
				SendDlgItemMessage(hWnd, IDC_NOMPAQ, WM_SETTEXT, 0, (LPARAM)aux->datos.nombre);
				SendDlgItemMessage(hWnd, IDC_PRECIO, WM_SETTEXT, 0, (LPARAM)aux->datos.precioP);
				SendDlgItemMessage(hWnd, IDC_DESCRIPCION, WM_SETTEXT, 0, (LPARAM)aux->datos.descripcion);
				aceptarB = false;
			}
			else {

				MessageBox(hWnd, "Elija una opcion del listbox", "Especialidad", MB_OK);
			}
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNLIMPIARPAQ && HIWORD(wParam) == BN_CLICKED) {
		
			SendDlgItemMessage(hWnd, IDC_CLAVEPROD, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_NOMPAQ, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_PRECIO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_DESCRIPCION, WM_SETTEXT, 0, (LPARAM)""); //aqui acaba la limpieza
			aceptarB = true;
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNDELETEESP && HIWORD(wParam) == BN_CLICKED) {

			int posicionLBCB = 0, claveTempo = 0;
			//obtener la posicion de lo que se tiene seleccionao
			posicionLBCB = SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_GETCURSEL, 0, 0);

			if (posicionLBCB != -1) {
				claveTempo = SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_GETITEMDATA, (WPARAM)posicionLBCB, 0);
				nodo <Paquetes>* tempoBorrar;
				//en tempoBorrar se reserva la direccion del nodo que se busco con la clave
				tempoBorrar = GL_listaPaquetes.buscar(claveTempo);

				GL_listaPaquetes.eliminarNodos(tempoBorrar);

				SendDlgItemMessage(hWnd, IDC_LBPAQUETES, LB_RESETCONTENT, 0, 0);
				GL_listaPaquetes.llenarLByCB(hWnd, IDC_LBPAQUETES, LB_ADDSTRING, LB_SETITEMDATA);
			}
			else {

				MessageBox(hWnd, "Elija una opcion del listbox", "Especialidad", MB_OK);
			}
		}

		break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}

BOOL CALLBACK RegistroCitas(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static Citas tempo, *tempoP;
	static nodo<Citas>* auxi = NULL;
	static Mecanicos temp, *tempP;
	static nodo<Cliente>* aux = NULL;
	char especialidad[MAX_PATH] = "", todas[MAX_PATH] = "", cursel2[MAX_PATH] = "", numCita[MAX_PATH] = "", buscarUs[MAX_PATH] = "", nombreCM[MAX_PATH] = "", nombreCC[MAX_PATH] = "";
	static bool aceptarB = true;
	int cursel = 0, nuevaClave = 0;

	switch (msg) {
	case WM_INITDIALOG: {

		valCitas = false;

		HWND PrimeraEspecialidad = GetDlgItem(hWnd, IDC_CBESPECIALIDAD);
		SendMessage(PrimeraEspecialidad, CB_ADDSTRING, NULL, (LPARAM)"Todas");

		//SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, WM_GETTEXT, sizeof(todas), (LPARAM)todas);

		GL_listaPaquetes.llenarCBP(hWnd, IDC_CBESPECIALIDAD, CB_ADDSTRING, CB_SETITEMDATA);

		GL_listaCliente.llenarLByCB(hWnd, IDC_LBCLIENTES, LB_ADDSTRING, LB_SETITEMDATA);

		SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_RESETCONTENT, 0, 0);
		GL_listaCitas.llenarLByCB(hWnd, IDC_LBCITASREG, LB_ADDSTRING, LB_SETITEMDATA);

		HWND Hora_Combo = GetDlgItem(hWnd, IDC_CBHORA);
		char Horas[17][MAX_PATH]{ "06","07","08","09","10","11","12","13","14","15","16","17","18","19", "20", "21", "22" };
		for (int i = 0; i < 17; i++) {
			SendMessage(Hora_Combo, CB_ADDSTRING, NULL, (LPARAM)Horas[i]);

		}

		HWND Minuto_Combo = GetDlgItem(hWnd, IDC_CBMINUTOS);
		char Minutos[6][MAX_PATH]{ "00", "10", "20", "30", "40", "50" };
		for (int i = 0; i < 6; i++) {
			SendMessage(Minuto_Combo, CB_ADDSTRING, NULL, (LPARAM)Minutos[i]);
		}

		/*string tmp = to_string(numeCita);
		char numCita[MAX_PATH] = tmp.c_str();*/

		_itoa_s(numeCita, numCita, 10);
		SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)numCita);
	
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}
		if (LOWORD(wParam) == IDC_BTNESPBUSCAR && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_RESETCONTENT, 0, 0);
			SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, WM_GETTEXT, sizeof(especialidad), (LPARAM)especialidad);
			SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, WM_GETTEXT, sizeof(temp.especialidad), (LPARAM)temp.especialidad);

			if (SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, CB_GETCURSEL, 0, 0) == -1) {

				MessageBox(hWnd, "Elige una especialidad", "Csm", MB_OK);
			}
			else if (strcmp(especialidad, "Todas") == 0) {
				GL_ArbolMecanicos.Llenar_Lista_Preorden(hWnd, IDC_LBMECANICOCITA, LB_ADDSTRING, LB_SETITEMDATA, GL_ArbolMecanicos.raiz);
			}
			else {
				GL_ArbolMecanicos.AñadirMec(GL_ArbolMecanicos.raiz, hWnd, IDC_LBMECANICOCITA, temp.especialidad, LB_ADDSTRING, LB_SETITEMDATA);
			}

		}
		if (LOWORD(wParam) == IDC_BTNSELMEC && HIWORD(wParam) == BN_CLICKED)
		{
			cursel = SendDlgItemMessage(hWnd, IDC_LBCLIENTES, LB_GETCURSEL, 0, 0);

			SendDlgItemMessage(hWnd, IDC_LUNESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_MARTESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_MIERCOLESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_JUEVESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_VIERNESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);
			 
			if (cursel != -1 && SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_GETCURSEL, 0, 0) != -1) {

				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCLIENTES, LB_GETITEMDATA, cursel, 0);
				aux = GL_listaCliente.buscar(nuevaClave);

				string busc;
				SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_GETTEXT, SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_GETCURSEL, 0, 0), (LPARAM)cursel2);
				busc = cursel2;
				int pos = busc.find_first_of("-");
				pos--;
				strcpy(cursel2, busc.substr(0, pos).c_str());
				tempP = GL_ArbolMecanicos.Buscar(cursel2);

				bmp = (HBITMAP)LoadImage(NULL, tempP->chFileName, IMAGE_BITMAP, 100, 120, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, IDC_BITMAPMEC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				ShowWindow(GetDlgItem(hWnd, IDC_BITMAPMEC), true);

				if (tempP->Lunes == 1) SendDlgItemMessage(hWnd, IDC_LUNESC, BM_SETCHECK, BST_CHECKED, 0);
				if (tempP->Martes == 1) SendDlgItemMessage(hWnd, IDC_MARTESC, BM_SETCHECK, BST_CHECKED, 0);
				if (tempP->Miercoles == 1) SendDlgItemMessage(hWnd, IDC_MIERCOLESC, BM_SETCHECK, BST_CHECKED, 0);
				if (tempP->Jueves == 1) SendDlgItemMessage(hWnd, IDC_JUEVESC, BM_SETCHECK, BST_CHECKED, 0);
				if (tempP->Viernes == 1) SendDlgItemMessage(hWnd, IDC_VIERNESC, BM_SETCHECK, BST_CHECKED, 0);

				strcpy(nombreCM, tempP->nombre);
				strcat(nombreCM, " - ");
				strcat(nombreCM, tempP->apPaterno);
				strcat(nombreCM, " ");
				strcat(nombreCM, tempP->apMaterno);

				strcpy(nombreCC, aux->datos.nombre);
				strcat(nombreCC, " - ");
				strcat(nombreCC, aux->datos.apellidoCl);
				strcat(nombreCC, " ");
				strcat(nombreCC, aux->datos.apellidoCl2);

				strcpy(tempo.imagenMec, tempP->chFileName);
				strcpy(tempo.especialidadCita, tempP->especialidad);
				strcpy(tempo.claveMec, tempP->numeroEmpleado);

				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTECITA, WM_SETTEXT, 0, (LPARAM)nombreCC);
				SendDlgItemMessage(hWnd, IDC_CELULARCLCITA, WM_SETTEXT, 0, (LPARAM)aux->datos.telCl);
				SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_SETTEXT, 0, (LPARAM)nombreCM);
				SendDlgItemMessage(hWnd, IDC_TNMECANICO, WM_SETTEXT, 0, (LPARAM)tempP->turnoTrabajo);
				
				valCitas = true;
			}
			else MessageBox(hWnd, "Te hace falta elegir un cliente o un mecanico", "Alerta", MB_OK);
		}

		if (LOWORD(wParam) == IDC_BTNREGISTRARCITA && HIWORD(wParam) == BN_CLICKED) {
			
			tempo.LunesC = false;
			tempo.MartesC = false;
			tempo.MiercolesC = false;
			tempo.JuevesC = false;
			tempo.ViernesC = false;

			if (valCitas == true) {
				GL_validar = true;
				SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_GETTEXT, sizeof(tempo.nomMecanico), (LPARAM)tempo.nomMecanico);
				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTECITA, WM_GETTEXT, sizeof(tempo.nombre), (LPARAM)tempo.nombre);
				SendDlgItemMessage(hWnd, IDC_CELULARCLCITA, WM_GETTEXT, sizeof(tempo.celularCliente), (LPARAM)tempo.celularCliente);
				SendDlgItemMessage(hWnd, IDC_FECHACITAACT, WM_GETTEXT, sizeof(tempo.fechaCita), (LPARAM)tempo.fechaCita);
				SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_GETTEXT, sizeof(tempo.claveP), (LPARAM)tempo.claveP);
				SendDlgItemMessage(hWnd, IDC_TNMECANICO, WM_GETTEXT, sizeof(tempo.turnoTC), (LPARAM)tempo.turnoTC);
				
				if (IsDlgButtonChecked(hWnd, IDC_LUNESC) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_MARTESC) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_MIERCOLESC) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_JUEVESC) == BST_CHECKED || IsDlgButtonChecked(hWnd, IDC_VIERNESC) == BST_CHECKED) {
					if (IsDlgButtonChecked(hWnd, IDC_LUNESC) == BST_CHECKED) {
						tempo.LunesC = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_MARTESC) == BST_CHECKED) {
						tempo.MartesC = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_MIERCOLESC) == BST_CHECKED) {
						tempo.MiercolesC = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_JUEVESC) == BST_CHECKED) {
						tempo.JuevesC = true;
					}
					if (IsDlgButtonChecked(hWnd, IDC_VIERNESC) == BST_CHECKED) {
						tempo.ViernesC = true;
					}
				}

				////HORA
				string horaCita;
				if (GL_validar) {

					SendDlgItemMessage(hWnd, IDC_CBHORA, WM_GETTEXT, sizeof(tempo.horaCita), (LPARAM)tempo.horaCita);
					horaCita = tempo.horaCita;

					if (SendDlgItemMessage(hWnd, IDC_CBHORA, CB_GETCURSEL, 0, 0) == -1) {
						MessageBox(hWnd, "No se eligio nada en la sección ""Hora""", "Hora Cita", MB_OK);
						GL_validar = false;
					}
					else {
						GL_validar = true;
					}
				}

				/////MINUTOS 
				string minuCita;
				if (GL_validar) {

					SendDlgItemMessage(hWnd, IDC_CBMINUTOS, WM_GETTEXT, sizeof(tempo.minutoCita), (LPARAM)tempo.minutoCita);
					minuCita = tempo.minutoCita;

					if (SendDlgItemMessage(hWnd, IDC_CBMINUTOS, CB_GETCURSEL, 0, 0) == -1) {
						MessageBox(hWnd, "No se eligio nada en la sección ""Minuto""", "Minutos Cita", MB_OK);
						GL_validar = false;
					}
					else {
						GL_validar = true;
					}
				}

				//////FECHA 
				string Fecha;
				if (GL_validar) {

					int j = 0;
					int k = 0;
					int p = 0;

					SendDlgItemMessage(hWnd, IDC_DTPCITA, WM_GETTEXT, sizeof(tempo.fechaCita), (LPARAM)tempo.fechaCita);
					Fecha = tempo.fechaCita;

					for (int i = 0; i < Fecha.size(); i++) {
						if (tempo.fechaCita[i] != 44) {
							tempo.semanaCita[i] = tempo.fechaCita[i];
						}
						else {
							i++;
							i++;
							for (i; i < Fecha.size(); i++) {
								for (j; j < 4; j++) {
									if (tempo.fechaCita[i] != 32) {
										tempo.diaCita[j] = tempo.fechaCita[i];
										GL_validar = false;
										i++;
									}
									else {
										GL_validar = true;
									}
								}
								break;
							}
							if (GL_validar) {
								i++;
								i++;
								i++;
								i++;
								for (k; k < Fecha.size(); k++) {
									if (tempo.fechaCita[i] != 32) {
										tempo.mesCita[k] = tempo.fechaCita[i];
										i++;
										GL_validar = false;
									}
									else {
										GL_validar = true;
									}
									if (GL_validar) {
										k = Fecha.size();
									}
								}
								if (GL_validar) {
									i = i + 4;
									for (p; p < Fecha.size(); p++) {
										if (tempo.fechaCita[i] != 32) {
											tempo.anioCita[p] = tempo.fechaCita[i];
											i++;
											GL_validar = true;
										}
										else {
											i = Fecha.size();
											j = Fecha.size();
											k = Fecha.size();
											p = Fecha.size();
										}
									}
								}
							}
						}
					}
				}

				if (GL_validar) {
					
					GL_ArbolMecanicos.idSemana(GL_ArbolMecanicos.raiz, tempP->numeroEmpleado, hWnd, tempo.semanaCita);
					GL_ArbolMecanicos.mananaTarde(GL_ArbolMecanicos.raiz, tempo.turnoTC, tempP->numeroEmpleado, hWnd, tempo.horaCita);
					GL_listaCitas.validarCita(hWnd, tempo.especialidadCita, tempP->numeroEmpleado, tempo.minutoCita, tempo.mesCita, tempo.diaCita);
				}

				if (valDCita == true) {
					if (GL_validar) {
						if (strcmp(fechaC, tempo.fechaCita) == 0 || strcmp(horaC, tempo.horaCita) == 0) {
							GL_validar = false;
							MessageBox(hWnd, "No se puede hacer una segunda cita en el mismo dia o la misma hora", "Fecha cita", MB_OK);
						}
					}
				}
				
				if (GL_validar) {
					if (IsDlgButtonChecked(hWnd, IDC_RDNO) == BST_CHECKED) valDobleCita = false;
 					else if (IsDlgButtonChecked(hWnd, IDC_RDSI) == BST_CHECKED) valDobleCita = true;
					else {
						MessageBox(hWnd, "No elegiste si haras una cita doble o no", "Alerta", MB_OK);
						GL_validar = false;
					}
				}

				if (GL_validar) {

					if (aceptarB) {
						GL_listaCitas.agregarNodos(tempo);
						MessageBox(hWnd, "Se agrego nueva Cita", "Cita", MB_OK);
						
					}
					else {

						auxi->copiardatos(tempo);
						aceptarB = true;
					}

					if (valDobleCita == true) {

						valDCita = true;
						numeCita++;
						_itoa_s(numeCita, numCita, 10);
						
						strcpy(fechaC, tempo.fechaCita);
						strcpy(horaC, tempo.horaCita);
						SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)numCita);

						//SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_CBHORA, CB_SETCURSEL, -1, 0);
						SendDlgItemMessage(hWnd, IDC_CBMINUTOS, CB_SETCURSEL, -1, 0);
						SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_FECHACITAACT, WM_SETTEXT, 0, (LPARAM)tempo.fechaCita);
						SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_RESETCONTENT, 0, 0);//recargar lisbox
						GL_listaCitas.llenarLByCB(hWnd, IDC_LBCITASREG, LB_ADDSTRING, LB_SETITEMDATA);//recargar lisbox
			
					}
					else if (valDobleCita == false){

						//inicio limpieza
						SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTECITA, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_CELULARCLCITA, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_FECHACITAACT, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_TNMECANICO, WM_SETTEXT, 0, (LPARAM)"");
						SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, CB_SETCURSEL, -1, 0);
						SendDlgItemMessage(hWnd, IDC_CBHORA, CB_SETCURSEL, -1, 0);
						SendDlgItemMessage(hWnd, IDC_CBMINUTOS, CB_SETCURSEL, -1, 0);
						SendDlgItemMessage(hWnd, IDC_LUNESC, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_MARTESC, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_MIERCOLESC, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_JUEVESC, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_VIERNESC, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);
						SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_RESETCONTENT, 0, 0); //reset listbox
						SendDlgItemMessage(hWnd, IDC_LBCLIENTES, LB_RESETCONTENT, 0, 0); 
						GL_listaCliente.llenarLByCB(hWnd, IDC_LBCLIENTES, LB_ADDSTRING, LB_SETITEMDATA);
						SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_RESETCONTENT, 0, 0);//recargar lisbox
						GL_listaCitas.llenarLByCB(hWnd, IDC_LBCITASREG, LB_ADDSTRING, LB_SETITEMDATA);//recargar lisbox

						numeCita++;
						_itoa_s(numeCita, numCita, 10);
						SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)numCita);
						
						valDCita = false;

						bmp = (HBITMAP)LoadImage(NULL, temp.chFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						SendDlgItemMessage(hWnd, IDC_BITMAPMEC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
						ShowWindow(GetDlgItem(hWnd, IDC_BITMAPMEC), false);
						
					}
				}
				else MessageBox(hWnd, "No c agrego nada", "Alerta", MB_OK);
				valCitas = true;
			}
			else MessageBox(hWnd, "No elegiste nada para registrar cita", "Alerta", MB_OK);
		}

		if (LOWORD(wParam) == IDC_BTNLIMPIARCITA && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTECITA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_CELULARCLCITA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_FECHACITAACT, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_TNMECANICO, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, IDC_CBHORA, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, IDC_CBMINUTOS, CB_SETCURSEL, -1, 0);
			SendDlgItemMessage(hWnd, IDC_LUNESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_MARTESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_MIERCOLESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_JUEVESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_VIERNESC, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_LBMECANICOCITA, LB_RESETCONTENT, 0, 0); //reset listbox
			SendDlgItemMessage(hWnd, IDC_LBCLIENTES, LB_RESETCONTENT, 0, 0);
			GL_listaCliente.llenarLByCB(hWnd, IDC_LBCLIENTES, LB_ADDSTRING, LB_SETITEMDATA);
			SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_RESETCONTENT, 0, 0);//recargar lisbox
			GL_listaCitas.llenarLByCB(hWnd, IDC_LBCITASREG, LB_ADDSTRING, LB_SETITEMDATA);//recargar lisbox
			_itoa_s(numeCita, numCita, 10);
			SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)numCita);
			aceptarB = true;
			valCitas = false;

			bmp = (HBITMAP)LoadImage(NULL, temp.chFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, IDC_BITMAPMEC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
			ShowWindow(GetDlgItem(hWnd, IDC_BITMAPMEC), false);

		}
		if (LOWORD(wParam) == IDC_BTNMODCITA && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_GETITEMDATA, cursel, 0);
				auxi = GL_listaCitas.buscar(nuevaClave);

				SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_SETTEXT, 0, (LPARAM)auxi->datos.nomMecanico);
				SendDlgItemMessage(hWnd, IDC_NOMBRECLIENTECITA, WM_SETTEXT, 0, (LPARAM)auxi->datos.nombre);
				SendDlgItemMessage(hWnd, IDC_CELULARCLCITA, WM_SETTEXT, 0, (LPARAM)auxi->datos.celularCliente);
				SendDlgItemMessage(hWnd, IDC_CLAVEDCITA, WM_SETTEXT, 0, (LPARAM)auxi->datos.claveP);
				SendDlgItemMessage(hWnd, IDC_FECHACITAACT, WM_SETTEXT, 0, (LPARAM)auxi->datos.fechaCita);
				SendDlgItemMessage(hWnd, IDC_TNMECANICO, WM_SETTEXT, 0, (LPARAM)auxi->datos.turnoTC);
				//SendDlgItemMessage(hWnd, IDC_CBESPECIALIDAD, CB_SETCURSEL, -1, (LPARAM)auxi->datos.especialidadCita);
				SendDlgItemMessage(hWnd, IDC_CBHORA, CB_SELECTSTRING, -1, (LPARAM)auxi->datos.horaCita);
				SendDlgItemMessage(hWnd, IDC_CBMINUTOS, CB_SELECTSTRING, -1, (LPARAM)auxi->datos.minutoCita);

				if (auxi->datos.LunesC == 1) SendDlgItemMessage(hWnd, IDC_LUNESC, BM_SETCHECK, BST_CHECKED, 0);
				if (auxi->datos.MartesC == 1) SendDlgItemMessage(hWnd, IDC_MARTESC, BM_SETCHECK, BST_CHECKED, 0);
				if (auxi->datos.MiercolesC == 1) SendDlgItemMessage(hWnd, IDC_MIERCOLESC, BM_SETCHECK, BST_CHECKED, 0);
				if (auxi->datos.JuevesC == 1) SendDlgItemMessage(hWnd, IDC_JUEVESC, BM_SETCHECK, BST_CHECKED, 0);
				if (auxi->datos.ViernesC == 1) SendDlgItemMessage(hWnd, IDC_VIERNESC, BM_SETCHECK, BST_CHECKED, 0);

				bmp = (HBITMAP)LoadImage(NULL, auxi->datos.imagenMec, IMAGE_BITMAP, 100, 120, LR_LOADFROMFILE);
				strcpy(tempo.imagenMec, auxi->datos.imagenMec);
				strcpy(tempo.claveMec, auxi->datos.claveMec);
				strcpy(tempo.especialidadCita, auxi->datos.especialidadCita);
				SendDlgItemMessage(hWnd, IDC_BITMAPMEC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				ShowWindow(GetDlgItem(hWnd, IDC_BITMAPMEC), true);

				valCitas = true;
				aceptarB = false;
			}
			else {

				MessageBox(hWnd, "Elija una opcion del listbox", "Especialidad", MB_OK);
			}
			return true;
		}

		if (LOWORD(wParam) == IDC_BTNELIMINARCITA && HIWORD(wParam) == BN_CLICKED) {

			int posicionLBCB = 0, claveTempo = 0;
			//obtener la posicion de lo que se tiene seleccionao
			posicionLBCB = SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_GETCURSEL, 0, 0);

			if (posicionLBCB != -1) {
				claveTempo = SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_GETITEMDATA, (WPARAM)posicionLBCB, 0);
				nodo <Citas>* tempoBorrar;
				//en tempoBorrar se reserva la direccion del nodo que se busco con la clave
				tempoBorrar = GL_listaCitas.buscar(claveTempo);

				GL_listaCitas.eliminarNodos(tempoBorrar);

				SendDlgItemMessage(hWnd, IDC_LBCITASREG, LB_RESETCONTENT, 0, 0);
				GL_listaCitas.llenarLByCB(hWnd, IDC_LBCITASREG, LB_ADDSTRING, LB_SETITEMDATA);
			}
			else {

				MessageBox(hWnd, "Elija una opcion del listbox", "Citas", MB_OK);
			}
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK ReporteCitas(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static nodo<Citas>* auxi = NULL;
	char horaReporte[MAX_PATH] = "", cancelacionCita[MAX_PATH] = "";
	bool cancelCita = false, noCancel = false;

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMBREUUSUARIO);
		SetWindowText(Nombre, login.nombreUsuario);

	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNSELREPORTE && HIWORD(wParam) == BN_CLICKED) {
			
			strcpy(horaReporte, auxiliarRep->datos.horaCita);
			strcat(horaReporte, " : ");
			strcat(horaReporte, auxiliarRep->datos.minutoCita);

			SendDlgItemMessage(hWnd, IDC_FECHAREP, WM_SETTEXT, 0, (LPARAM)auxiliarRep->datos.fechaCita);
			SendDlgItemMessage(hWnd, IDC_HORAREP, WM_SETTEXT, 0, (LPARAM)horaReporte);
			SendDlgItemMessage(hWnd, IDC_ESPREPORTE, WM_SETTEXT, 0, (LPARAM)auxiliarRep->datos.especialidadCita);
			SendDlgItemMessage(hWnd, IDC_NOMMECREP, WM_SETTEXT, 0, (LPARAM)auxiliarRep->datos.nomMecanico);
			SendDlgItemMessage(hWnd, IDC_NOMCLREP, WM_SETTEXT, 0, (LPARAM)auxiliarRep->datos.nombre);
			SendDlgItemMessage(hWnd, IDC_CELREPORTE, WM_SETTEXT, 0, (LPARAM)auxiliarRep->datos.celularCliente);

			bmp = (HBITMAP)LoadImage(NULL, auxiliarRep->datos.imagenMec, IMAGE_BITMAP, 100, 120, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, IDC_FOTOREPORTE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			ShowWindow(GetDlgItem(hWnd, IDC_FOTOREPORTE), true);

		}
		if (LOWORD(wParam) == IDC_BTNLIMPIARREPO && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_FECHAREP, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_HORAREP, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_ESPREPORTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_NOMMECREP, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_NOMCLREP, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_CELREPORTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_MOTIVOCANCEL, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);

			SendDlgItemMessage(hWnd, IDC_FOTOREPORTE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
			ShowWindow(GetDlgItem(hWnd, IDC_FOTOREPORTE), false);

		}
		if (LOWORD(wParam) == IDC_BTNCANCELREP && HIWORD(wParam) == BN_CLICKED) {

			if (IsDlgButtonChecked(hWnd, IDC_CITASI) == BST_CHECKED) cancelCita = true;
			else if (IsDlgButtonChecked(hWnd, IDC_CITANO) == BST_CHECKED) cancelCita = false;

			string cancelacion;
			SendDlgItemMessage(hWnd, IDC_MOTIVOCANCEL, WM_GETTEXT, sizeof(auxiliarRep->datos.cancelarCita), (LPARAM)auxiliarRep->datos.cancelarCita);
			cancelacion = auxiliarRep->datos.cancelarCita;
			if (cancelacion.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en la sección ""Razon de cancelacion""", "Reporte Citas", MB_OK);
			}
			
			if (cancelacion.size() != 0 && cancelCita == true) {
				int posicionLBCB = 0, claveTempo = 0;
				//obtener la posicion de lo que se tiene seleccionao
				posicionLBCB = SendDlgItemMessage(hWnd, IDC_CBCLAVCITA, CB_GETCURSEL, 0, 0);

				if (posicionLBCB != -1) {
					
					nodo <Citas>* tempoBorrar;
					//en tempoBorrar se reserva la direccion del nodo que se busco con la clave
					tempoBorrar = auxiliarRep;
					GL_listaCitas.eliminarNodos(tempoBorrar);

					SendDlgItemMessage(hWnd, IDC_FECHAREP, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_HORAREP, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_ESPREPORTE, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_NOMMECREP, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_NOMCLREP, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_CELREPORTE, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_MOTIVOCANCEL, WM_SETTEXT, 0, (LPARAM)"");
					SendDlgItemMessage(hWnd, IDC_RDSI, BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hWnd, IDC_RDNO, BM_SETCHECK, BST_UNCHECKED, 0);

					SendDlgItemMessage(hWnd, IDC_FOTOREPORTE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)"");
					ShowWindow(GetDlgItem(hWnd, IDC_FOTOREPORTE), false);

					SendDlgItemMessage(hWnd, IDC_LBCLAVCITA, LB_RESETCONTENT, 0, 0);
					GL_listaCitas.llenarCBP(hWnd, IDC_LBCLAVCITA, LB_ADDSTRING, LB_SETITEMDATA);
				}
				else {

					MessageBox(hWnd, "No elegiste ninguna cita", "Citas", MB_OK);
				}
			}
			else MessageBox(hWnd, "No eligio si queria cancelarla con justificacion", "ALERTA", MB_OK);
			
		}
		if (LOWORD(wParam) == IDC_BTNGUARDAR && HIWORD(wParam) == BN_CLICKED) {

			if (IsDlgButtonChecked(hWnd, IDC_CITASI) == BST_CHECKED) cancelCita = true;
			else if (IsDlgButtonChecked(hWnd, IDC_CITANO) == BST_CHECKED) cancelCita = false;
			else noCancel = true;

			string cancelacion;
			SendDlgItemMessage(hWnd, IDC_MOTIVOCANCEL, WM_GETTEXT, sizeof(auxiliarRep->datos.cancelarCita), (LPARAM)auxiliarRep->datos.cancelarCita);
			cancelacion = auxiliarRep->datos.cancelarCita;
			if (cancelacion.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en la sección ""Razon de cancelacion""", "Reporte Citas", MB_OK);
			}

			if (cancelacion.size() != 0 && noCancel == false) {
				GL_listaCitas.guardarEnTxt(hWnd, auxiliarRep->datos.claveP, cancelCita);
				MessageBox(hWnd, "Guardado exitoso", "Reporte Citas", MB_OK);
				noCancel = false;
			}
			else {
				MessageBox(hWnd, "No se coloco nada en la sección ""Razon de cancelacion"" o no elegiste si cancelar o no", "Reporte Citas", MB_OK);
				noCancel = false;
			}
		}
		
		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK CitasxMecanico(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	strcpy(mesesito, "");

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMUS);
		SetWindowText(Nombre, login.nombreUsuario);

		HWND Hora_Combo2 = GetDlgItem(hWnd, IDC_CBMESXMECANICO);
		char Horas2[12][MAX_PATH]{ "enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre" };
		for (int j = 0; j < 12; j++) {
			SendMessage(Hora_Combo2, CB_ADDSTRING, NULL, (LPARAM)Horas2[j]);
		}

		GL_ArbolMecanicos.LlenarNumEmp(hWnd, IDC_CBNUMNOM, CB_ADDSTRING, CB_SETITEMDATA, GL_ArbolMecanicos.raiz);
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNELEGIRMES && HIWORD(wParam) == BN_CLICKED) {

			string numeroEmpleado;
			SendDlgItemMessage(hWnd, IDC_NUMNOMXMEC, WM_GETTEXT, sizeof(busqBin), (LPARAM)busqBin);

			numeroEmpleado = busqBin;
			if (numeroEmpleado.size() == 0) {
				MessageBox(hWnd, "No se coloco nada en la sección ""Numero de Empleado""", "Numero del Mecanico", MB_OK);
				GL_validar = false;
			}
			else GL_validar = true;

			if(GL_validar == true){

				char buffer[MAX_PATH] = "";
				agregarOrdenar(GL_ArbolMecanicos.raiz);
				quickSort(inicio, final);
				if (binarySearch(busqBin) == NULL) {

					MessageBox(hWnd, "No existe ese numero de empleado", "Alerta", MB_OK);
				}
				else {
					char buffer[MAX_PATH] = "";
					strcpy(buffer, auxi->apPaterno);
					strcat(buffer, " ");
					strcat(buffer, auxi->nombre);
					
					SendDlgItemMessage(hWnd, IDC_NUMXMECANICO, WM_SETTEXT, 0, (LPARAM)auxi->numeroEmpleado);
					SendDlgItemMessage(hWnd, IDC_NOMMECANICOCITA, WM_SETTEXT, 0, (LPARAM)buffer);
					SendDlgItemMessage(hWnd, IDC_PAQUETECITAMEC, WM_SETTEXT, 0, (LPARAM)auxi->especialidad);
				}

			}
		}
		if (LOWORD(wParam) == IDC_BUSCARCITAXMESMEC && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_LBCITASXMEC, LB_RESETCONTENT, 0, 0);

			string numerito;
			SendDlgItemMessage(hWnd, IDC_NUMXMECANICO, WM_GETTEXT, sizeof(numMecBin), (LPARAM)numMecBin);
			numerito = numMecBin;

			SendDlgItemMessage(hWnd, IDC_CBMESXMECANICO, WM_GETTEXT, sizeof(mesesito), (LPARAM)mesesito);
			if (SendDlgItemMessage(hWnd, IDC_CBMESXMECANICO, CB_GETCURSEL, 0, 0) == -1) {
				MessageBox(hWnd, "No se ha colocado un Mes en la sección", "Mecanico y Mes", MB_OK);
				GL_validar = false;
			}
			else GL_validar = true;

			if (numerito.size() != 0 && GL_validar == true) {

				GL_listaCitas.mecanicoMes(hWnd, IDC_LBCITASXMEC, LB_ADDSTRING, LB_SETITEMDATA);
			}
			else MessageBox(hWnd, "No elegiste algo del combobox y no has elegido un empleado", "Alerta", MB_OK);
			
		}
		if (LOWORD(wParam) == IDC_BTNCITAXMECANICO && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBCITASXMEC, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCITASXMEC, LB_GETITEMDATA, cursel, 0);
				auxiliarRep = GL_listaCitas.buscar(nuevaClave);

				HWND RepCit = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
				ShowWindow(RepCit, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else MessageBox(hWnd, "No elegiste nada de la listbox", "Mes y Especialidad", MB_OK);

		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK CitasxNumero(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMDUSURU);
		SetWindowText(Nombre, login.nombreUsuario);

		GL_listaCitas.llenarCBP(hWnd, IDC_LBCONSXNUM, LB_ADDSTRING, LB_SETITEMDATA);

	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNCONSULXNUM && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBCONSXNUM, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCONSXNUM, LB_GETITEMDATA, cursel, 0);
				auxiliarRep = GL_listaCitas.buscar(nuevaClave);

				HWND RepCit = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
				ShowWindow(RepCit, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else MessageBox(hWnd, "No elegiste nada de la listbox", "Mes y Semana", MB_OK);

		}
		
		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK CitasxPaquete(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	strcpy(mesesito, "");

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMUSU);
		SetWindowText(Nombre, login.nombreUsuario);

		GL_listaPaquetes.llenarCBP(hWnd, IDC_CBCITASXPAQUETE, CB_ADDSTRING, CB_SETITEMDATA);

		GL_listaPaquetes.llenarLByCB(hWnd, IDC_LBPAQ, LB_ADDSTRING, LB_SETITEMDATA);

		HWND Hora_Combo2 = GetDlgItem(hWnd, IDC_CBMESCITAXP);
		char Horas2[12][MAX_PATH]{ "enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre" };
		for (int j = 0; j < 12; j++) {
			SendMessage(Hora_Combo2, CB_ADDSTRING, NULL, (LPARAM)Horas2[j]);

		}
		HWND Semana_Combo2 = GetDlgItem(hWnd, IDC_CBSEMXPAQ);
		char Semana2[12][MAX_PATH]{ "1","2","3","4","5" };
		for (int j = 0; j < 5; j++) {
			SendMessage(Semana_Combo2, CB_ADDSTRING, NULL, (LPARAM)Semana2[j]);

		}
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNCITASXPAQUETE && HIWORD(wParam) == BN_CLICKED) {

			SendDlgItemMessage(hWnd, IDC_LBCITASXPAQUETE, LB_RESETCONTENT, 0, 0);
			SendDlgItemMessage(hWnd, IDC_CBMESCITAXP, WM_GETTEXT, sizeof(mesesito), (LPARAM)mesesito);

			if (SendDlgItemMessage(hWnd, IDC_CBMESCITAXP, CB_GETCURSEL, 0, 0) == -1) {
				MessageBox(hWnd, "No se ha colocado un Mes en la sección", "Mes y Especialidad", MB_OK);
				GL_validar = false;
			}
			else GL_validar = true;

			if (GL_validar) {

				SendDlgItemMessage(hWnd, IDC_CBSEMXPAQ, WM_GETTEXT, sizeof(semanita), (LPARAM)semanita);

				if (SendDlgItemMessage(hWnd, IDC_CBSEMXPAQ, CB_GETCURSEL, 0, 0) == -1) {
					MessageBox(hWnd, "No se ha colocado un Semana en la sección", "Mes y Especialidad", MB_OK);
					GL_validar = false;
				}
				else GL_validar = true;
			}

			if (GL_validar) {

				SendDlgItemMessage(hWnd, IDC_CBCITASXPAQUETE, WM_GETTEXT, sizeof(especialidadcita), (LPARAM)especialidadcita);

				if (SendDlgItemMessage(hWnd, IDC_CBCITASXPAQUETE, CB_GETCURSEL, 0, 0) == -1) {
					MessageBox(hWnd, "No se ha colocado una Especialidad en la sección", "Mes y Especialidad", MB_OK);
					GL_validar = false;
				}
				else GL_validar = true;
			}

			if (GL_validar == true) {

				GL_listaCitas.semanaMesEspecialidad(hWnd, IDC_LBCITASXPAQUETE, LB_ADDSTRING, LB_SETITEMDATA);
			}
		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {

			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBCITASXPAQUETE, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCITASXPAQUETE, LB_GETITEMDATA, cursel, 0);
				auxiliarRep = GL_listaCitas.buscar(nuevaClave);

				HWND RepCit = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
				ShowWindow(RepCit, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else MessageBox(hWnd, "No elegiste nada de la listbox", "Mes y Especialidad", MB_OK);
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK CitasxSemana(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMUSUR);
		SetWindowText(Nombre, login.nombreUsuario);

		HWND Hora_Combo2 = GetDlgItem(hWnd, IDC_CBMESREP);
		char Horas2[12][MAX_PATH]{ "enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre" };
		for (int j = 0; j < 12; j++) {
			SendMessage(Hora_Combo2, CB_ADDSTRING, NULL, (LPARAM)Horas2[j]);
		}

		HWND Semana_Combo2 = GetDlgItem(hWnd, IDC_CBSEMREP);
		char Semana2[12][MAX_PATH]{ "1","2","3","4","5" };
		for (int j = 0; j < 5; j++) {
			SendMessage(Semana_Combo2, CB_ADDSTRING, NULL, (LPARAM)Semana2[j]);

		}
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_REGISTROPAQ && HIWORD(wParam) == BN_CLICKED) {
			HWND RegPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROPAQUETE), NULL, RegistroPaquetes);
			ShowWindow(RegPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODEMECANICOS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORMECANICO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasMecanico = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXMECANICO), NULL, CitasxMecanico);
			ShowWindow(CitasMecanico, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORSEMANA && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasSemana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXSEMANA), NULL, CitasxSemana);
			ShowWindow(CitasSemana, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORPAQUETEDESERVICIO && HIWORD(wParam) == BN_CLICKED) {
			HWND CitasPaquete = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXPAQUETE), NULL, CitasxPaquete);
			ShowWindow(CitasPaquete, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECLIENTES && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_REGISTRODECITAS && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaCliente.primero != 0 && GL_listaPaquetes.primero != 0 && GL_ArbolMecanicos.raiz != 0) {
				HWND Citas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITAS), NULL, RegistroCitas);
				ShowWindow(Citas, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "Te falta agregar minimo un Paquete, un Mecanico y un Cliente", "Csm", MB_OK);
			}
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_REPORTEDECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND RepCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
			ShowWindow(RepCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_CONSULTASDECITAS_PORNUMERODECITAS && HIWORD(wParam) == BN_CLICKED) {
			HWND NumCitas = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CITASXNUMERO), NULL, CitasxNumero);
			ShowWindow(NumCitas, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
			HWND SalirP = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_SALIR), NULL, Salida);
			ShowWindow(SalirP, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		if (LOWORD(wParam) == IDC_BTNCITASXMES && HIWORD(wParam) == BN_CLICKED) {
		
			SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_RESETCONTENT, 0, 0);
			SendDlgItemMessage(hWnd, IDC_CBMESREP, WM_GETTEXT, sizeof(mesesito), (LPARAM)mesesito);

			if (SendDlgItemMessage(hWnd, IDC_CBMESREP, CB_GETCURSEL, 0, 0) == -1) {
				MessageBox(hWnd, "No se ha colocado un Mes en la sección", "Mes y Semana", MB_OK);
				GL_validar = false;
			}
			else GL_validar = true;
			
			
			if (GL_validar) { 

				SendDlgItemMessage(hWnd, IDC_CBSEMREP, WM_GETTEXT, sizeof(semanita), (LPARAM)semanita);

				if (SendDlgItemMessage(hWnd, IDC_CBSEMREP, CB_GETCURSEL, 0, 0) == -1) {
					MessageBox(hWnd, "No se ha colocado un Semana en la sección", "Mes y Semana", MB_OK);
					GL_validar = false;
				}
				else GL_validar = true;
			}

			if (GL_validar == true) {

				GL_listaCitas.semanaMes(hWnd, IDC_LBCITASXMES, LB_ADDSTRING, LB_SETITEMDATA);
			}

		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
		
			int cursel = 0, nuevaClave = 0;
			cursel = SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_GETCURSEL, 0, 0);
			if (cursel != -1) {
				nuevaClave = SendDlgItemMessage(hWnd, IDC_LBCITASXMES, LB_GETITEMDATA, cursel, 0);
				auxiliarRep = GL_listaCitas.buscar(nuevaClave);

				HWND RepCit = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REPORTECITAS), NULL, ReporteCitas);
				ShowWindow(RepCit, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else MessageBox(hWnd, "No elegiste nada de la listbox", "Mes y Semana", MB_OK);
		}
		
		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK ReporteClientes(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_USUARIONOMR);
		SetWindowText(Nombre, login.nombreUsuario);

		SendDlgItemMessage(hWnd, IDC_LBCLIENTESR, LB_RESETCONTENT, -1, 0);

	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BTNREGRESAR && HIWORD(wParam) == BN_CLICKED) {
			HWND Cliente = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_CLIENTES), NULL, Clientes);
			ShowWindow(Cliente, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}
		if (LOWORD(wParam) == ID_BTNVER && HIWORD(wParam) == BN_CLICKED) {
			OrdenarHeap(hWnd);
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK ReporteMecanicos(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMDEUSUR);
		SetWindowText(Nombre, login.nombreUsuario);

		SendDlgItemMessage(hWnd, IDC_LBREPMEC, LB_RESETCONTENT, -1, 0);

	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BTNREGMEC && HIWORD(wParam) == BN_CLICKED) {
			if (GL_listaPaquetes.primero != 0) {
				HWND RegMecanicos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_REGISTROMECANICOS), NULL, RegistroMecanicos);
				ShowWindow(RegMecanicos, SW_SHOW);
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				MessageBox(hWnd, "No has metido ningun Paquete", "Alerta", MB_OK);
			}
		}
		if (LOWORD(wParam) == IDC_BTNREPMEC && HIWORD(wParam) == BN_CLICKED) {
			
			agregarOrdenar(GL_ArbolMecanicos.raiz);
			quickSort(inicio, final);

			Mecanicos* tempAux;

			aux = inicio;
			char txt[MAX_PATH] = "";
			GetCurrentDirectory(MAX_PATH, txt);
			strcat(txt, "\\Reporte de Mecanicos.txt");
			ofstream archivo;
			archivo.open(txt);
			if (archivo.is_open())
			{
				while (aux != 0)
				{
					char bufer[MAX_PATH] = "", numero[10] = "", demas[MAX_PATH] = "";

					strcpy(bufer, aux->numeroEmpleado);
					strcat(bufer, " - ");
					strcat(bufer, aux->apPaterno);
					strcat(bufer, " ");
					strcat(bufer, aux->apMaterno);
					strcat(bufer, " ");
					strcat(bufer, aux->nombre);

					SendDlgItemMessage(hWnd, IDC_LBREPMEC, LB_ADDSTRING, 0, (LPARAM)bufer);

					strcat(bufer, " ");
					strcat(bufer, aux->numeroEmpleado);
					strcat(bufer, " ");
					strcat(demas, aux->telefono);
					strcat(demas, " ");
					strcat(demas, aux->especialidad);
					strcat(demas, " ");
					strcat(demas, aux->turnoTrabajo);
					strcat(demas, " ");
					strcat(demas, aux->chFileName);

					archivo << bufer << demas << "\n";

					tempAux = aux;
					aux = aux->derecha;

					delete tempAux;
				}

				archivo.close();
				inicio = 0;
				final = 0;
				aux = 0;

			}
		}

		break;
	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK Salida(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {

		HWND Nombre = GetDlgItem(hWnd, IDC_NOMU);
		SetWindowText(Nombre, login.nombreUsuario);
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == BTN_SI && HIWORD(wParam) == BN_CLICKED) {

			DestroyWindow(hWnd);
		}

		if (LOWORD(wParam) == BTN_NO && HIWORD(wParam) == BN_CLICKED) {
			HWND VenPrin = CreateDialog(hGlobalInst, MAKEINTRESOURCE(IDD_VENPRIN), NULL, VentanaPrincipal);
			ShowWindow(VenPrin, SW_SHOW);
			ShowWindow(hWnd, SW_HIDE);
		}

		break;

	case WM_CLOSE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

Mecanicos* middle(Mecanicos* start, Mecanicos* last) {
	if (start == NULL)
		return NULL;

	Mecanicos* primero = start;
	Mecanicos* siguiente = start->derecha;

	while (siguiente != last)
	{
		siguiente = siguiente->derecha;
		if (siguiente != last)
		{
			primero = primero->derecha;
			siguiente = siguiente->derecha;
		}
	}

	return primero;
}

Mecanicos* binarySearch(char value[MAX_PATH])
{
	Mecanicos* inicioM = inicio;
	Mecanicos* ultimoM = NULL;

	do
	{
		auxi = middle(inicioM, ultimoM);

		if (auxi == NULL)
			return NULL;

		if (atoi(auxi->numeroEmpleado) == atoi(value))
			return auxi;

		else if (atoi(auxi->numeroEmpleado) < atoi(value)) inicioM = auxi->derecha;

		else ultimoM = auxi;

	} while (ultimoM == NULL || ultimoM != inicioM);

	return NULL;
}

void agregarOrdenar(Mecanicos* r) {
	
	if (r != 0) {
		//arreglo[buscar] = *r;

		aux = new Mecanicos(*r);

		if (inicio == 0) {
			inicio = aux;
			final = aux;

			aux->izquierda = 0;
			aux->derecha = 0;
		}

		else {

			//lo enlaza
			final->derecha = aux;
			aux->izquierda = final;
			final = aux;

			final->derecha = 0;
		}

		//buscar++;
		agregarOrdenar(r->derecha);
		agregarOrdenar(r->izquierda);
	}
}

Mecanicos* partition(Mecanicos* first, Mecanicos* last) {

	Mecanicos* i = first->izquierda;
	for (Mecanicos* j = first; j != last; j = j->derecha) {
		if (j != NULL) {
			if (atoi(j->numeroEmpleado) <= atoi(last->numeroEmpleado)) {
				i = (i == NULL) ? first : i->derecha;
				swap2(i, j);
			}
		}
	}
	i = (i == NULL) ? first : i->derecha;
	swap2(i, last);

	return i;
}

void quickSort(Mecanicos* first, Mecanicos* last) {
	if (last != NULL && first != last && first != NULL && first != last->derecha) {
		Mecanicos* pivot = partition(first, last);
		quickSort(first, pivot->izquierda);
		quickSort(pivot->derecha, last);
	}
}

void swap2(Mecanicos* xp, Mecanicos* yp) {
	Mecanicos temp;

	strcpy(temp.nombre, xp->nombre);
	strcpy(temp.apPaterno, xp->apPaterno);
	strcpy(temp.apMaterno, xp->apMaterno);
	strcpy(temp.telefono, xp->telefono);
	strcpy(temp.especialidad, xp->especialidad);
	strcpy(temp.numeroEmpleado, xp->numeroEmpleado);
	strcpy(temp.turnoTrabajo, xp->turnoTrabajo);
	strcpy(temp.chFileName, xp->chFileName);
	temp.Lunes = xp->Lunes;
	temp.Martes = xp->Martes;
	temp.Miercoles = xp->Miercoles;
	temp.Jueves = xp->Jueves;
	temp.Viernes = xp->Viernes;

	strcpy(xp->nombre, yp->nombre);
	strcpy(xp->apPaterno, yp->apPaterno);
	strcpy(xp->apMaterno, yp->apMaterno);
	strcpy(xp->telefono, yp->telefono);
	strcpy(xp->especialidad, yp->especialidad);
	strcpy(xp->numeroEmpleado, yp->numeroEmpleado);
	strcpy(xp->turnoTrabajo, yp->turnoTrabajo);
	strcpy(xp->chFileName, yp->chFileName);
	xp->Lunes = yp->Lunes;
	xp->Martes = yp->Martes;
	xp->Miercoles = yp->Miercoles;
	xp->Jueves = yp->Jueves;
	xp->Viernes = yp->Viernes;

	strcpy(yp->nombre, temp.nombre);
	strcpy(yp->apPaterno, temp.apPaterno);
	strcpy(yp->apMaterno, temp.apMaterno);
	strcpy(yp->telefono, temp.telefono);
	strcpy(yp->especialidad, temp.especialidad);
	strcpy(yp->numeroEmpleado, temp.numeroEmpleado);
	strcpy(yp->turnoTrabajo, temp.turnoTrabajo);
	strcpy(yp->chFileName, temp.chFileName);
	yp->Lunes = temp.Lunes;
	yp->Martes = temp.Martes;
	yp->Miercoles = temp.Miercoles;
	yp->Jueves = temp.Jueves;
	yp->Viernes = temp.Viernes;

}
