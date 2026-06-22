#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>git

using namespace std;

// 1. Duomenų struktūra
struct Automobilis
{
    int id;
    string marke;
    string modelis;
    int metai;
    int galia_kw;
    string modifikacija;
    double kaina;
};

// --- Darbas su failais ---

void nuskaitytiDuomenis(vector<Automobilis> &sarasas)
{
    ifstream failas("duomenys.txt");
    if (!failas.is_open())
    {
        cout << "Klaida: Nepavyko atidaryti duomenu failo nuskaitymui!\n";
        return;
    }

    string eilute;
    int eilesNr = 0;
    while (getline(failas, eilute))
    {
        eilesNr++;
        if (eilute.empty())
            continue;

        stringstream ss(eilute);
        string id_str, marke, modelis, metai_str, galia_str, modifikacija, kaina_str;

        getline(ss, id_str, ';');
        getline(ss, marke, ';');
        getline(ss, modelis, ';');
        getline(ss, metai_str, ';');
        getline(ss, galia_str, ';');
        getline(ss, modifikacija, ';');
        getline(ss, kaina_str, ';');

        // APSAUGA 1: Naudojame try-catch bloką.
        try
        {
            Automobilis autoObj;
            autoObj.id = stoi(id_str);
            autoObj.marke = marke;
            autoObj.modelis = modelis;
            autoObj.metai = stoi(metai_str);
            autoObj.galia_kw = stoi(galia_str);
            autoObj.modifikacija = modifikacija;
            autoObj.kaina = stod(kaina_str);

            sarasas.push_back(autoObj);
        }
        catch (const exception &e)
        {
            cout << "Klaida nuskaitant failo eilute nr. " << eilesNr << ". Eilute praleista.\n";
        }
    }
    failas.close();
    cout << "Duomenys sekmingai nuskaityti! Is viso irasu: " << sarasas.size() << "\n";
}

void issaugotiDuomenis(const vector<Automobilis> &sarasas)
{
    ofstream failas("duomenys.txt");
    if (!failas.is_open())
    {
        cout << "Klaida: Nepavyko atidaryti duomenu failo irasymui!\n";
        return;
    }

    for (const auto &autoObj : sarasas)
    {
        failas << autoObj.id << ";"
               << autoObj.marke << ";"
               << autoObj.modelis << ";"
               << autoObj.metai << ";"
               << autoObj.galia_kw << ";"
               << autoObj.modifikacija << ";"
               << autoObj.kaina << "\n";
    }
    failas.close();
    cout << "Duomenys sekmingai issaugoti i faila!\n";
}

void isvalytiIvestiesKlaida()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- CRUD operacijos ---

void rodytiVisus(const vector<Automobilis> &sarasas)
{
    if (sarasas.empty())
    {
        cout << "Sarasas yra tuscias!\n";
        return;
    }

    cout << "\n------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(5) << "ID"
         << setw(15) << "Marke"
         << setw(15) << "Modelis"
         << setw(8) << "Metai"
         << setw(10) << "Galia(kW)"
         << setw(35) << "Modifikacija"
         << "Kaina(EUR)" << "\n";
    cout << "------------------------------------------------------------------------------------------------------\n";

    for (const auto &autoObj : sarasas)
    {
        cout << left << setw(5) << autoObj.id
             << setw(15) << autoObj.marke
             << setw(15) << autoObj.modelis
             << setw(8) << autoObj.metai
             << setw(10) << autoObj.galia_kw
             << setw(35) << autoObj.modifikacija
             << fixed << setprecision(2) << autoObj.kaina << "\n";
    }
    cout << "------------------------------------------------------------------------------------------------------\n";
}

void pridetiAutomobili(vector<Automobilis> &sarasas)
{
    Automobilis naujasAuto;

    if (sarasas.empty())
    {
        naujasAuto.id = 1;
    }
    else
    {
        naujasAuto.id = sarasas.back().id + 1;
    }

    cout << "\n--- Naujo iraso kurimas ---\n";
    cin.ignore();

    cout << "Iveskite marke: ";
    getline(cin, naujasAuto.marke);

    cout << "Iveskite modeli: ";
    getline(cin, naujasAuto.modelis);

    // APSAUGA 2: Užtikriname, kad įvestas skaičius, o ne tekstas
    cout << "Iveskite gamybos metus: ";
    while (!(cin >> naujasAuto.metai))
    {
        cout << "Klaida! Iveskite skaiciu: ";
        isvalytiIvestiesKlaida();
    }

    cout << "Iveskite galia (kW): ";
    while (!(cin >> naujasAuto.galia_kw))
    {
        cout << "Klaida! Iveskite skaiciu: ";
        isvalytiIvestiesKlaida();
    }

    cin.ignore();
    cout << "Iveskite modifikacija: ";
    getline(cin, naujasAuto.modifikacija);

    cout << "Iveskite modifikacijos kaina (EUR): ";
    while (!(cin >> naujasAuto.kaina))
    {
        cout << "Klaida! Iveskite skaiciu: ";
        isvalytiIvestiesKlaida();
    }

    sarasas.push_back(naujasAuto);
    cout << "Automobilis sekmingai pridetas!\n";
}

void redaguotiAutomobili(vector<Automobilis> &sarasas)
{
    if (sarasas.empty())
    {
        cout << "Nera ka redaguoti - sarasas tuscias!\n";
        return;
    }

    int ieskomasId;
    cout << "\n--- Iraso redagavimas ---\n";
    cout << "Iveskite automobilio ID, kuri norite redaguoti: ";
    while (!(cin >> ieskomasId))
    {
        cout << "Klaida! Iveskite tikslaus ID skaiciu: ";
        isvalytiIvestiesKlaida();
    }

    bool rasta = false;
    for (auto &autoObj : sarasas)
    {
        if (autoObj.id == ieskomasId)
        {
            rasta = true;
            cout << "Rastas automobilis: " << autoObj.marke << " " << autoObj.modelis << "\n";

            cout << "Iveskite nauja galia (kW) (buvo " << autoObj.galia_kw << "): ";
            while (!(cin >> autoObj.galia_kw))
            {
                cout << "Klaida! Iveskite skaiciu: ";
                isvalytiIvestiesKlaida();
            }

            cin.ignore();
            cout << "Iveskite nauja modifikacija (buvo " << autoObj.modifikacija << "): ";
            getline(cin, autoObj.modifikacija);

            cout << "Iveskite nauja modifikacijos kaina (buvo " << autoObj.kaina << "): ";
            while (!(cin >> autoObj.kaina))
            {
                cout << "Klaida! Iveskite skaiciu: ";
                isvalytiIvestiesKlaida();
            }

            cout << "Irasas sekmingai atnaujintas!\n";
            break;
        }
    }

    if (!rasta)
    {
        cout << "Automobilis su ID " << ieskomasId << " nerastas.\n";
    }
}

void istrintiAutomobili(vector<Automobilis> &sarasas)
{
    if (sarasas.empty())
    {
        cout << "Nera ka trinti - sarasas tuscias!\n";
        return;
    }

    int ieskomasId;
    cout << "\n--- Iraso trynimas ---\n";
    cout << "Iveskite automobilio ID, kuri norite istrinti: ";
    while (!(cin >> ieskomasId))
    {
        cout << "Klaida! Iveskite tikslaus ID skaiciu: ";
        isvalytiIvestiesKlaida();
    }

    for (auto it = sarasas.begin(); it != sarasas.end(); ++it)
    {
        if (it->id == ieskomasId)
        {
            cout << "Trinamas automobilis: " << it->marke << " " << it->modelis << "\n";
            sarasas.erase(it);
            cout << "Irasas sekmingai istrintas!\n";
            return;
        }
    }

    cout << "Automobilis su ID " << ieskomasId << " nerastas.\n";
}

// --- Papildomos funkcijos ---

void ieskotiPagalMarke(const vector<Automobilis> &sarasas)
{
    if (sarasas.empty())
    {
        cout << "Sarasas yra tuscias!\n";
        return;
    }

    string ieskomaMarke;
    cout << "\n--- Paieska pagal marke ---\n";
    cout << "Iveskite automobilio marke (pvz., BMW, Volkswagen): ";
    cin >> ieskomaMarke;

    bool rasta = false;
    cout << "\nPaieskos rezultatai:\n";
    cout << "---------------------------------------------------------------------------------\n";

    for (const auto &autoObj : sarasas)
    {
        if (autoObj.marke == ieskomaMarke)
        {
            cout << autoObj.id << ". " << autoObj.marke << " " << autoObj.modelis
                 << " (" << autoObj.galia_kw << " kW) - " << autoObj.modifikacija << "\n";
            rasta = true;
        }
    }

    if (!rasta)
    {
        cout << "Automobiliu su marke '" << ieskomaMarke << "' nerasta.\n";
    }
    cout << "---------------------------------------------------------------------------------\n";
}

void skaiciuotiModifikacijuSuma(const vector<Automobilis> &sarasas)
{
    if (sarasas.empty())
    {
        cout << "Sarasas yra tuscias, nera ko skaiciuoti!\n";
        return;
    }

    double bendraSuma = 0.0;

    for (const auto &autoObj : sarasas)
    {
        bendraSuma += autoObj.kaina;
    }

    cout << "\n--- Modifikaciju Statistika ---\n";
    cout << "Is viso atlikta modifikaciju: " << sarasas.size() << "\n";
    cout << "Bendra visu modifikaciju verte: " << fixed << setprecision(2) << bendraSuma << " EUR\n";
}

// --- Pagrindinė programa ---

int main()
{
    vector<Automobilis> autoSarasas;

    nuskaitytiDuomenis(autoSarasas);

    int pasirinkimas;

    do
    {
        cout << "\n=== Automobiliu Modifikaciju Registras ===\n";
        cout << "1. Rodyti visus irasus (Read)\n";
        cout << "2. Prideti nauja automobili (Create)\n";
        cout << "3. Redaguoti esama irasa (Update)\n";
        cout << "4. Istrinti irasa (Delete)\n";
        cout << "5. Ieskoti pagal marke (Papildoma funkcija 1)\n";
        cout << "6. Skaiciuoti modifikaciju suma (Papildoma funkcija 2)\n";
        cout << "0. Iseiti is programos ir issaugoti\n";
        cout << "Pasirinkite veiksma: ";

        // APSAUGA 3: Meniu pasirinkimo apsauga nuo raidžių įvedimo
        if (!(cin >> pasirinkimas))
        {
            isvalytiIvestiesKlaida();
            pasirinkimas = -1;
        }

        switch (pasirinkimas)
        {
        case 1:
            cout << "\n[Vykdoma: Rodyti visus...]\n";
            rodytiVisus(autoSarasas);
            break;
        case 2:
            cout << "\n[Vykdoma: Prideti nauja...]\n";
            pridetiAutomobili(autoSarasas);
            break;
        case 3:
            cout << "\n[Vykdoma: Redaguoti...]\n";
            redaguotiAutomobili(autoSarasas);
            break;
        case 4:
            cout << "\n[Vykdoma: Istrinti...]\n";
            istrintiAutomobili(autoSarasas);
            break;
        case 5:
            cout << "\n[Vykdoma: Paieska...]\n";
            ieskotiPagalMarke(autoSarasas);
            break;
        case 6:
            cout << "\n[Vykdoma: Sumos skaiciavimas...]\n";
            skaiciuotiModifikacijuSuma(autoSarasas);
            break;
        case 0:
            cout << "\nPrograma baigia darba. Duomenys issaugomi failan...\n";
            issaugotiDuomenis(autoSarasas);
            break;
        default:
            cout << "\nKlaida! Neteisingas pasirinkimas. Bandykite dar karta.\n";
        }
    } while (pasirinkimas != 0);

    return 0;
}