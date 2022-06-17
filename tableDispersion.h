//
// Created by Pascal Charpentier on 2022-06-09.
//

#ifndef TABLE_DISPERSION_TABLEDISPERSION_H
#define TABLE_DISPERSION_TABLEDISPERSION_H

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <cassert>

template <typename Cle_type, typename Valeur_type, typename Hash_type>
class TableDeDispersion {

public:
    static size_t prochain_premier(size_t n) ;
    static bool   est_premier(size_t n) ;
    static size_t exponentiation(size_t b, size_t e) ;

public:
    static const size_t Capacite_Defaut = 13 ;
    static const size_t Capacite_Maximale = 10000 ;

private:
    typedef Cle_type Cle ;
    typedef Valeur_type Valeur ;
    typedef Hash_type Hash ;

    typedef enum {Libre, Occupe, Efface} Etat ;
    typedef struct Cellule {
        Cle cle;
        Valeur valeur;
        Etat etat;
    } Cellule ;


public:
    explicit TableDeDispersion(size_t capacite = Capacite_Defaut) ;
    TableDeDispersion(std::initializer_list<Cle> cles, std::initializer_list<Valeur> valeurs) ;

    void          inserer   (const Cle& cle, const Valeur& valeur) ;
    const Valeur& rechercher(const Cle& cle) const ;
    void          retirer   (const Cle& cle) ;
    void          effacer   () ;

private:
    std::vector<Cellule> table ;
    size_t capacite ;
    size_t cardinal ;
    Hash   hacheur ;

private:
    void rehacher () ;
    size_t index (const Cle& cle) const ;

};


/**
 * Ajoute une paire clé valeur à la table.  Lance une exception si la clé est déjà présente.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param cle Nouvelle clé
 * @param valeur Nouvelle valeur
 * @except runtime_error si la clé est déjà dans la table.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
void TableDeDispersion<Cle_type, Valeur_type, Hash_type>::inserer(const Cle &cle, const Valeur &valeur) {
    auto& cellule = table.at(index(cle)) ;
    if (cellule.etat == Occupe) throw std::runtime_error("inserer: insertion dans une cellule occupée") ;
    cellule.cle = cle ;
    cellule.valeur = valeur ;
    cellule.etat = Occupe ;
    ++ cardinal ;
    if (capacite  <= 2 * cardinal) rehacher() ;
}

/**
 * Cherche la valeur associées à une clé
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param cle Clé à rechercher
 * @return La valeur associée à la clé si celle-ci est présente.
 * @except runtime_error si la clé est absente de la table.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
const Valeur_type &TableDeDispersion<Cle_type, Valeur_type, Hash_type>::rechercher(const Cle &cle) const {
    const auto& cellule = table.at(index(cle)) ;
    if (cellule.cle == cle && cellule.etat == Occupe) return cellule.valeur ;
    throw std::runtime_error("rechercher: clé absente") ;

}

/**
 * Enlève une paire clé valeur de la table
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param cle Clé à retirer
 * @except runtime_error si la clé à retirer est absente.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
void TableDeDispersion<Cle_type, Valeur_type, Hash_type>::retirer(const Cle &cle) {
    auto& cellule = table.at(index(cle)) ;
    if (cellule.cle == cle && cellule.etat == Occupe) {
        cellule.etat = Efface ;
        -- cardinal ;
    }
    else throw std::runtime_error("retirer: clé absente") ;
}

/**
 * Constructeur de base.  Crée une table vide avec une capacité donnée.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param capacite Capacité initiale de la table.  Réglée par défaut à la constante Capacite_Defaut.
 * @except Si la capacité dépasse la capacité maximale Capacite_Maximale
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
TableDeDispersion<Cle_type, Valeur_type, Hash_type>::TableDeDispersion(size_t capacite) :
table(capacite), capacite(capacite), cardinal(0),  hacheur(capacite) {
    if (capacite > Capacite_Maximale) throw std::invalid_argument ("Constructeur: capacité maximale dépassée.") ;
}

/**
 * Constructeur à liste d'initialisation.  Construit une table avec une liste de clés et une liste de valeurs.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param cles Liste d'initialisation des clés.
 * @param valeurs Liste d'initialisation des valeurs
 * @except runtime_error si les deux listes ne sont pas de la même longueur.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
TableDeDispersion<Cle_type, Valeur_type, Hash_type>::TableDeDispersion(std::initializer_list<Cle> cles,
                                                                       std::initializer_list<Valeur> valeurs) :
        table(), capacite(capacite), cardinal(0), hacheur(capacite) {

    if (cles.size() != valeurs.size()) throw std::runtime_error("Listes d'initialisations fautives dans le constructeur.") ;
    for (auto cle: cles) inserer(cle, *valeurs.begin());


}

/**
 * Méthode privée.  Rajuste la capacité de la table afin qu'elle soit toujours un nombre premier au-moins égal au double
 * du cardinal.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
void TableDeDispersion<Cle_type, Valeur_type, Hash_type>::rehacher() {

    std::vector<Cellule> copie = std::move(table) ;
    table.clear() ;

    capacite = prochain_premier(2 * capacite) ;
    table.resize(capacite) ;
    hacheur.reinitialiser(capacite) ;

    for (auto cellule: table) cellule.etat = Libre ;
    for (auto cellule: copie) if (cellule.etat == Occupe) inserer(cellule.cle, cellule.valeur) ;
}

/**
 * Recherche la position d'une clé
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param cle Clé demandée
 * @return La position où se trouve la clé, ou bien la première position inactive disponible.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
size_t TableDeDispersion<Cle_type, Valeur_type, Hash_type>::index(const Cle &cle) const {
    size_t tentative = 0 ;
    size_t i = hacheur(cle, tentative) ;
    auto cellule = table.at(i) ;
    while (cellule.cle != cle && cellule.etat != Libre) {
        ++ tentative ;
        i = hacheur(cle, tentative) ;
        cellule = table.at(i) ;
        assert(tentative < 1000) ;
    }
    return  i ;
}

/**
 * Efface complètement la table de dispersion: celle-ci est donc vide.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
void TableDeDispersion<Cle_type, Valeur_type, Hash_type>::effacer() {
    cardinal = 0 ;
    for (auto cellule: table) cellule.etat = Libre ;
}

/**
 * Recherche un nombre premier supérieur à l'argument
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param n Nombre dont on veut connaitre le successeur premier.  Il doit être pair.
 * @return
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
size_t TableDeDispersion<Cle_type, Valeur_type, Hash_type>::prochain_premier(size_t n) {
    size_t i = n + 1 ;
    while (!est_premier(i)) i += 2 ;
    return i ;
}

/**
 * Méthode bête et méchante, vu qu'on ne manipule pas de très gros nombres ce sera acceptable.  L'idéal aurait été de
 * prendre le test de Miller-Rabin.
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param n Entier à tester pour la primalité
 * @return true si n est premier, false si n est composite.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
bool TableDeDispersion<Cle_type, Valeur_type, Hash_type>::est_premier(size_t n) {
    for (auto i = 2; i * i <= n; ++i) if (n % i == 0) return false  ;
    return true ;
}

/**
 * Élève un entier à une puissance entière
 * @tparam Cle_type
 * @tparam Valeur_type
 * @tparam Hash_type
 * @param a Un entier non signé
 * @param e Un entier non signé
 * @return a puissance e.
 */
template<typename Cle_type, typename Valeur_type, typename Hash_type>
size_t TableDeDispersion<Cle_type, Valeur_type, Hash_type>::exponentiation(size_t a, size_t e) {
    if (e == 0) return 1 ;
    size_t r = 1 ;
    for (auto b = e; b != 0; b /= 2) {
        if (b % 2 == 1) r *= a ;
        a *= a ;
    }
   return r ;
}

#endif //TABLE_DISPERSION_TABLEDISPERSION_H
