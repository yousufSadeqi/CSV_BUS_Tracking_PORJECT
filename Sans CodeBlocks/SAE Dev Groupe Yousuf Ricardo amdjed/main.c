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
Auteur principal : SADEQI Muhammad Yousuf
*/
int main() {
    Trajet *trajets = trajetsArray;    
    
    int nbTrajets = chargerTrajets(TRAJETS_CSV_CHEMIN, trajets, MAX_TRAJETS);

    int choix = 1;

    while (choix >= 1 && choix <= 11) {
        afficherMenu();
        if (scanf("%d", &choix) != 1) {
            // oblige d'utiliser pour vider le buffer d'autre choix c'etais d'utiliser getchar() 
            scanf("%*s");
            printf("Erreur: Veuillez entrer un nombre valide.\n");
            choix = 1; // Continuer la boucle
            continue;
        }
        
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
                char temp[10];
                float prix;

                printf("Numero du bus : ");
                scanf("%d", &numBus);
                fgets(temp, 10, stdin); // Vider le tampon

                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    int valide = 0;
                    while (!valide) {
                        printf("Nom du passager (Nom Prenom) : ");
                        valide = lireNomComplet(nom, 50);
                        if (!valide) {
                            printf("Erreur: Veuillez entrer exactement 2 mots (Nom Prenom). Reessayez.\n");
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
                float prix = -1;
                char reponse;
                char *nomPtr = NULL;

                printf("Numero du bus : ");
                scanf("%d", &numBus);

                int idx = trouverTrajet(numBus, trajets, nbTrajets);
                if (idx == -1) {
                    printf("Bus introuvable.\n");
                } else {
                    printf("ID du passager : ");
                    scanf("%d", &id);

                    // Modification du nom 
                    int valide = 0;
                    while (!valide) {
                        printf("Voulez-vous modifier le nom ? (o/n) : ");
                        scanf(" %c", &reponse);

                        if (reponse == 'o' || reponse == 'O') {
                            int nomValide = 0;
                            while (!nomValide) {
                                printf("Nouveau nom (Nom Prenom) : ");
                                nomValide = lireNomComplet(nom, 50);
                                if (!nomValide) {
                                    printf("Erreur: Veuillez entrer exactement 2 mots (Nom Prenom).\n");
                                } else {
                                    nomPtr = nom;
                                }
                            }
                            valide = 1;
                        } else if (reponse == 'n' || reponse == 'N') {
                            valide = 1;
                        } else {
                            printf("Erreur: Veuillez entrer 'o' ou 'n'.\n");
                        }
                    }

                    // Modification prix 
                    valide = 0;
                    while (!valide) {
                        printf("Voulez-vous modifier le prix ? (o/n) : ");
                        scanf(" %c", &reponse);

                        if (reponse == 'o' || reponse == 'O') {
                            printf("Nouveau prix : ");
                            scanf("%f", &prix);
                            valide = 1;
                        } else if (reponse == 'n' || reponse == 'N') {
                            valide = 1;
                        } else {
                            printf("Erreur: Veuillez entrer 'o' ou 'n'.\n");
                        }
                    }

                    modifierPassager(&trajets[idx], id, nomPtr, prix);
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
                // Afficher les trajets triés par Chiffre d'Affaires
                afficherTriParCA(trajets, nbTrajets);
                break;
            }

            case 10: {
                int gain = supprimerTrajetsDeficitaires(trajets, &nbTrajets);
                printf("Operation terminee. Gain genere par redistribution/suppression : %d €\n", gain);
                break;
            }
            
            case 11:
                printf("Au revoir!\n");
                break;
            
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 11.\n");
                break;
        }
    }

    printf("Fin du programme.\n");
    return 0;
}
