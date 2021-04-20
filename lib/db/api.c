#include "db.h"

#include "vector/vector.h"

#if defined(__linux__)
    #include <linux/limits.h>
#else
    #include <sys/syslimits.h>
#endif
#if defined(__linux__)
    #define  _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>
#include <string.h>

void ouverture_db(
    char const* dossier)
{
    char chemin[PATH_MAX];
    
    // To help testing and allow to re-open the DB without closing (and thus writing) it first, destroy the current tables.
    destroy(&table_restaurants);
    destroy(&table_items);
    destroy(&table_livreurs);
    destroy(&table_clients);

    // Read and copy restaurants.
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "r");
    if(test_db_restaurants)
    {
        table_restaurants = lecture_table_restaurants(test_db_restaurants);
        fclose(test_db_restaurants);
    }
    else
    {
        table_restaurants = make_vector(sizeof(restaurant), 0, 2.0);
    }

    // Read and copy items.
    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "r");
    if(test_db_items)
    {
        table_items = lecture_table_items(test_db_items);
        fclose(test_db_items);
    }
    else
    {
        table_items = make_vector(sizeof(item), 0, 2.0);
    }

    // Read and copy deliverers.
    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "r");
    if(test_db_livreurs)
    {
        table_livreurs = lecture_table_livreurs(test_db_livreurs);
        fclose(test_db_livreurs);
    }
    else
    {
        table_livreurs = make_vector(sizeof(livreur), 0, 2.0);
    }

    // Read and copy customers.
    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "r");
    if(test_db_clients)
    {
        table_clients = lecture_table_clients(test_db_clients);
        fclose(test_db_clients);
    }
    else
    {
        table_clients = make_vector(sizeof(client), 0, 2.0);
    }
}

void fermeture_db(
    char const* dossier)
{
    char chemin[PATH_MAX];
    
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "w");
    ecriture_table_restaurants(test_db_restaurants, &table_restaurants);
    fclose(test_db_restaurants);
    destroy(&table_restaurants);

    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "w");
    ecriture_table_items(test_db_items, &table_items);
    fclose(test_db_items);
    destroy(&table_items);

    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "w");
    ecriture_table_livreurs(test_db_livreurs, &table_livreurs);
    fclose(test_db_livreurs);
    destroy(&table_livreurs);

    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "w");
    ecriture_table_clients(test_db_clients, &table_clients);
    fclose(test_db_clients);
    destroy(&table_clients);
}

void efface_db(
    char const* dossier)
{
    char chemin[PATH_MAX];

    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "items.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    remove(chemin);
}

void le_dump_tables(
    FILE* f)
{
    ecriture_table_restaurants(f, &table_restaurants);
    ecriture_table_items(f, &table_items);
    ecriture_table_livreurs(f, &table_livreurs);
    ecriture_table_clients(f, &table_clients);
}
