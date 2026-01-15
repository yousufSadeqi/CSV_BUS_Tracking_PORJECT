#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/*
Rôle : Lit un entier depuis une chaîne de caractères à partir d'un index donné.
Préconditions : s != NULL, i != NULL
Paramètre : s (chaîne source), i (pointeur vers l'index courant)
Postconditions : L'index i est avancé après le nombre lu.
Sortie :
  - L'entier lu
  - -1 si le champ est vide
Auteur principal : SADEQI Muhammad Yousuf
*/
int lireEntier(const char *s, int *i) {
    // Champ vide : retourner -1
    nbOperations += 8;
    if (s[*i] == ',' || s[*i] == '\0' || s[*i] == '\n') {
        return -1; 
    }
    int val = 0;
    nbOperations ++; // affectation
    while (s[*i] >= '0' && s[*i] <= '9') {
        nbOperations += 5;
        val = val * 10 + (s[*i] - '0');

        nbOperations += 5; 
        *i = *i + 1;

        nbOperations+=2; // incrementation sans *i dereference
    }
    if(s[*i] >= '0'){
        nbOperations += 5; // parce que si s[*i] >= '0' est vrai, on compte 5 ops
    }else{
        nbOperations += 2; // parce que si s[*i] >= '0' est faux, on compte 2 ops
    }
    return val;
}

/*
Rôle : Lit un nombre réel (float) depuis une chaîne de caractères.
Préconditions : s != NULL, i != NULL
Paramètre : s (chaîne source), i (pointeur vers l'index courant)
Postconditions : L'index i est avancé après le nombre lu.
Sortie :
  - Le réel lu
  - -1.0 si le champ est vide
Auteur principal : SADEQI Muhammad Yousuf
*/
float lireReel(const char *s, int *i) {
    // Champ vide : retourner -1.0
    if (s[*i] == ',' || s[*i] == '\0' || s[*i] == '\n' || s[*i] == ':') {
        return -1.0f; 
    }
    
    float val = 0.0f;
    // Partie entiere
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10.0f + (s[*i] - '0');
        *i = *i + 1;
    }
    // Partie decimale
    if (s[*i] == '.') {
        *i = *i + 1;
        float factor = 0.1f;
        while (s[*i] >= '0' && s[*i] <= '9') {
            val += (s[*i] - '0') * factor;
            factor *= 0.1f;
            *i = *i + 1;
        }
    }
    return val;
}

/*
Rôle : Lit une chaîne de caractères jusqu'au prochain délimiteur (virgule).
Préconditions : s != NULL, i != NULL, dest != NULL (taille min 50)
Paramètre : s (chaîne source), i (pointeur vers l'index courant), dest (buffer de destination pour la chaîne lue)
Postconditions : dest contient la chaîne lue, l'index i est avancé.
Sortie : Aucune (void)
Auteur principal : SADEQI Muhammad Yousuf
*/
void lireChamp(const char *s, int *i, char *dest) {
    int k = 0;
    while (s[*i] != ',' && s[*i] != '\0') {
        if (k < 49) { 
            dest[k] = s[*i];
            k = k + 1;
        }
        *i = *i + 1;
    }
    dest[k] = '\0';
    if (s[*i] == ',') {
        *i = *i + 1;
    }
}

/*
Rôle : Compare deux chaînes de caractères (ordre lexicographique).
Préconditions : a != NULL, b != NULL
Paramètre : a (première chaîne), b (deuxième chaîne)
Postconditions : Aucune modification des chaînes.
Sortie :
  - -1 si a < b
  - 1 si a > b
  - 0 si a == b
Auteur principal : SADEQI Muhammad Yousuf
*/
int comparerChaines(const char *a, const char *b) {
    int i = 0;
    nbOperations ++; // affectation
    // access tab * 2 + compairson * 2 + && = 5 ops   
    while (a[i] != '\0' && b[i] != '\0') {
        nbOperations += 5; 
        
        // access tab * 2 + compairson = 3 ops
        nbOperations += 3;
        if (a[i] < b[i]) {
            return -1;
        }
        
        // access tab * 2 + compairson = 3 ops
        nbOperations += 3;
        if (a[i] > b[i]) {
            return 1;
        }
        
        i = i + 1;
        nbOperations+=2; // incrementation
    }
    if(a[i] != '\0'){
        nbOperations += 5; // si a[i] != '\0' est vrai alors on compte 5 
    }else{
        nbOperations += 2; // si a[i] != '\0' est vrai alors on compte 2 car boucle s'arrete 
    }

    nbOperations += 5; // On compte au moins a[i] == 0 (2) + check &&
    if (a[i] == '\0' && b[i] != '\0') {
        return -1;
    }
    
    nbOperations += 5;
    if (a[i] != '\0' && b[i] == '\0') {
        return 1;
    }

    return 0;
}

/*
Rôle : Compare les dates de deux trajets (chronologique).
Préconditions : t1 != NULL, t2 != NULL
Paramètre : t1 (premier trajet), t2 (deuxième trajet)
Postconditions : Aucune.
Sortie :
  - -1 si t1 est avant t2
  - 1 si t1 est après t2
  - 0 si même date
Auteur principal : SADEQI Muhammad Yousuf
*/
int comparerDates(const Trajet *t1, const Trajet *t2) {

    nbOperations++; // comparison sans compter déréférencement de t1 et t2
    if (t1->annee < t2->annee) {
        return -1;
    }
    nbOperations++; // comparison
    if (t1->annee > t2->annee) {
         return 1;
    }
    nbOperations++; // comparison
    if (t1->mois < t2->mois) {
         return -1;
    }
    nbOperations++; // comparison
    if (t1->mois > t2->mois) {
         return 1;
    }
    nbOperations++; // comparison
    if (t1->jour < t2->jour) {
         return -1;
    }
    nbOperations++; // comparison
    if (t1->jour > t2->jour) {
         return 1;
    }
    return 0;
}

/*
Rôle : Fonction de comparaison pour trier les passagers par prix décroissant.
Préconditions : a != NULL, b != NULL
Paramètre : a (pointeur vers passager 1), b (pointeur vers passager 2)
Postconditions : Aucune.
Sortie :
  - 1 si prix a < prix b (pour tri décroissant)
  - -1 si prix a > prix b
  - 0 si prix égaux
Auteur principal : SADEQI Muhammad Yousuf
*/
int comparePassagersPrixDesc(const void *a, const void *b) {
    const Passager *p1 = (const Passager *)a;
    const Passager *p2 = (const Passager *)b;
    nbOperations+=2; // affectation * 2 sans compter déréférencement de p1 et p2

    nbOperations++; // comparison sans compter déréférencement de p1 et p2
    if (p1->prix < p2->prix){
        return 1;
    } 
    nbOperations++; // comparison sans compter déréférencement de p1 et p2
    if (p1->prix > p2->prix) {
        return -1;
    }
    return 0;
}

/*
Rôle : Convertit la date et l'heure d'un trajet en un entier unique (minutes).
Préconditions : t != NULL
Paramètre : t (trajet)
Postconditions : Aucune.
Sortie :
  - Nombre de minutes écoulées depuis l'an 0 (approximatif pour comparaison)
Auteur principal : SADEQI Muhammad Yousuf
*/
long unifieDateHeure(const Trajet *t) {
    nbOperations += 16; // acces champ (mois, jour, heureDepart, heureArrivee) + multiplication * 3 + addition * 3 + division * 2 + modulo * 2
    long minutes = t->annee * 525600 + t->mois * 43200 + t->jour * 1440 
                   + (t->heureDepart / 100) * 60 + (t->heureDepart % 100);
    return minutes;
}
