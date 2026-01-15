#ifndef UTILS_H
#define UTILS_H

#include "var.h"

// Fonctions de lecture / parsing 
int lireEntier(const char *s, int *i);      
float lireReel(const char *s, int *i);    
void lireChamp(const char *s, int *i, char *dest); 

// Fonctions de comparaison 
int comparerChaines(const char *a, const char *b);          
int comparerDates(const Trajet *t1, const Trajet *t2);    
int comparePassagersPrixDesc(const void *a, const void *b); 

// Fonctions utilitaires / aides 
long unifieDateHeure(const Trajet *t); 
int lireNomComplet(char *nom, int taille);

#endif
