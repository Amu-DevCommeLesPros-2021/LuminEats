#include "test_harness/test_harness.h"

#include "db/db.h"
#include "logger/logger.h"
#include "lumineats/lumineats.h"
#include "vector/vector.h"

#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

// Valeurs pour le harnais de test spécifiques à ce programme.
int const tests_total = 124;
int const test_column_width = 60;

int main()
{
    system("rm -rf build/test-db && mkdir -p build/test-db && cp -a test/db/. build/test-db");
    mkdir("build/test-db/ecriture", 0755);
    mkdir("build/test-db/creation-compte", 0755);
    mkdir("build/test-db/suppression-compte", 0755);

    // Tests de lecture et d'écriture de la table 'restaurants'.
    {
        FILE *test_db_restaurants = fopen("build/test-db/restaurants.csv", "r");
        vector restaurants = lecture_table_restaurants(test_db_restaurants);
        fclose(test_db_restaurants);

        TEST(size(restaurants) == 3);

        restaurant *r = (restaurant*)value(begin(&restaurants));
        TEST(r->index == 1);
        TEST(strcmp(r->nom, "Chez Michel") == 0);
        TEST(strcmp(r->code_postal, "13001") == 0);
        TEST(strcmp(r->telephone, "04 13 13 13 13") == 0);
        TEST(strcmp(r->type, "Provencal") == 0);
        TEST(r->items[0] == 1);
        TEST(r->items[1] == 4);
        TEST(r->items[2] == 5);
        TEST(r->items[3] == 0);
        TEST(r->items[TAILLE_ITEMS - 1] == 0);
        TEST(r->solde == 50);

        r = (restaurant*)value(at(&restaurants, 2));
        TEST(r->index == 3);
        TEST(strcmp(r->nom, "Joe's International House of Pancakes") == 0);
        TEST(strcmp(r->code_postal, "13010") == 0);
        TEST(strcmp(r->telephone, "04 22 33 44 55") == 0);
        TEST(strcmp(r->type, "Americain") == 0);
        TEST(r->items[0] == 6);
        TEST(r->items[1] == 7);
        TEST(r->items[2] == 0);
        TEST(r->items[TAILLE_ITEMS - 1] == 0);
        TEST(r->solde == 44);

        FILE *test_db_restaurants_copie = fopen("build/test-db/restaurants-copie.csv", "w");
        ecriture_table_restaurants(test_db_restaurants_copie, &restaurants);
        fclose(test_db_restaurants_copie);
        TEST_FILE("build/test-db/restaurants.csv", "build/test-db/restaurants-copie.csv");

        destroy(&restaurants);
    }

    // Tests de lecture et d'écriture de la table 'items'.
    {
        FILE *test_db_items = fopen("build/test-db/items.csv", "r");
        vector items = lecture_table_items(test_db_items);
        fclose(test_db_items);

        TEST(size(items) == 7);

        item *i = (item*)value(begin(&items));
        TEST(i->index == 1);
        TEST(strcmp(i->nom, "bouillabaise") == 0);
        TEST(strcmp(i->ingredients[0], "poissons de roche") == 0);
        TEST(strcmp(i->ingredients[1], "pommes de terre") == 0);
        TEST(strcmp(i->ingredients[2], "") == 0);
        TEST(i->prix == 25);

        i = (item*)value(at(&items, 6));
        TEST(i->index == 7);
        TEST(strcmp(i->nom, "petit-dej du champion") == 0);
        TEST(strcmp(i->ingredients[0], "oeufs") == 0);
        TEST(strcmp(i->ingredients[1], "toast") == 0);
        TEST(strcmp(i->ingredients[2], "bacon") == 0);
        TEST(strcmp(i->ingredients[3], "pomme de terre") == 0);
        TEST(strcmp(i->ingredients[4], "") == 0);
        TEST(i->prix == 12);

        FILE *test_db_items_copie = fopen("build/test-db/items-copie.csv", "w");
        ecriture_table_items(test_db_items_copie, &items);
        fclose(test_db_items_copie);
        TEST_FILE("build/test-db/items.csv", "build/test-db/items-copie.csv");

        destroy(&items);
    }

    // Tests de lecture et d'écriture de la table 'livreurs'.
    {
        FILE *test_db_livreurs = fopen("build/test-db/livreurs.csv", "r");
        vector livreurs = lecture_table_livreurs(test_db_livreurs);
        fclose(test_db_livreurs);

        TEST(size(livreurs) == 3);

        livreur *l = (livreur*)value(begin(&livreurs));
        TEST(l->index == 1);
        TEST(strcmp(l->nom, "Francois Pignon") == 0);
        TEST(strcmp(l->telephone, "06 00 00 00 00") == 0);
        TEST(strcmp(l->deplacements[0], "13001") == 0);
        TEST(strcmp(l->deplacements[1], "13002") == 0);
        TEST(strcmp(l->deplacements[2], "13003") == 0);
        TEST(strcmp(l->deplacements[3], "") == 0);
        TEST(l->restaurant == 1);
        TEST(l->solde == 20);

        l = (livreur*)value(at(&livreurs, 2));
        TEST(l->index == 3);
        TEST(strcmp(l->nom, "Mickey Mouse") == 0);
        TEST(strcmp(l->telephone, "06 11 22 33 44") == 0);
        TEST(strcmp(l->deplacements[0], "13008") == 0);
        TEST(strcmp(l->deplacements[1], "13009") == 0);
        TEST(strcmp(l->deplacements[2], "13010") == 0);
        TEST(strcmp(l->deplacements[3], "13011") == 0);
        TEST(strcmp(l->deplacements[4], "") == 0);
        TEST(l->restaurant == 0);
        TEST(l->solde == 0);

        FILE *test_db_livreurs_copie = fopen("build/test-db/livreurs-copie.csv", "w");
        ecriture_table_livreurs(test_db_livreurs_copie, &livreurs);
        fclose(test_db_livreurs_copie);
        TEST_FILE("build/test-db/livreurs.csv", "build/test-db/livreurs-copie.csv");

        destroy(&livreurs);
    }

    // Tests de lecture et d'écriture de la table 'clients'.
    {
        FILE *test_db_clients = fopen("build/test-db/clients.csv", "r");
        vector clients = lecture_table_clients(test_db_clients);
        fclose(test_db_clients);

        TEST(size(clients) == 3);

        client *c = (client*)value(begin(&clients));
        TEST(c->index == 1);
        TEST(strcmp(c->nom, "Francoise Perrin") == 0);
        TEST(strcmp(c->code_postal, "13005") == 0);
        TEST(strcmp(c->telephone, "04 10 20 30 40") == 0);
        TEST(c->solde == 0);

        c = (client*)value(at(&clients, 2));
        TEST(c->index == 3);
        TEST(strcmp(c->nom, "Quentin Tarantino") == 0);
        TEST(strcmp(c->code_postal, "13008") == 0);
        TEST(strcmp(c->telephone, "06 99 88 77 66") == 0);
        TEST(c->solde == 15);

        FILE *test_db_clients_copie = fopen("build/test-db/clients-copie.csv", "w");
        ecriture_table_clients(test_db_clients_copie, &clients);
        fclose(test_db_clients_copie);
        TEST_FILE("build/test-db/clients.csv", "build/test-db/clients-copie.csv");

        destroy(&clients);
    }

    // Tests des fonctions de convénience de lecture et d'écriture de la DB.
    {
        ouverture_db("build/test-db");

        TEST(size(table_restaurants) == 3);
        TEST(strcmp(((restaurant*)value(begin(&table_restaurants)))->nom, "Chez Michel") == 0);

        TEST(size(table_items) == 7);
        TEST(strcmp(((item*)value(begin(&table_items)))->nom, "bouillabaise") == 0);

        TEST(size(table_livreurs) == 3);
        TEST(strcmp(((livreur*)value(begin(&table_livreurs)))->nom, "Francois Pignon") == 0);

        TEST(size(table_clients) == 3);
        TEST(strcmp(((client*)value(begin(&table_clients)))->nom, "Francoise Perrin") == 0);

        fermeture_db("build/test-db/ecriture");
        TEST_FILE("build/test-db/restaurants.csv", "build/test-db/ecriture/restaurants.csv");
        TEST_FILE("build/test-db/items.csv", "build/test-db/ecriture/items.csv");
        TEST_FILE("build/test-db/livreurs.csv", "build/test-db/ecriture/livreurs.csv");
        TEST_FILE("build/test-db/clients.csv", "build/test-db/ecriture/clients.csv");
    }

    // Tests pour le système de journal.
    {
        char const* chemin_journal = "test-log.txt";

        lopen(chemin_journal);
        llog("Ceci est un %s, %d", "test", 123);
        lclose();

        // Confirm the file contains the message.
        FILE *log = fopen(chemin_journal, "r");
        char *buffer = NULL;
        size_t buffer_size;
        getline(&buffer, &buffer_size, log);

        TEST(strstr(buffer, "Ceci est un test, 123") != NULL);

        free(buffer);
        fclose(log);
        remove(chemin_journal);
    }

    // Tests d'existence de comptes.
    {
        ouverture_db("build/test-db");

        // Parmi les comptes Restaurateurs.
        TEST(le_compte_existe("Chez Michel") == true);
        TEST(le_compte_existe("04 22 33 44 55") == true);
        
        // Parmi les comptes Livreurs.
        TEST(le_compte_existe("Donald Duck") == true);
        TEST(le_compte_existe("06 00 00 00 00") == true);

        // Parmi les comptes Clients.
        TEST(le_compte_existe("Quentin Tarantino") == true);
        TEST(le_compte_existe("04 10 20 30 40") == true);

        // Tests négatifs.
        TEST(le_compte_existe("") == false);
        TEST(le_compte_existe("La Rotonde") == false);
        TEST(le_compte_existe("00 00 00 00 00") == false);

        fermeture_db("build/test-db");
    }

    // Tests d recherche de comptes.
    {
        ouverture_db("build/test-db");

        // Parmi les comptes Restaurateurs.
        TEST(strcmp(le_cherche_restaurant("Chez Michel")->nom, "Chez Michel") == 0);
        TEST(strcmp(le_cherche_restaurant("04 22 33 44 55")->telephone, "04 22 33 44 55") == 0);
        
        // Parmi les comptes Livreurs.
        TEST(strcmp(le_cherche_livreur("Donald Duck")->nom, "Donald Duck") == 0);
        TEST(strcmp(le_cherche_livreur("06 00 00 00 00")->telephone, "06 00 00 00 00") == 0);

        // Parmi les comptes Clients.
        TEST(strcmp(le_cherche_client("Quentin Tarantino")->nom, "Quentin Tarantino") == 0);
        TEST(strcmp(le_cherche_client("04 10 20 30 40")->telephone, "04 10 20 30 40") == 0);

        // Tests négatifs.
        TEST(le_cherche_restaurant("") == NULL);
        TEST(le_cherche_restaurant("La Rotonde") == NULL);
        TEST(le_cherche_restaurant("00 00 00 00 00") == NULL);

        fermeture_db("build/test-db");
    }

    // Tests de creation de comptes.
    {
        // Création d'un compte Restaurateur.
        ouverture_db("build/test-db/creation-compte");
        char nom_restaurant[] = "Snack-Bar Chez Raymond";
        restaurant *r = le_creer_compte_restaurateur(nom_restaurant, "13001", "04 00 00 00 00", "fast food");

        TEST(le_compte_existe(nom_restaurant) == true);
        TEST(strcmp(r->nom, nom_restaurant) == 0);

        fermeture_db("build/test-db/creation-compte");

        // Vérification intrusive du fichier 'restaurants.csv'.
        FILE *restaurants = fopen("build/test-db/creation-compte/restaurants.csv", "r");
        char *buffer = NULL;
        size_t buffer_size;
        getline(&buffer, &buffer_size, restaurants);
        getline(&buffer, &buffer_size, restaurants);

        TEST(strcmp(buffer, "1,Snack-Bar Chez Raymond,13001,04 00 00 00 00,fast food,,0\n") == 0);

        fclose(restaurants);

        // Création d'un compte Livreur.
        ouverture_db("build/test-db/creation-compte");
        char nom_livreur[] = "Bob Binette";
        livreur *l = le_creer_compte_livreur(nom_livreur, "04 99 99 99 99");
        
        TEST(le_compte_existe(nom_livreur) == true);
        TEST(strcmp(l->nom, nom_livreur) == 0);
        
        fermeture_db("build/test-db/creation-compte");

        // Vérification intrusive du fichier 'livreurs.csv'.
        FILE *livreurs = fopen("build/test-db/creation-compte/livreurs.csv", "r");
        getline(&buffer, &buffer_size, livreurs);
        getline(&buffer, &buffer_size, livreurs);

        TEST(strcmp(buffer, "1,Bob Binette,04 99 99 99 99,,0,0\n") == 0);

        fclose(livreurs);

        // Création d'un compte Client.
        ouverture_db("build/test-db/creation-compte");
        char nom_client[] = "Paul Pitron";
        client *c = le_creer_compte_client(nom_client, "13001", "06 66 66 66 66");

        TEST(le_compte_existe(nom_client) == true);
        TEST(strcmp(c->nom, nom_client) == 0);

        fermeture_db("build/test-db/creation-compte");

        // Vérification intrusive du fichier 'clients.csv'.
        FILE *clients = fopen("build/test-db/creation-compte/clients.csv", "r");
        getline(&buffer, &buffer_size, clients);
        getline(&buffer, &buffer_size, clients);

        TEST(strcmp(buffer, "1,Paul Pitron,13001,06 66 66 66 66,0\n") == 0);

        free(buffer);
        fclose(clients);

        // Tests négatifs. Les noms ou téléphones existent déjà dans la BdD.
        ouverture_db("build/test-db/creation-compte");
        r = le_creer_compte_restaurateur("Snack-Bar Chez Raymond", "13001", "04 11 11 11 11", "fast food");
        TEST(r == NULL);

        l = le_creer_compte_livreur("Bobby Binette", "04 99 99 99 99");
        TEST(l == NULL);

        c = le_creer_compte_client("Paul Pitron", "13001", "06 66 66 66 66");
        TEST(c == NULL);
        fermeture_db("build/test-db/creation-compte");
    }

    // Tests de suppression de compte.
    {
        // Suppression d'un compte Restaurateur.
        ouverture_db("build/test-db/suppression-compte");
        char nom_restaurant[] = "Snack-Bar Chez Raymond";
        le_creer_compte_restaurateur(nom_restaurant, "13001", "04 00 00 00 00", "fast food");
        le_supprimer_compte(nom_restaurant);

        TEST(le_compte_existe(nom_restaurant) == false);

        fermeture_db("build/test-db/suppression-compte");

        // Vérification intrusive du fichier 'restaurants.csv'.
        FILE *restaurants = fopen("build/test-db/suppression-compte/restaurants.csv", "r");
        char *buffer = NULL;
        size_t buffer_size;
        getline(&buffer, &buffer_size, restaurants);
        getline(&buffer, &buffer_size, restaurants);

        TEST(strlen(buffer) == 0);

        fclose(restaurants);

        // Suppression d'un compte Livreur.
        ouverture_db("build/test-db/suppression-compte");
        char nom_livreur[] = "Bob Binette";
        le_creer_compte_livreur(nom_livreur, "04 99 99 99 99");
        le_supprimer_compte(nom_livreur);

        TEST(le_compte_existe(nom_livreur) == false);
        
        fermeture_db("build/test-db/suppression-compte");

        // Vérification intrusive du fichier 'livreurs.csv'.
        FILE *livreurs = fopen("build/test-db/suppression-compte/livreurs.csv", "r");
        getline(&buffer, &buffer_size, livreurs);
        getline(&buffer, &buffer_size, livreurs);

        TEST(strlen(buffer) == 0);

        fclose(livreurs);

        // Suppression d'un compte Client.
        ouverture_db("build/test-db/suppression-compte");
        char nom_client[] = "Paul Pitron";
        le_creer_compte_client(nom_client, "13001", "06 66 66 66 66");
        le_supprimer_compte(nom_client);

        TEST(le_compte_existe(nom_client) == false);

        fermeture_db("build/test-db/suppression-compte");

        // Vérification intrusive du fichier 'clients.csv'.
        FILE *clients = fopen("build/test-db/suppression-compte/clients.csv", "r");
        getline(&buffer, &buffer_size, clients);
        getline(&buffer, &buffer_size, clients);

        TEST(strlen(buffer) == 0);

        free(buffer);
        fclose(clients);


        // Tests négatifs. Supprimer un compte qui n'existe pas ne doit pas affecter les comptes existants.
        ouverture_db("build/test-db/suppression-compte");
        le_creer_compte_restaurateur(nom_restaurant, "13001", "04 11 11 11 11", "fast food");
        le_creer_compte_livreur(nom_livreur, "04 99 99 99 99");
        le_creer_compte_client(nom_client, "13001", "06 66 66 66 66");

        le_supprimer_compte("Inexistant");

        TEST(le_compte_existe(nom_restaurant) == true);
        TEST(le_compte_existe(nom_livreur) == true);
        TEST(le_compte_existe(nom_client) == true);

        fermeture_db("build/test-db/suppression-compte");
    }

    return 0;
}