#include "catch.hpp"

#include "../Main/geom.hpp"
#include <sstream>

SCENARIO("Moguce je konstruisati preslikavanja", "[ctor]"){
    SECTION("Konstruktor bez argumenta"){
        geom::geom g;

        geom::Tip k{{1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}};

        REQUIRE(g.mat() == k);
    }

    SECTION("Konstruktor kopije"){
        geom::geom g{{1, 2, 3},
                     {4, 5, 6},
                     {0, 0, 1}};

        auto k(g);

        CHECK(k.mat() == g.mat());

        auto b = k;

        CHECK(k.mat() == b.mat());
        CHECK(g.mat() == b.mat());

        auto a = std::move(g);

        CHECK(k.mat() == a.mat());
        CHECK(b.mat() == a.mat());

        REQUIRE(g.mat().empty());
    }

    GIVEN("Korektna matrica"){
        geom::Tip mat{{1, 1, 1},
                      {1, 1, 1},
                      {0, 0, 1}};

        WHEN("L-value"){
            REQUIRE_NOTHROW(geom::geom(mat));

            REQUIRE(geom::geom(mat).mat() == mat);
        }

        WHEN("R-value"){
            REQUIRE_NOTHROW((geom::geom(geom::Tip{{1, 1, 1},
                                                  {1, 1, 1},
                                                  {0, 0, 1}})));
        }

        WHEN("Move ctor"){
            geom::geom g(std::move(mat));

            REQUIRE(mat.empty());
        }
    }

    GIVEN("Nekorektna matrica"){
        geom::Tip mat{{1, 1, 1},
                      {1, 1, 1},
                      {0, 0, 0}};

        geom::Tip mat1{{1, 1, 1},
                       {1, 1},
                       {0, 0, 1}};

        geom::Tip mat2{{1, 1, 1},
                       {1, 1, 1}};

        WHEN("L-value"){
            CHECK_THROWS_AS(geom::geom(mat), geom::Exc);
            CHECK_THROWS_AS(geom::geom(mat1), geom::Exc);
            REQUIRE_THROWS_AS(geom::geom(mat2), geom::Exc);
        }

        WHEN("R-value"){
            CHECK_THROWS_AS((geom::geom(geom::Tip{{1, 1, 1},
                                                  {1, 1, 1},
                                                  {0, 0, 0}})),
                            geom::Exc);
            CHECK_THROWS_AS((geom::geom(geom::Tip{{1, 1, 1},
                                                  {1, 1},
                                                  {0, 0, 1}})),
                            geom::Exc);
            REQUIRE_THROWS_AS((geom::geom(geom::Tip{{1, 1, 1},
                                                    {1, 1, 1}})),
                              geom::Exc);
        }

        WHEN("Move ctor"){
            REQUIRE_THROWS_AS(geom::geom(std::move(mat)),
                              geom::Exc);
            CHECK(mat.empty());

            REQUIRE_THROWS_AS(geom::geom(std::move(mat1)),
                              geom::Exc);
            CHECK(mat1.empty());

            REQUIRE_THROWS_AS(geom::geom(std::move(mat2)),
                              geom::Exc);
            REQUIRE(mat2.empty());
        }
    }

    GIVEN("Korektan niz vektora"){
        geom::Tip mat{{1, 1, 1},
                      {1, 1, 1},
                      {0, 0, 1}};

        WHEN("L-value"){
            REQUIRE_NOTHROW(geom::geom(mat[0],
                                       mat[1],
                                       mat[2]));

            geom::geom g(mat[0], mat[1], mat[2]);

            for (geom::Vel i = 0; i < std::size(g); i++){
                DYNAMIC_SECTION("Iteracija: " << i) {
                    REQUIRE(g.mat()[i] == mat[i]);
                }
            }
        }

        WHEN("R-value"){
            REQUIRE_NOTHROW((geom::geom{{1, 1, 1},
                                        {1, 1, 1},
                                        {0, 0, 1}}));
        }

        WHEN("Move ctor"){
            geom::geom g{std::move(mat[0]),
                         std::move(mat[1]),
                         std::move(mat[2])};

            for (geom::Vel i = 0; i < std::size(mat); i++){
                DYNAMIC_SECTION("Iteracija: " << i) {
                    REQUIRE(mat[i].empty());
                }
            }
        }
    }

    GIVEN("Nekorektan niz vektora"){
        geom::Tip mat{{1, 1, 1},
                      {1, 1, 1},
                      {1, 0, 1}};

        WHEN("L-value"){
            REQUIRE_THROWS_AS((geom::geom{mat[0],
                                          mat[1],
                                          mat[2]}),
                              geom::Exc);
        }

        WHEN("R-value"){
            REQUIRE_THROWS_AS((geom::geom{{1, 1, 1},
                                          {1, 1, 1},
                                          {1, 0, 1}}),
                              geom::Exc);
        }

        WHEN("Move ctor"){
            REQUIRE_THROWS_AS((geom::geom{std::move(mat[0]),
                                          std::move(mat[1]),
                                          std::move(mat[2])}),
                              geom::Exc);

            for (geom::Vel i = 0; i < std::size(mat); i++){
                DYNAMIC_SECTION("Iteracija: " << i) {
                    REQUIRE(mat[i].empty());
                }
            }
        }
    }
}

SCENARIO("Inverz preslikavanja", "[inv]"){
    GIVEN("Neka preslikavanja"){
        geom::geom g{{1, 0, 1},
                     {0, 1, 1},
                     {0, 0, 1}};

        geom::geom k{{1, 0, 1},
                     {0, 0, 1},
                     {0, 0, 1}};

        WHEN("Postoji inverz"){
            geom::geom b;

            REQUIRE_NOTHROW(b = g.inv());

            THEN("Inverz inverza je polazna"){
                CHECK(g == b.inv());
                CHECK(g == g.inv().inv());
            }

            g.inv(true);
            CHECK(g == b);

            geom::inv(g, true);
            CHECK(g == geom::inv(b));

            THEN("Proizvod je jedinicna"){
                CHECK(g * g.inv() == geom::geom());
                REQUIRE(g * geom::inv(g) == geom::geom());
            }
        }

        WHEN("Ne postoji inverz"){
            CHECK_THROWS_AS(k.inv(), geom::Exc);
            CHECK_THROWS_AS(k.inv(true), geom::Exc);

            CHECK_THROWS_AS(geom::inv(k), geom::Exc);
            REQUIRE_THROWS_AS(geom::inv(k, true), geom::Exc);
        }
    }
}

SCENARIO("Predstavljanje u obliku niske", "[str]"){
    GIVEN("Neko preslikavanje"){
        geom::geom g{{1, 0, 1},
                     {0, 1, 1},
                     {0, 0, 1}};

        WHEN("Sve je uobicajeno"){
            std::ostringstream s;
            s << "[[1, 0, 1]" << std::endl;
            s << " [0, 1, 1]" << std::endl;
            s << " [0, 0, 1]]";

            REQUIRE(g.str() == s.str());
        }

        WHEN("Preslikavanju je uzeta implementacija"){
            auto k = std::move(g);

            THEN("Niska je prazna"){
                REQUIRE(g.str() == "[]");
            }
        }
    }
}

SCENARIO("Moguce je dodeljivati preslikavanja", "[dodela]"){
    GIVEN("Vec postojeca preslikavanja"){
        geom::geom g{{1, 0, 1},
                     {0, 1, 1},
                     {0, 0, 1}};

        geom::geom k;

        WHEN("Dodeljuje se transformacija"){
            REQUIRE_NOTHROW(k = g);
            CHECK(k == g);

            REQUIRE_NOTHROW(k = std::move(g));
            REQUIRE(g.mat().empty());
        }
    }

    GIVEN("Korektna matrica"){
        geom::Tip mat{{1, 1, 1},
                      {1, 1, 1},
                      {0, 0, 1}};

        geom::geom g;

        WHEN("Dodeljuje se matrica"){
            REQUIRE_NOTHROW(g = mat);
            CHECK(g.mat() == mat);
            CHECK(g == geom::geom(mat));

            REQUIRE_NOTHROW(g = std::move(mat));
            // Ono kad je testiranje stvarno posluzilo svrsi!
            // Ovaj test nije prosao jer je slucajno dodela
            // sa semantikom pomeranja bila deklarisana sa
            // const Tip&&, sto bi vrv proslo neopazeno
            CHECK(mat.empty());

            REQUIRE_NOTHROW(g = geom::Tip{{1, 1, 1},
                                          {1, 1, 1},
                                          {0, 0, 1}});
        }
    }

    GIVEN("Nekorektne matrice"){
        geom::Tip a{{1, 0, 1},
                    {1, 0, 1},
                    {1, 0, 1}};

        geom::Tip b{{1, 0, 1},
                    {1, 0, 1}};

        geom::Tip c{{1, 0, 1},
                    {1, 0, 1},
                    {1, 0}};

        geom::geom g;

        WHEN("Dodeljuje se matrica"){
            CHECK_THROWS_AS(g = a, geom::Exc);
            CHECK_THROWS_AS(g = b, geom::Exc);
            CHECK_THROWS_AS(g = c, geom::Exc);

            REQUIRE_THROWS_AS(g = std::move(a), geom::Exc);
            CHECK(a.empty());

            REQUIRE_THROWS_AS(g = std::move(b), geom::Exc);
            CHECK(b.empty());

            REQUIRE_THROWS_AS(g = std::move(c), geom::Exc);
            REQUIRE(c.empty());
        }
    }
}

SCENARIO("Moguce je pristupati preko indeksa", "[index]"){
    GIVEN("Preslikavanje i niz indeksa"){
        geom::geom g;

        auto i = GENERATE_REF(0ull, std::size(g)/2, std::size(g)-1);
            WHEN("Postoji par indeksa"){
                auto j = GENERATE_REF(0ull, std::size(g)/2, std::size(g)-1);
                CHECK_NOTHROW(g[i]);

                THEN("Mora biti konstantna vrednost"){
                    using Tip = std::remove_reference<decltype(g[i])>::type;
                    CHECK(std::is_const<Tip>::value);
                }

                CHECK_NOTHROW(g[i][i]);
                REQUIRE_NOTHROW(g[i][j]);
            }

        auto niz = {std::size(g), std::size(g)+1, std::size(g)+100};
        for (const auto& i : niz){
            for (const auto& j : niz){
                DYNAMIC_SECTION("Iteracija: " << i << j) {
                    CHECK_THROWS_AS(g[i], std::out_of_range);
                    CHECK_THROWS_AS(g[i][i], std::out_of_range);
                    REQUIRE_THROWS_AS(g[i][j], std::out_of_range);
                }
            }
        }
    }
}

SCENARIO("Moguce je ispisati transformaciju na izlazni tok", "[izlaz]"){
    GIVEN("Vec postojeca preslikavanja"){
        geom::geom g{{1, 0, 1},
                     {0, 1, 1},
                     {0, 0, 1}};

        geom::geom k;

        WHEN("Ispisuje se na neki tok"){
            std::ostringstream s;

            CHECK(s << g);
            CHECK(s.str() == g.str());

            s.str("");
            s.clear();

            CHECK(s << k);
            REQUIRE(s.str() == k.str());
        }
    }
}

SCENARIO("Moguce je upisati transformaciju sa ulaznog toka", "[ulaz]"){
    GIVEN("Spreman ulazni tok"){
        std::istringstream s1("1 2 3 "
                              "4 5 6 "
                              "0 0 1");

        std::istringstream s2("1 2 3 "
                              "4 5 6 "
                              "7 8 9");

        std::istringstream s3("1 2 3 "
                              "4 5 6");

        std::istringstream s4("1, 2, 3,"
                              "4, 5, 6,"
                              "0, 0, 1");

        geom::geom k;

        WHEN("Upisuje se sa ispravnog toka"){
            REQUIRE(static_cast<bool>(s1 >> k));
        }

        WHEN("Upisuje se sa neispravnog toka"){
            CHECK_FALSE(static_cast<bool>(s2 >> k));
            CHECK_FALSE(static_cast<bool>(s3 >> k));
            REQUIRE_FALSE(static_cast<bool>(s4 >> k));
        }
    }
}

SCENARIO("Moguce je upisati transformaciju preko citaca", "[izlaz]"){
    GIVEN("Bilo koja transformacija"){
        geom::geom g;

        WHEN("Sve je u redu"){
            REQUIRE_NOTHROW(g << 1, 2, 3,
                                 4, 5, 6,
                                 0, 0, 1);
        }

        WHEN("Ulaz nije korektan"){
            CHECK_THROWS_AS((g << 1, 2, 3,
                                  4, 5, 6,
                                  0, 0, 2),
                            geom::Exc);

            CHECK_THROWS_AS((g << 1, 2, 3,
                                  4, 5, 6,
                                  1, 0, 1),
                            geom::Exc);

            REQUIRE_THROWS_AS((g << 1, 2, 3,
                                    4, 5, 6,
                                    0, 0, 1, 1),
                              geom::Exc);
        }
    }
}

SCENARIO("Moguce je mnozenje numerickom vrednoscu", "[mult]"){
    GIVEN("Neki brojevi i transformacije"){
        geom::geom g;

        CHECK(2*g == g*2);

        CHECK(2.5*g == geom::geom{{2.5,  0,   0},
                                  { 0,  2.5,  0},
                                  { 0,   0,   1}});

        g *= 2;

        REQUIRE(g == 2*geom::geom());
    }
}
