//
// Created by EGor on 29.10.2019.
//
#include <iostream>
#include <vector>

#include "../base.h"

#ifndef SESC_SPEC_2019_LEXIC_H
    #define SESC_SPEC_2019_LEXIC_H

    std::string ReadFile( std::string fileName );
    std::vector<Token> makeTokens( std::string text );

#endif //SESC_SPEC_2019_LEXIC_H
