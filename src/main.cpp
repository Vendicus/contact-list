#include <iostream>
#include "ksiazka_adresowa.hpp"

using namespace std;

int main(){

    cout<<"Witaj w programie Ksiazka adresowa."<<endl<<endl;

    KsiazkaAdresowa Ksiazka;
    unsigned short int wybor{1};

    while(wybor != 0){
        cout<<endl<<"Wybierz odpowiedni numer z menu aby aktywowac odpowiednia funkcje :"<<endl;
        cout<<"     0. Wyjdz z programu."<<endl;
        cout<<"     1. Zapisz nowego adresata"<<endl;
        cout<<"     2. Wyszukaj istniejacego adresata"<<endl;
        cout<<"     3. Wyswielt wszystkich adresatow"<<endl;
        
        cin>>wybor;

        switch(wybor){
            case 0: break;
            case 1: Ksiazka.zapisz_czlonka(); break;
            case 2: Ksiazka.wyszukaj_czlonka(); break;
            case 3: Ksiazka.wyswietl_wszystkich(); break;
            default:
                cout<<"Podano niepoprawna liczbe, sprobuj ponownie! "<<endl;
        }
        cout<<endl;
    }

    cout<<"Zamykam program, do zobaczenia!"<<endl;

    return 0;
}