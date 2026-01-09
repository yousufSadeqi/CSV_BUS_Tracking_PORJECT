#ifndef TRAJET_H
#define TRAJET_H

#include <stdio.h>

#define MAX_PASSAGERS 55
#define MAX_TRAJETS 6000


typedef struct {
    int id;
    char nom[50];
    float prix;
} Passager;

typedef struct {
    int numBus;
    char villeDepart[50];
    char villeArrivee[50];
    int jour;
    int mois;
    int annee;
    int heureDepart;
    int heureArrivee;
    Passager passagers[MAX_PASSAGERS];
    int nbPassagers;
} Trajet;


int lireEntier(const char *s, int *i);
float lireReel(const char *s, int *i);
void lireChamp(const char *s, int *i, char *dest);
Trajet lireTrajet(const char *ligne);
int chargerTrajets(const char *nom, Trajet tab[], int max);

/* ===== Affichage ===== */

void afficherTrajet(const Trajet *t);
void afficherTous(const Trajet tab[], int n);
void afficherTrajetDepuisNumBus(int numBus, const Trajet tab[], int n);
void afficherMenu();

/* ===== Recherche ===== */

int trouverTrajet(int numBus, const Trajet tab[], int n);

/* ===== Comparaison / Tri ===== */

int comparerChaines(const char *a, const char *b);
int comparerDates(const Trajet *t1, const Trajet *t2);
int comparerTrajets(const Trajet *t1, const Trajet *t2);
// Tri
void trierTrajets(Trajet tab[], int n);
void afficherTrajetsTries(Trajet tab[], int n);
void afficherTriParCA(Trajet tab[], int n);

/* ===== Gestion des passagers ===== */

int ajouterPassager(Trajet *t, const char *nom, float prix);
int supprimerPassager(Trajet *t, int id);
void modifierPassager(Trajet *t, int id, const char *nouveauNom, float nouveauPrix);

/* ===== Sauvegarde ===== */

void sauvegarderTrajets(const char *nom, Trajet tab[], int n);

/* ===== Filtres ===== */

int arriveLeLendemain(const Trajet *t);
void filtrerVilleDate(const Trajet tab[], int n,
                      const char *ville, int j, int m, int a);

void filtrerComplet(const Trajet tab[], int n,
                    const char *villeDepart,
                    const char *villeArrivee,
                    int j, int m, int a);

/* ===== Chiffre d'affaires ===== */

float chiffreAffaires(const Trajet *t);

/* ===== Suppression des trajets deficitaires ===== */

int supprimerTrajetsDeficitaires(Trajet tab[], int *n);

#endif
