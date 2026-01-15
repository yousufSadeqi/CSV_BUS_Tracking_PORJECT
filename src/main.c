#include <stdio.h>
#include <string.h>
#include "var.h"
#include "function.h"
#include "utils.h"

/*
Rôle : Point d'entrée du programme. Gère le menu principal et les appels fonctions.
Préconditions : Fichier trajets_bus.csv accessible pour chargement
Paramètre : Aucun
Postconditions : L'application s'exécute et se termine normalement sur choix utilisateur ou erreur.
Sortie :
  - 0 si succès
Auteur principal : Yousuf
*/
int main() {
    Trajet *trajets = trajetsArray;    
    
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

            case 3:{
                afficherTrajetsTries(trajets, nbTrajets);
                break;
            }
            case 4: {
                int numBus;
                char nom[50];
                char tampon[10];
                float prix;

                printf("Numero du bus : ");
                scanf("%d", &numBus);
                fgets(tampon, 10, stdin); // Vider le tampon

                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    int valide = 0;
                    while (!valide) {
                        printf("Nom du passager (Prenom Nom) : ");
                        fgets(nom, 50, stdin);
                        
                        // Enlever le saut de ligne
                        int longueur = 0;
                        while (nom[longueur] != '\0' && nom[longueur] != '\n') longueur++;
                        if (nom[longueur] == '\n') nom[longueur] = '\0';
                        
                        // Compter les mots
                        int mots = 0;
                        int dans_mot = 0;
                        int i = 0;
                        while (nom[i] != '\0') {
                            if (nom[i] != ' ') {
                                if (!dans_mot) {
                                    mots++;
                                    dans_mot = 1;
                                }
                            } else {
                                dans_mot = 0;
                            }
                            i++;
                        }
                        
                        if (mots == 2) {
                            valide = 1;
                        } else {
                            printf("Erreur: Veuillez entrer exactement 2 mots (Prenom Nom). Reessayez.\n");
                        }
                    }

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

                    printf("Nouveau nom : ");
                    printf("(Entrez '.' pour garder le meme nom) : ");
                    scanf("%s", nom);
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
                scanf("%s", ville);

                printf("Date (JJ MM AAAA) : ");
                scanf("%d %d %d", &j, &m, &a);

                filtrerVilleDate(trajets, nbTrajets, ville, j, m, a);
                break;
            }

            case 8: {
                char vd[50], va[50];
                int j, m, a;

                printf("Ville depart : ");
                scanf("%s", vd);

                printf("Ville arrivee : ");
                scanf("%s", va);

                printf("Date (JJ MM AAAA) : ");
                scanf("%d %d %d", &j, &m, &a);

                filtrerComplet(trajets, nbTrajets, vd, va, j, m, a);
                break;
            }

            case 9: {
                // Afficher chiffre d'affaires (simple liste)
                int i = 0;
                while (i < nbTrajets) {
                    float ca = chiffreAffaires(&trajets[i]);
                    
                    if (ca == CA_INVALIDE) {
                        printf("Bus %d : Calcul invalide - horaire manquant\n", trajets[i].numBus);
                    } else {
                        printf("Bus %d : Chiffre d'affaires = %.2f €\n", trajets[i].numBus, ca);
                    }
                    
                    i++;
                }
                break;
            }

            case 10: {
                int gain = supprimerTrajetsDeficitaires(trajets, &nbTrajets);
                printf("Operation terminee. Gain genere par redistribution/suppression : %d €\n", gain);
                break;
            }
        }
    }

    printf("Fin du programme.\n");
    return 0;
}
