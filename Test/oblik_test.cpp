#include "catch.hpp"

#include "../Main/geom.hpp"

SCENARIO("Moguce je konstruisati mnogougao", "[poly]"){
    GIVEN("Niz tacaka"){
        std::vector<geom::tacka> niz = {{ 1.,  1.},
                                        {-1., -1.},
                                        { 1., -1.},
                                        {-1.,  1.},
                                        { 1.,  1.},
                                        {0.5, 0.5},
                                        {0.7, 0.9}};

        WHEN("Pokusa se konstrukcija"){
            REQUIRE_NOTHROW(geom::poly(niz));
            const auto a = geom::poly(niz);
            CHECK(a.tacke() == niz);

            REQUIRE_NOTHROW(geom::poly(std::move(niz)));
            const auto b = a;
            CHECK(a.tacke() == b.tacke());
            REQUIRE(niz.empty());
        }

        WHEN("Barata se otvorenoscu poligona"){
            geom::poly a;
            CHECK_FALSE(a.zatvoren());

            a.zatvori();
            CHECK(a.zatvoren());

            a.zatvori();
            CHECK(a.zatvoren());

            a.otvori();
            CHECK_FALSE(a.zatvoren());

            a.otvori();
            REQUIRE_FALSE(a.zatvoren());
        }

        WHEN("Dodaju se tacke"){
            geom::poly a;
            CHECK(a.tacke().empty());

            geom::tacka t;
            a.dodaj(t);

            CHECK(a.tacke().size() == 1);
            CHECK(a.tacke().front() == t);

            a.dodaj(std::move(t));
            CHECK(a.tacke().size() == 2);
            REQUIRE(t.mat().empty());
        }

        WHEN("Pravi se konveksni omot"){
            geom::poly a(niz);
            CHECK(a.tacke().size() == 7);

            a.omot();
            CHECK(a.tacke().size() == 4);

            geom::PodTip niz{1, 2, 3};
            niz = util::listcomp(niz,
                                 [](const auto& x){return x>2;},
                                 [](auto& x){x+=2;});
            CHECK(niz == geom::PodTip{5});

            const geom::Tip rez{{1, 2}};

            geom::Tip niz1{{1, 2}};
            niz1 = util::konv_omot(niz1);
            CHECK(niz1 == rez);

            geom::Tip niz2{{1, 2}, {1, 2}};
            niz2 = util::konv_omot(niz2);
            REQUIRE(niz2 == rez);

            geom::Tip niz3{};
            niz3 = util::konv_omot(niz3);
            REQUIRE(niz3 == geom::Tip{});
        }

        WHEN("Primenjuje se transformacija"){
            geom::poly o({{3, 0, 3}, {1, 1}});
            CHECK(o.tacke()[0] == geom::tacka{1, 0});
            CHECK(o.tacke()[1] == geom::tacka{1, 1});

            o.transformisi(geom::skal(1, 2));
            CHECK(o.tacke()[0] == geom::tacka{1, 0});
            REQUIRE(o.tacke()[1] == geom::tacka{1, 2});
        }
    }
}


SCENARIO("Moguce je konstruisati pravilan mnogougao", "[ppoly]"){
    GIVEN("Parametri mnogougla"){
        const geom::tacka t;
        const double r = 3;

        WHEN("Radi se sa trouglom"){
            REQUIRE_NOTHROW(geom::ppoly(t, 3, r));
            const auto a = geom::ppoly(t, 3, r);
            CHECK(a.tacke().size() == 3);

            std::vector<geom::tacka> tt{{  0,   1.73},
                                        {-1.5, -0.87},
                                        { 1.5, -0.87}};

            bool jed = true;
            for (unsigned i = 0; i < a.tacke().size(); i++){
                jed &= std::abs(a.tacke()[i][0] - tt[i][0]) < 1e-2;
                jed &= std::abs(a.tacke()[i][1] - tt[i][1]) < 1e-2;
            }
            REQUIRE(jed);
        }

        WHEN("Radi se sa cetvorouglom"){
            REQUIRE_NOTHROW(geom::ppoly(t, 4, r));
            const auto a = geom::ppoly(t, 4, r);
            CHECK(a.tacke().size() == 4);

            std::vector<geom::tacka> tt{{ 1.5,  1.5},
                                        {-1.5,  1.5},
                                        {-1.5, -1.5},
                                        { 1.5, -1.5}};

            bool jed = true;
            for (unsigned i = 0; i < a.tacke().size(); i++){
                jed &= std::abs(a.tacke()[i][0] - tt[i][0]) < 1e-2;
                jed &= std::abs(a.tacke()[i][1] - tt[i][1]) < 1e-2;
            }
            REQUIRE(jed);
        }
    }
}

SCENARIO("Moguce je konstruisati elipsu", "[elipsa]"){
    GIVEN("Parametri elipse"){
        const geom::tacka t{0, 0};
        const geom::tacka t1{1, 0};
        const geom::tacka t2{0, 3};

        WHEN("Radi se sa osama"){
            REQUIRE_NOTHROW(geom::elipsa(t, 1, 3));
            const auto a = geom::elipsa(t, 1, 3);
            REQUIRE(a.tacke() == std::vector<geom::tacka>{t, t1, t2});
        }

        WHEN("Radi se sa tackama"){
            REQUIRE_NOTHROW(geom::elipsa(t, t1, t2));
            const auto a = geom::elipsa(t, t1, t2);
            REQUIRE(a.tacke() == std::vector<geom::tacka>{t, t1, t2});
        }
    }
}

SCENARIO("Moguce je konstruisati krug", "[krug]"){
    GIVEN("Parametri kruga"){
        const geom::tacka t{0, 0};
        const geom::tacka t1{1, 0};

        WHEN("Radi se sa osom"){
            REQUIRE_NOTHROW(geom::krug(t, 1));
            const auto a = geom::krug(t, 1);
            REQUIRE(a.tacke() == std::vector<geom::tacka>{t, t1});
        }

        WHEN("Radi se sa tackama"){
            REQUIRE_NOTHROW(geom::krug(t, t1));
            const auto a = geom::krug(t, t1);
            REQUIRE(a.tacke() == std::vector<geom::tacka>{t, t1});
        }
    }
}
