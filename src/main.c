#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trajet.h"

#define "trajets_bus.csv"

typedef struct {
    int id;
    char nom[50];
    float prix;
} Passager;

typedef struct {
    int numBus;
    char villeDepart[50];
    char villeArrivee[50];
    int jour, mois, annee;
    int heureDepart;
    int heureArrivee;
} Trajet;


int main() {
    Trajet trajets[6000];
    int nb = chargerTrajets("trajets_bus.csv", trajets, 6000);
    afficherTous(trajets, nb);

    // fin de la quesiton 1

    int demande;
    printf("Entrez un numero de bus : ");
    scanf("%d", &demande);

    afficherTrajetDepuisNumBus(demande, trajets, nb);

    // fin de la question 2

    afficherTrajetsTries(trajets, nb);

    // fin de la question 3

    afficherListePassagers(tab, nb);

    // fin de la question 4





    return 0;
}
