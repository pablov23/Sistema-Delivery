#include<iostream>
#include<string.h>

using namespace std;

struct Pedido {
    string domicilio;
    unsigned zona;
    string comercio;
    unsigned rubro;
    unsigned importe;
};
struct Comercio{
    string nombre;
    unsigned zona;
};
struct NodoPedido{
    Pedido dato;
    NodoPedido* sig;
};
struct ColaPedidos{
    NodoPedido* pri;
    NodoPedido* ult;
};
struct Repartidor
{
    int dni;
    char nombre[20];
    char apellido[20];
    unsigned zona;
};
struct NodoRepartidor
{
    Repartidor dato;
    Pedido *sig;
};

const unsigned CANT_ZONAS = 6;

void recibirPedido(FILE* archivos[4], ColaPedidos* colas[CANT_ZONAS]);
void encolarPedido(ColaPedidos* cola, Pedido pedido);
int getZonaComercio(FILE* archivo, string nombre);
Pedido ingresarDatosPedido();
void asignarPedidos();
int getZonaRepartidor(string nombreRep);
void mostrar();
void salir();

int main(){
    // Abro los archivos y verifico
    FILE* archivos[4] = {fopen("Pizzerias.dat", "rb"), fopen("Heladerias.dat", "rb"), fopen("Bebidas.dat", "rb"), fopen("Parrillas.dat", "rb")};
    if (archivos[0]==NULL || archivos[1]==NULL || archivos[2]==NULL || archivos[3]==NULL) return 1;
    // Creo un vector con colas (uno para cada zona), y lo inicializo
    ColaPedidos* colas[CANT_ZONAS];
    for (unsigned i = 0; i<CANT_ZONAS; i++) colas[i] = new ColaPedidos;
    // Le muestro un menu de opciones al usuario
    unsigned opcion = 1;
    while (opcion >= 1 && opcion <=3)
    {
        cout<<"- Menu ----------------------------------------------------------"<<endl;
        cout<<"1: Recibir un pedido / 2: Asignar pedidos / 3: Mostrar / 4: Salir"<<endl;
        cout<<"Ingrese un numero del 1 al 4: ";
        cin>>opcion;
        switch (opcion){
            case 1: recibirPedido(archivos, colas); break;
            case 2: asignarPedidos(); break;
            case 3: mostrar(); break;
            case 4: salir(); break;
            default: break;
        }
    }
    // Cierro todos los archivos
    fclose(archivos[0]); fclose(archivos[1]); fclose(archivos[2]); fclose(archivos[3]);
    return 0;
}

// Ingresa un nuevo pedido
void recibirPedido(FILE* archivos[4], ColaPedidos* colas[CANT_ZONAS]){
    Pedido p = ingresarDatosPedido();
    int zona = getZonaComercio(archivos[p.rubro-1], p.comercio);
    cout<<"-----------------------------------------------------------------"<<endl;
    if (zona == p.zona) {
        encolarPedido(colas[zona-1], p);
        cout<<"Pedido recibido!"<<endl;
    } else {
        cout<<"Error: el comercio no esta inscripto. Ingrese nuevamente los datos."<<endl;
    }
}

// Inserta un pedido en una cola
void encolarPedido(ColaPedidos* cola, Pedido pedido){
    NodoPedido* p;
    p = new NodoPedido;
    p->dato = pedido;
    p->sig = NULL;
    if (cola->pri == NULL) cola->pri = p;
    else cola->ult->sig = p;
    cola->ult = p;
}

// Ingreso de datos para un nuevo pedido
Pedido ingresarDatosPedido(){
    Pedido p;
    cout<<"Ingrese los siguientes datos:"<<endl<<"Domicilio: ";
    cin>>p.domicilio;
    cout<<"Zona: ";
    cin>>p.zona;
    cout<<"Comercio: ";
    cin>>p.comercio;
    cout<<"Rubro (1=Pizzeria 2=Heladeria 3=Bebidas 4=Parrilla): ";
    cin>>p.rubro;
    cout<<"Importe: ";
    cin>>p.importe; 
    return p;
}

// Busca un comercio en un archivo (con busqueda binaria) y devuelve su zona
int getZonaComercio(FILE* archivo, string nombre){
    Comercio c;
    unsigned desde = 0, hasta, medio;
    fseek(archivo,0,SEEK_END);
    hasta = ftell(archivo)/sizeof(Comercio);
    while(desde <= hasta)
    {
        medio = (desde + hasta) / 2;
        fseek(archivo, medio*sizeof(Comercio), SEEK_SET);
        fread(&c, sizeof(Comercio), 1, archivo);
        // La funcion .compare devuelve -1, 0, ó 1 dependiento el orden alfabetico
        if (nombre.compare(c.nombre) == 0) return c.zona;
        else if (nombre.compare(c.nombre) < 0) hasta=medio-1;
        else desde=medio+1;
    }
    return -1;
}

// Asigna pedidos a un repartidor
void asignarPedidos(){
    string nombreRep;
    char n[20] = "Fausto";
    int cantPedidos;
    cout<<"Ingrese nombre del repartidor: ";
    cin>> nombreRep;
    cout<<"Ingrese cantidad de Pedidos: ";
    cin>> cantPedidos;
    int zona = getZonaRepartidor(nombreRep);
    cout<<zona<<endl;
}

// Devuelve la zona de un repartidor, fijandose en el archivo
int getZonaRepartidor(string nombreRep){
    FILE* archivo = fopen("Repartidores.dat", "rb");
    if (archivo == NULL) return -1;
    Repartidor r;
    fread(&r, sizeof(Repartidor), 1, archivo);
    while (!feof(archivo)) {
        if (r.nombre == nombreRep) {
            return r.zona;
            fclose(archivo);
        }
        fread(&r, sizeof(Repartidor), 1, archivo);
    }
    fclose(archivo);
    return -1;
}

/*
void mostrar(){
    NodoRepartidor* nr;
    nr = listaRepartidores;
    NodoPedido* p;
    while(nr!=NULL)
    {   
        cout<<nr->dato.nombre<<nr->dato.apellido<<nr->dato.zona<<endl;
        while(p!=NULL){
                cout<<p->dato.importe<<p->dato.comercio<<p->dato.rubro<<endl;
                p=nr->sig;
        }
    }
}

*/

void salir(){
    
}
