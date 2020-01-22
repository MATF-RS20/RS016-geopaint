#include "catch.hpp"

#include <sstream>

#include "../Cons/geom.hpp"

SCENARIO("Moguce je konstruisati tacke", "[ctor]"){
    SECTION("Konstruktor bez argumenta"){
        const geom::tacka t;

        const geom::Vektor k{0, 0, 1};

        REQUIRE(t.mat() == k);
    }

    SECTION("Konstruktor kopije"){
        geom::tacka t{1, 2, 3};

        const auto k(t);

        CHECK(k.mat() == t.mat());

        const auto b = k;

        CHECK(k.mat() == b.mat());
        CHECK(t.mat() == b.mat());

        const auto a = std::move(t);

        CHECK(k.mat() == a.mat());
        CHECK(b.mat() == a.mat());

        REQUIRE(t.mat().empty());
    }

    GIVEN("Korektan vektor, dva ili tri broja"){
        geom::Matrica mat{{1, 1, 1}};
        geom::Matrica mat1{{1, 1}};
        geom::Vektor vek{1, 1, 1};
        geom::Vektor vek1{1, 1};

        WHEN("L-value"){
            REQUIRE_NOTHROW(geom::tacka(vek[0], vek[1]));
            REQUIRE_NOTHROW(geom::tacka(vek[0], vek[1], vek[2]));
            REQUIRE_NOTHROW(geom::tacka(mat));
            REQUIRE_NOTHROW(geom::tacka(mat1));
            REQUIRE_NOTHROW(geom::tacka(vek));
            REQUIRE_NOTHROW(geom::tacka(vek1));

            CHECK(geom::tacka(vek[0], vek[1]).mat() == vek);
            CHECK(geom::tacka(vek[0], vek[1], vek[2]).mat() == vek);
            CHECK(geom::tacka(mat).mat() == mat[0]);
            CHECK(geom::tacka(mat1).mat() == mat[0]);
            CHECK(geom::tacka(vek).mat() == vek);
            REQUIRE(geom::tacka(vek1).mat() == vek);
        }

        WHEN("R-value"){
            CHECK_NOTHROW(geom::tacka(1, 1));
            CHECK_NOTHROW(geom::tacka(1, 1, 1));
            CHECK_NOTHROW(geom::tacka(geom::Matrica{{1, 1, 1}}));
            CHECK_NOTHROW(geom::tacka(geom::Matrica{{1, 1}}));
            CHECK_NOTHROW(geom::tacka(geom::Vektor{1, 1, 1}));
            REQUIRE_NOTHROW(geom::tacka(geom::Vektor{1, 1}));
        }

        WHEN("Move ctor"){
            const geom::tacka t1(std::move(mat));
            REQUIRE(mat[0].empty());

            const geom::tacka t2(std::move(mat1));
            REQUIRE(mat[0].empty());

            const geom::tacka t3(std::move(vek));
            REQUIRE(vek.empty());

            const geom::tacka t4(std::move(vek1));
            REQUIRE(vek1.empty());
        }
    }

    GIVEN("Nekorektan vektor ili tri broja"){
        geom::Matrica mat{{1, 1, 1},
                          {1, 1, 1},
                          {0, 0, 0}};

        geom::Vektor vek{1, 1, 0};

        geom::Vektor vek1{1, 1, 1, 1};

        WHEN("L-value"){
            CHECK_THROWS_AS(geom::tacka(mat), geom::Exc);
            CHECK_THROWS_AS(geom::tacka(vek), geom::Exc);
            CHECK_THROWS_AS(geom::tacka(vek[0], vek[1], vek[2]), geom::Exc);
            REQUIRE_THROWS_AS(geom::tacka(vek1), geom::Exc);
        }

        WHEN("R-value"){
            CHECK_THROWS_AS(geom::tacka(geom::Matrica{{1, 1, 1},
                                                     {1, 1, 1},
                                                     {0, 0, 0}}),
                            geom::Exc);
            CHECK_THROWS_AS(geom::tacka(geom::Vektor{1, 1, 0}),
                            geom::Exc);
            CHECK_THROWS_AS(geom::tacka(1, 1, 0), geom::Exc);
            REQUIRE_THROWS_AS(geom::tacka(geom::Vektor{1, 1, 1, 1}),
                              geom::Exc);
        }

        WHEN("Move ctor"){
            REQUIRE_THROWS_AS(geom::tacka(std::move(mat)),
                              geom::Exc);
            CHECK(mat[0].empty());

            REQUIRE_THROWS_AS(geom::tacka(std::move(vek)),
                              geom::Exc);
            CHECK(vek.empty());

            REQUIRE_THROWS_AS(geom::tacka(std::move(vek1)),
                              geom::Exc);
            REQUIRE(vek1.empty());
        }
    }
}

SCENARIO("Moguce je predstaviti tacku u obliku niske", "[str]"){
    GIVEN("Neka tacka"){
        geom::tacka t{1, 0, 1};

        WHEN("Sve je uobicajeno"){
            REQUIRE(t.str() == "[1.00, 0.00, 1.00]");
        }

        WHEN("Tacki je uzeta implementacija"){
            auto k = std::move(t);

            THEN("Niska je prazna"){
                REQUIRE(t.str() == "[]");
            }
        }
    }
}

SCENARIO("Moguce je dodeljivati tacke", "[dodela]"){
    GIVEN("Vec postojece tacke"){
        geom::tacka t1{1, 0, 1};

        geom::tacka t2;

        WHEN("Dodeljuje se tacka"){
            REQUIRE_NOTHROW(t2 = t1);
            CHECK(t2 == t1);

            REQUIRE_NOTHROW(t2 = std::move(t1));
            REQUIRE(t1.mat().empty());
        }
    }

    GIVEN("Korektan vektor ili matrica"){
        geom::Matrica mat{{1, 1, 1}};
        geom::Matrica mat1{{1, 1}};
        geom::Vektor vek{1, 1, 1};
        geom::Vektor vek1{1, 1};

        geom::tacka t;

        WHEN("Dodeljuje se matrica"){
            REQUIRE_NOTHROW(t = mat);
            CHECK(t.mat() == mat[0]);
            CHECK(t == geom::tacka(mat));

            REQUIRE_NOTHROW(t = mat1);
            CHECK(t.mat() == mat[0]);
            CHECK(t == geom::tacka(mat1));

            REQUIRE_NOTHROW(t = std::move(mat));
            CHECK(mat[0].empty());

            REQUIRE_NOTHROW(t = std::move(mat1));
            CHECK(mat1[0].empty());

            CHECK_NOTHROW(t = geom::Matrica{{1, 1, 1}});
            REQUIRE_NOTHROW(t = geom::Matrica{{1, 1}});
        }

        WHEN("Dodeljuje se vektor"){
            REQUIRE_NOTHROW(t = vek);
            CHECK(t.mat() == vek);
            CHECK(t == geom::tacka(vek));

            REQUIRE_NOTHROW(t = vek1);
            CHECK(t.mat() == vek);
            CHECK(t == geom::tacka(vek1));

            REQUIRE_NOTHROW(t = std::move(vek));
            CHECK(vek.empty());

            REQUIRE_NOTHROW(t = std::move(vek1));
            CHECK(vek1.empty());

            CHECK_NOTHROW(t = geom::Vektor{1, 1, 1});
            REQUIRE_NOTHROW(t = geom::Vektor{1, 1});
        }
    }

    GIVEN("Nekorektni vektori ili matrice"){
        geom::Matrica a{{1, 0, 1},
                        {1, 0, 1},
                        {1, 0, 1}};

        geom::Matrica b{{1, 0, 0}};
        geom::Matrica c{{1, 0, 1, 1}};

        geom::Vektor d{1, 0, 0};
        geom::Vektor e{1, 0, 1, 1};

        geom::tacka t;

        WHEN("Dodeljuje se matrica"){
            CHECK_THROWS_AS(t = a, geom::Exc);
            CHECK_THROWS_AS(t = b, geom::Exc);
            CHECK_THROWS_AS(t = c, geom::Exc);
            CHECK_THROWS_AS(t = d, geom::Exc);
            CHECK_THROWS_AS(t = e, geom::Exc);

            REQUIRE_THROWS_AS(t = std::move(a), geom::Exc);
            CHECK(a[0].empty());

            REQUIRE_THROWS_AS(t = std::move(b), geom::Exc);
            CHECK(b[0].empty());

            REQUIRE_THROWS_AS(t = std::move(c), geom::Exc);
            CHECK(c[0].empty());

            REQUIRE_THROWS_AS(t = std::move(d), geom::Exc);
            CHECK(d.empty());

            REQUIRE_THROWS_AS(t = std::move(e), geom::Exc);
            REQUIRE(e.empty());
        }
    }
}

SCENARIO("Moguca je aritmetika sa tackama", "[arithm]"){
    GIVEN("Neke tacke"){
        geom::tacka t1{1, 2};
        const geom::tacka t2(t1);

        WHEN("Unarne operacije"){
            CHECK(t1 == +t1);
            CHECK(+t1 == +t1);
            CHECK(+t1 == t1);
            CHECK(t1 == +t2);

            CHECK(-t1 == -t1);
            CHECK(t1 == -(-t1));
            CHECK(-t1 == -(-(-t1)));
            REQUIRE(-t1 == -t2);
        }

        WHEN("Sabiranje"){
            CHECK(t1+t1 == t1+t2);
            CHECK(t2+t1 == t1+t2);
            CHECK(t1+t1 == 2*t1);

            t1 += t2;

            CHECK(t1 == 2*t2);
            REQUIRE(t1/2 == t2);
        }

        WHEN("Oduzimanje"){
            CHECK(t1-t1 == t1-t2);
            CHECK(t2-t1 == t1-t2);
            CHECK(t1-t1 == 0*t1);

            t1 -= t2;

            CHECK(t1 == 0*t2);
            CHECK(t1+t2 == t2);
            REQUIRE(t1/2 == geom::tacka());
        }
    }
}

SCENARIO("Moguce je indeksirati tacke", "[index]"){
    GIVEN("Tacka i niz indeksa"){
        const geom::tacka t;

        const auto i = GENERATE_REF(0ull, std::size(t)/2, std::size(t)-1);
            WHEN("Postoji indeks"){
                CHECK_NOTHROW(t[i]);
            }

        const auto niz = {std::size(t), std::size(t)+1, std::size(t)+100};
        for (const auto& i : niz){
            DYNAMIC_SECTION("Iteracija: " << i){
                CHECK_THROWS_AS(t[i], std::out_of_range);
            }
        }
    }
}

SCENARIO("Moguce je ispisati tacku na izlazni tok", "[izlaz]"){
    GIVEN("Vec postojeca tacka"){
        const geom::tacka t1{1, 0, 1};

        const geom::geom t2;

        WHEN("Ispisuje se na neki tok"){
            std::ostringstream s;

            CHECK(s << t1);
            CHECK(s.str() == t1.str());

            s.str("");
            s.clear();

            CHECK(s << t2);
            REQUIRE(s.str() == t2.str());
        }
    }
}

SCENARIO("Moguce je upisati tacku sa ulaznog toka", "[ulaz]"){
    GIVEN("Spreman ulazni tok"){
        std::istringstream s1("1 2");

        std::istringstream s2("1 2 3");

        std::istringstream s3("1 2 0");

        geom::tacka t;

        WHEN("Upisuje se sa ispravnog toka"){
            REQUIRE(static_cast<bool>(s1 >> t));
            CHECK(t.mat() == geom::Vektor{1, 2, 1});

            REQUIRE(static_cast<bool>(s2 >> t));
            REQUIRE(t.mat() == geom::Vektor{1./3, 2./3, 1});
        }

        WHEN("Upisuje se sa neispravnog toka"){
            REQUIRE_FALSE(static_cast<bool>(s3 >> t));
            REQUIRE(t.mat().empty());
        }
    }
}

SCENARIO("Moguce je upisati tacku preko citaca", "[izlaz]"){
    GIVEN("Bilo koja transformacija"){
        geom::tacka t;

        WHEN("Sve je u redu"){
            REQUIRE_NOTHROW(t << 1);
            CHECK(t.mat() == geom::Vektor{1, 0, 1});

            REQUIRE_NOTHROW(t << 1, 2);
            CHECK(t.mat() == geom::Vektor{1, 2, 1});

            REQUIRE_NOTHROW(t << 1, 2, 3);
            REQUIRE(t.mat() == geom::Vektor{1./3, 2./3, 1});
        }

        WHEN("Ulaz nije korektan"){
            CHECK_THROWS_AS((t << 1, 2, 0),
                            geom::Exc);
            CHECK(t.mat().empty());

            CHECK_THROWS_AS((t << 1, 2, 3, 1),
                            geom::Exc);
            REQUIRE(t.mat().empty());
        }
    }
}

SCENARIO("Moguce je mnozenje matrice i tacke", "[mult]"){
    GIVEN("Neko preslikavanje i tacka"){
        const geom::tacka t(2, 1);

        const geom::trans tt(2, 3);
        CHECK(tt*t == geom::tacka{4, 4});

        const geom::skal s(2, 3);
        CHECK(s*t == geom::tacka{4, 3});

        const geom::rot r(90, {3, 1});
        CHECK(r*t == geom::tacka{3, 0});

        const geom::geom g;
        REQUIRE(g*t == t);
    }
}
