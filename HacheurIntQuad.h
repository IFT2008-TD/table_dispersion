//
// Created by Pascal Charpentier on 2022-06-13.
//

#ifndef TABLE_DISPERSION_HACHEURINTQUAD_H
#define TABLE_DISPERSION_HACHEURINTQUAD_H


#include <cstddef>

class HacheurIntQuad {

public:

    explicit HacheurIntQuad(size_t capacite) ;
    void     reinitialiser (size_t nouvelle) ;

    size_t   operator () (size_t cle, size_t tentative) const ;

private:
    size_t   capacite ;


};


#endif //TABLE_DISPERSION_HACHEURINTQUAD_H
