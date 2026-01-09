#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

#define MAX_TRAJETS 6000

int main() {
    Trajet *trajets = (Trajet *)malloc(MAX_TRAJETS * sizeof(Trajet));
    if (!trajets) {
        printf("Erreur allocation memoire.\n");
        return 1;
    }
    int nbTrajets = chargerTrajets("trajets_bus.csv", trajets, MAX_TRAJETS);

    int choix = 1;

    while (choix >= 1 && choix <= 12) {

        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherTous(trajets, nbTrajets);
                break;

            case 2: {
                int numBus;
                printf("Numero du bus : ");
                scanf("%d", &numBus);
                afficherTrajetDepuisNumBus(numBus, trajets, nbTrajets);
                break;
            }

            case 3:
                afficherTrajetsTries(trajets, nbTrajets);
                break;

            case 4: {
                int numBus;
                char nom[50];
                float prix;

                printf("Numero du bus : ");
                scanf("%d", &numBus);

                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    printf("Nom du passager : ");
                    scanf(" %49[^\n]", nom);

                    printf("Prix du billet : ");
                    scanf("%f", &prix);

                    if (ajouterPassager(&trajets[idx], nom, prix))
                        printf("Passager ajoute.\n");
                    else
                        printf("Bus complet.\n");
                }
                break;
            }

            case 5: {
                int numBus, id;
                char nom[50];
                float prix;

                printf("Numero du bus : ");
                scanf("%d", &numBus);

                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    printf("ID du passager : ");
                    scanf("%d", &id);

                    printf("Nouveau nom (laisser identique si vide) : ");
                    printf("(Entrez '.' pour garder le meme nom) : ");
                    scanf(" %49[^\n]", nom);
                    if (strcmp(nom, ".") == 0) {
                         modifierPassager(&trajets[idx], id, NULL, -1);
                    } else {
                         printf("Nouveau prix (-1 pour ne pas changer) : ");
                         scanf("%f", &prix);
                         modifierPassager(&trajets[idx], id, nom, prix);
                    }
                    printf("Modification effectuee.\n");
                }
                break;
            }

            case 6:
                sauvegarderTrajets("sauvegarde.csv", trajets, nbTrajets);
                printf("Donnees sauvegardees.\n");
                break;

            case 7: {
                char ville[50];
                int j, m, a;

                printf("Ville de depart : ");
                scanf(" %49[^\n]", ville);

                printf("Date (JJ MM AAAA) : ");
                scanf("%d %d %d", &j, &m, &a);

                filtrerVilleDate(trajets, nbTrajets, ville, j, m, a);
                break;
            }

            case 8: {
                char vd[50], va[50];
                int j, m, a;

                printf("Ville depart : ");
                scanf(" %49[^\n]", vd);

                printf("Ville arrivee : ");
                scanf(" %49[^\n]", va);

                printf("Date (JJ MM AAAA) : ");
                scanf("%d %d %d", &j, &m, &a);

                filtrerComplet(trajets, nbTrajets, vd, va, j, m, a);
                break;
            }

            case 9: {
                // Afficher chiffre d'affaires (simple liste)
                int i = 0;
                while (i < nbTrajets) {
                    printf("Bus %d : Chiffre d'affaires = %.2f €\n",
                           trajets[i].numBus,
                           chiffreAffaires(&trajets[i]));
                    i++;
                }
                break;
            }

            case 10: {
                int gain = supprimerTrajetsDeficitaires(trajets, &nbTrajets);
                printf("Operation terminee. Gain genere par redistribution/suppression : %d €\n", gain);
                break;
            }

            case 11: {
                // Supprimer un passager
                int numBus, id;
                printf("Numero du bus : ");
                scanf("%d", &numBus);
                
                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    printf("ID du passager a supprimer : ");
                    scanf("%d", &id);
                    if (supprimerPassager(&trajets[idx], id)) {
                        printf("Passager supprime.\n");
                    } else {
                        printf("Passager introuvable.\n");
                    }
                }
                break;
            }

            case 12:
                afficherTriParCA(trajets, nbTrajets);
                break;
        }
    }

    printf("Fin du programme.\n");
    free(trajets);
    return 0;
}
