//
// Created by Pascal Charpentier on 2022-06-13.
//

#include "HacheurIntQuad.h"

void HacheurIntQuad::reinitialiser(size_t nouvelle) {
    capacite = nouvelle ;
}

size_t HacheurIntQuad::operator()(size_t cle, size_t tentative) const {

    return (cle % capacite + tentative * tentative) % capacite ;
}

HacheurIntQuad::HacheurIntQuad(size_t capacite) : capacite(capacite) {

}
