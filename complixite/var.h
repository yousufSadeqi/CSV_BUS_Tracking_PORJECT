#ifndef VAR_H
#define VAR_H

#define MAX_PASSAGERS 55
#define MAX_TRAJETS 6000
#define CA_INVALIDE -1.0e30f
#define TRAJETS_CSV_CHEMIN "../documents/trajets_bus.csv"

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

// Global arrays (defined in globals.c)
extern Trajet trajetsArray[MAX_TRAJETS];
extern Trajet tempArray[MAX_TRAJETS];
extern long nbOperations;

#endif
