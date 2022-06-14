//
// Created by Pascal Charpentier on 2022-06-14.
//

#include "gtest/gtest.h"
#include "tableDispersion.h"
#include "HacheurIntQuad.h"

#include <string>

TEST(TableDeDispersion, exponentiation) {
    TableDeDispersion<int, std::string, HacheurIntQuad> table ;
    EXPECT_EQ(1, table.exponentiation(2, 0)) ;
    EXPECT_EQ(9, table.exponentiation(3, 2)) ;
    EXPECT_EQ(67108864, table.exponentiation(4, 13)) ;
}

TEST(TableDeDispersion, est_premier_true) {
    TableDeDispersion<int, std::string, HacheurIntQuad> table ;
    EXPECT_TRUE(table.est_premier(2)) ;
    EXPECT_TRUE(table.est_premier(3)) ;
    EXPECT_TRUE(table.est_premier(5)) ;
    EXPECT_TRUE(table.est_premier(7)) ;
    EXPECT_TRUE(table.est_premier(11)) ;
    EXPECT_TRUE(table.est_premier(13)) ;
    EXPECT_TRUE(table.est_premier(17)) ;
    EXPECT_TRUE(table.est_premier(19)) ;
    EXPECT_TRUE(table.est_premier(23)) ;
    EXPECT_TRUE(table.est_premier(10007)) ;
}

TEST(TableDeDispersion, est_premier_false) {
    TableDeDispersion<int, std::string, HacheurIntQuad> table ;
    EXPECT_FALSE(table.est_premier(4)) ;
    EXPECT_FALSE(table.est_premier(15)) ;
    EXPECT_FALSE(table.est_premier(9837)) ;
    EXPECT_FALSE(table.est_premier(10005)) ;
}

TEST(TableDeDispersion, prochain_premier) {
    TableDeDispersion<int, std::string, HacheurIntQuad> table ;
    EXPECT_EQ(5, table.prochain_premier(4)) ;
    EXPECT_EQ(23, table.prochain_premier(20)) ;
    EXPECT_EQ(9923, table.prochain_premier(9908)) ;
}

class Tdh_test : public ::testing::Test {
protected:
    TableDeDispersion<int, std::string, HacheurIntQuad> table ;

    void SetUp () override {
        table.inserer(1, "1") ;
        table.inserer(2, "2") ;
        table.inserer(3, "3") ;
        table.inserer(14, "14") ;
    }
} ;

TEST_F(Tdh_test, rechercher_present) {
    EXPECT_EQ("1", table.rechercher(1)) ;
    EXPECT_EQ("2", table.rechercher(2)) ;
    EXPECT_EQ("3", table.rechercher(3)) ;
    EXPECT_EQ("14", table.rechercher(14)) ;
}

TEST_F(Tdh_test, rechercher_absent) {
    EXPECT_THROW(table.rechercher(4), std::runtime_error) ;
    EXPECT_THROW(table.rechercher(66), std::runtime_error) ;
}

TEST_F(Tdh_test, retirer_present) {
    table.retirer(1) ;
    EXPECT_THROW(table.rechercher(1), std::runtime_error) ;
    EXPECT_EQ("14", table.rechercher(14)) ;
}

TEST_F(Tdh_test, retirer_efface) {
    table.retirer(1) ;
    EXPECT_THROW(table.retirer(1), std::runtime_error) ;
}

TEST_F(Tdh_test, retirer_absent) {
    EXPECT_THROW(table.retirer(66), std::runtime_error) ;
}
