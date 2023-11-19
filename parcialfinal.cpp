#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Equipo {
    string nombre;
    int puntos;
    int golesMarcados;
};

void jugarPartido(Equipo &equipo1, Equipo &equipo2) {
    equipo1.golesMarcados = rand() % 5;
    equipo2.golesMarcados = rand() % 5;

    if (equipo1.golesMarcados > equipo2.golesMarcados) {
        equipo1.puntos += 3; 
    } else if (equipo1.golesMarcados < equipo2.golesMarcados) {
        equipo2.puntos += 3; 
    } else {
        equipo1.puntos += 1; 
        equipo2.puntos += 1;
    }

    cout << equipo1.nombre << " " << equipo1.golesMarcados << " - " << equipo2.golesMarcados << " " << equipo2.nombre << endl;
}

void simularFaseGrupos(vector<vector<Equipo>> &grupos) {
    for (auto &grupo : grupos) {
        int numEquipos = grupo.size();

        for (int i = 0; i < numEquipos; ++i) {
            for (int j = i + 1; j < numEquipos; ++j) {
                jugarPartido(grupo[i], grupo[j]);
            }
        }

        cout << "\n--- Tabla de Posiciones del Grupo ---\n";
        cout << "Equipo\tPuntos\n";
        for (const auto &equipo : grupo) {
            cout << equipo.nombre << "\t" << equipo.puntos << "\n";
        }
    }
}

void clasificarEquipos(vector<Equipo> &equipos, int numClasificados) {

    sort(equipos.begin(), equipos.end(), [](const Equipo &a, const Equipo &b) {
        return a.puntos > b.puntos;
    });


    equipos.erase(equipos.begin() + numClasificados, equipos.end());
}

void simularRonda(vector<Equipo> &equipos, const string &nombreRonda, int numClasificados) {
    cout << "\n--- " << nombreRonda << " ---\n";


    clasificarEquipos(equipos, numClasificados);


    for (int i = 0; i < equipos.size(); i += 2) {
        jugarPartido(equipos[i], equipos[i + 1]);
    }
}

int main() {
    srand(time(0));

    vector<Equipo> equipos;


    ifstream archivoEquipos("equipos.txt");

    if (!archivoEquipos.is_open()) {
        cerr << "No se pudo abrir el archivo de equipos." << endl;
        return 1; 
    }

    string nombreEquipo;
    while (getline(archivoEquipos, nombreEquipo)) {
        equipos.push_back({nombreEquipo, 0, 0});
    }

    archivoEquipos.close();


    int numGrupos = equipos.size() / 4;
    vector<vector<Equipo>> grupos(numGrupos);


    for (int i = 0; i < numGrupos; ++i) {
        grupos[i] = vector<Equipo>(equipos.begin() + i * 4, equipos.begin() + (i + 1) * 4);
    }


    simularFaseGrupos(grupos);


    vector<Equipo> equiposOctavos;
    for (const auto &grupo : grupos) {
        equiposOctavos.insert(equiposOctavos.end(), grupo.begin(), grupo.end());
    }
    simularRonda(equiposOctavos, "Octavos de Final", 16);


    simularRonda(equiposOctavos, "Cuartos de Final", 8);


    simularRonda(equiposOctavos, "Semifinales", 4);

    simularRonda(equiposOctavos, "Final", 2);

    cout << "\n¡El campeón del Mundial es: " << (equiposOctavos[0].puntos > equiposOctavos[1].puntos ? equiposOctavos[0].nombre : equiposOctavos[1].nombre) << "!" << endl;

    return 0;
}


//herramienta/ opciones de copilador/añadir -std=c++11 -std=gnu++11

