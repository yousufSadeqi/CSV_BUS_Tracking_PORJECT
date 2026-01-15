#ifndef TRAJET_H
#define TRAJET_H

#include <stdio.h>
#include "var.h"



int chargerTrajets(const char *nom, Trajet tab[], int max);

// Affichage 

void afficherTrajet(const Trajet *t);
void afficherTrajetComplet(const Trajet *t);
void afficherTous(const Trajet tab[], int n);
void afficherTrajetDepuisNumBus(int numBus, const Trajet tab[], int n);
void afficherMenu();

// Recherche 

int trouverTrajet(int numBus, const Trajet tab[], int n);

// Comparaison 

int comparerTrajets(const Trajet *t1, const Trajet *t2);
// Tri
void trierTrajets(Trajet tab[], int n);
void afficherTrajetsTries(Trajet tab[], int n);

// Gestion des passagers 

int ajouterPassager(Trajet *t, const char *nom, float prix);
void modifierPassager(Trajet *t, int id, const char *nouveauNom, float nouveauPrix);

// Sauvegarde 

void sauvegarderTrajets(const char *nom, Trajet tab[], int n);

// Les Filtres 

int arriveLeLendemain(const Trajet *t);
void filtrerVilleDate(const Trajet tab[], int n,
                      const char *ville, int j, int m, int a);

void filtrerComplet(const Trajet tab[], int n,
                    const char *villeDepart,
                    const char *villeArrivee,
                    int j, int m, int a);

// Chiffre d'affaires 

float chiffreAffaires(const Trajet *t);

// Suppression des trajets deficitaires 

int supprimerTrajetsDeficitaires(Trajet tab[], int *n);

#endif
