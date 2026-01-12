#ifndef VAR_H
#define VAR_H

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

#endif
