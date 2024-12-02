#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

const int MAX = 30;

int inputGraph[MAX][MAX];
int krawedzie1[MAX] = {0};
int wierzcholki1 = 0;

int outputGraph[MAX * 2][MAX * 2];
int krawedzie2[MAX] = {0};
int wierzcholki2 = 0;

void Wczytaj() {
  fstream plik1;
  plik1.open("input.txt", ios::in);
  if (plik1.good() == false) {
    cout << "Nie ma takiego pliku";
    exit(0);
  }

  int i = 1, j = 0, wierzcholek;
  string linia, pomin;
  while (!plik1.eof()) {
    getline(plik1, linia);
    stringstream strumien(linia);
    strumien >> pomin >> pomin;
    j = 0;
    while (strumien >> wierzcholek) {
      inputGraph[i][j] = wierzcholek;
      krawedzie1[i]++;
      j++;
    }
    wierzcholki1++;
    i++;
  }
  plik1.close();
}
void Wypisz() {
  fstream plik2;
  plik2.open("outputTomek.txt", ios::out);
  if (plik2.good() == false) {
    cout << "Nie ma pliku wynikowego";
    exit(0);
  }

  for (int j = 1; j <= wierzcholki2; j++) {
    plik2 << j << " : ";
    for (int k = 0; k < krawedzie2[j]; k++) {
      plik2 << outputGraph[j][k] << " ";
    }
    plik2 << endl;
  }
  plik2.close();
}
bool Nie_1_graf() {
  int k = 0;
  for (int i = 1; i <= wierzcholki1; i++) {
    for (int j = 0; j < krawedzie1[i]; j++) {
      k = j + 1;
      while (k < krawedzie1[i]) {
        if (inputGraph[i][j] == inputGraph[i][k])
          return true;
        k++;
      }
    }
  }
  return false;
}
bool Nie_sprzezony() {
  for (int i = 1; i <= wierzcholki1; i++) {
    for (int j = 1; j <= wierzcholki1; j++) {
      int sprawdz[MAX] = {0};
      for (int k = 0; k < krawedzie1[i]; k++) {
        for (int l = 0; l < krawedzie1[j]; l++) {
          if (inputGraph[i][k] == inputGraph[j][l]) {
            sprawdz[k] = 1;
          }
        }
      }
      if (krawedzie1[i] == krawedzie1[j]) {
        for (int m = 0; m < krawedzie1[i]; m++) {
          if (sprawdz[m] == 1) {
            for (int n = 0; n < krawedzie1[i]; n++) {
              if (sprawdz[n] == 0)
                return true;
            }
          }
        }
      } else {
        for (int m = 0; m < krawedzie1[i]; m++) {
          if (sprawdz[m] == 1) {
            return true;
          }
        }
      }
    }
  }
  return false;
}
void Przeksztalc() {
  int lista_lukow[MAX][2] = {0};
  int luk = 1;
  int w = 1;
  for (int i = 1; i <= wierzcholki1; i++) {
    lista_lukow[luk][0] = w++;
    lista_lukow[luk][1] = w++;
    luk++;
  }
  wierzcholki2 = lista_lukow[luk - 1][1];

  for (int i = 1; i <= wierzcholki1; i++) {
    for (int j = 0; j < krawedzie1[i]; j++) {
      int tmp = lista_lukow[i][1];
      int tmp1 = lista_lukow[inputGraph[i][j]][0];
      lista_lukow[i][1] = tmp1;
      for (int k = 1; k < luk; k++) {
        if (lista_lukow[k][0] == tmp)
          lista_lukow[k][0] = tmp1;
        if (lista_lukow[k][1] == tmp)
          lista_lukow[k][1] = tmp1;
      }
    }
  }

  for (int i = 1; i < luk; i++) {
    int wierzcholek = lista_lukow[i][0];
    outputGraph[wierzcholek][krawedzie2[wierzcholek]] = lista_lukow[i][1];
    krawedzie2[wierzcholek]++;
  }

  int nie_izolowany[MAX * 2] = {0};
  for (int i = 1; i <= wierzcholki2; i++) {
    if (krawedzie2[i] == 0) {
      for (int j = 1; j <= wierzcholki2; j++) {
        for (int k = 0; k < krawedzie2[j]; k++) {
          if (outputGraph[j][k] == i) {
            nie_izolowany[i] = 1;
          }
        }
      }
    } else {
      nie_izolowany[i] = 1;
    }
  }

  for (int i = 1; i <= wierzcholki2; i++) {
    if (nie_izolowany[i] == 0) {
      for (int j = i + 1; j <= wierzcholki2; j++) {
        for (int k = 0; k < krawedzie2[j]; k++) {
          outputGraph[j - 1][k] = outputGraph[j][k];
        }
        krawedzie2[j - 1] = krawedzie2[j];
        krawedzie2[j] = 0;
        for (int l = 1; l <= wierzcholki2; l++) {
          for (int m = 0; m < krawedzie2[l]; m++) {
            if (outputGraph[l][m] == j)
              outputGraph[l][m] = j - 1;
          }
        }
      }
      for (int k = 1; k < MAX * 2 - 1; k++) {
        nie_izolowany[k] = nie_izolowany[k + 1];
      }
      wierzcholki2--;
      i--;
    }
  }
}
bool Liniowy() {
  int k = 0;
  for (int i = 1; i <= wierzcholki2; i++) {
    for (int j = 0; j < krawedzie2[i]; j++) {
      k = j + 1;
      while (k < krawedzie2[i]) {
        if (outputGraph[i][j] == outputGraph[i][k])
          return false;
        k++;
      }
    }
  }
  return true;
}

int main() {
  Wczytaj();

  if (Nie_1_graf()) {
    cout << "Graf nie jest 1-grafem, wiec nie moze byc grafem sprzezonym";
    return 0;
  }

  if (Nie_sprzezony()) {
    cout << "Graf nie jest sprzezony";
    return 0;
  }

  Przeksztalc();
  Wypisz();
  if (Liniowy())
    cout << "Graf jest sprzezony i liniowy";
  else
    cout << "Graf jest sprzezony i nie jest liniowy";

  return 1;
}
