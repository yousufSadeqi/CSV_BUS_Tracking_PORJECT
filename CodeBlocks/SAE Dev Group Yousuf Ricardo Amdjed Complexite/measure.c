#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "var.h"

/*
Rôle : Exécute les mesures de complexité sur différents jeux de données. 
Attention on compte les *i dereferences et on compte i++ comme i+=1 deux operations 
Préconditions : Les fichiers CSV doivent exister dans le dossier documents.
Paramètre : Aucun
Postconditions : Affiche un tableau des opérations élémentaires sur la sortie standard.
Sortie : 
  - 0 si succès
Auteur principal : SADEQI Muhammad Yousuf
*/
int main(){
    char* files[] = {
    "../documents/trajets_10.csv",
    "../documents/trajets_50.csv",
    "../documents/trajets_100.csv",
    "../documents/trajets_500.csv",
    "../documents/trajets_1000.csv",
    "../documents/trajets_bus.csv"
    };

    // En-tête du tableau
    printf("%-10s %-20s %-20s %-15s %-20s\n", 
           "Taille", "TriFusion_Date", "TriRapide_CA", "Recherche", "Redistribution");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 6; i++) {
        Trajet *trajets = trajetsArray; 
        int n;

        // 1. Mesure Tri Fusion (Date)
        nbOperations = 0;
        n = chargerTrajets(files[i], trajets, MAX_TRAJETS);
        if (n == 0) {
             continue; 
        }
        
        int taille_originale = n; // Sauvegarder la taille originale du fichier
        
        nbOperations = 0;
        trierTrajets(trajets, n);
        long opsSortDate = nbOperations;

        // 2. Mesure Tri Rapide (CA)
        n = chargerTrajets(files[i], trajets, MAX_TRAJETS);
        nbOperations = 0;
        triRapideRec(trajets, 0, n - 1);
        long opsSortCA = nbOperations;

        // 3. Mesure Recherche Lineaire (Pire cas)
        nbOperations = 0;
        trouverTrajet(999999, trajets, n);
        long opsSearch = nbOperations;

        // 4. Mesure Redistribution / Suppression Deficitaires
        n = chargerTrajets(files[i], trajets, MAX_TRAJETS);
        nbOperations = 0;
        supprimerTrajetsDeficitaires(trajets, &n);
        long opsRedist = nbOperations;

        printf("%-10d %-20ld %-20ld %-15ld %-20ld\n", taille_originale, opsSortDate, opsSortCA, opsSearch, opsRedist);
    }
    return 0;
}
