#include "catch.hpp"

#include "../Main/geom.hpp"

SCENARIO("Moguce je konstruisati translaciju", "[trans]"){
    GIVEN("Dva realna broja i bool"){
        auto x = GENERATE_REF(-13.5, 0., 1., 1.5, 100.);
            WHEN("Postoji par brojeva"){
                auto y = GENERATE_REF(-12., -7.1, 0., 1., 17.);
                    WHEN("Postoji i bool"){
                        auto inv = GENERATE_REF(true, false);

                        const geom::trans t(x, y, inv);
                        CHECK(t == geom::trans({x, y}, inv));

                        if (inv){
                            REQUIRE(t == geom::geom{{1, 0, -x},
                                                    {0, 1, -y},
                                                    {0, 0,  1}});
                        } else {
                            REQUIRE(t == geom::geom{{1, 0, x},
                                                    {0, 1, y},
                                                    {0, 0, 1}});
                        }
                    }
            }
    }
}

SCENARIO("Moguce je konstruisati skaliranje", "[skal]"){
    GIVEN("Dva realna broja i bool"){
        auto x = GENERATE_REF(-13.5, 0., 1., 1.5, 100.);
            WHEN("Postoji par brojeva"){
                auto y = GENERATE_REF(-12., -7.1, 0., 1., 17.);
                    WHEN("Postoji i bool"){
                        auto inv = GENERATE_REF(true, false);

                        if (inv && x*y == 0.){
                            REQUIRE_THROWS_AS(geom::skal(x, y, {0, 0}, inv), geom::Exc);
                        } else if (inv){
                            geom::skal t(x, y, {0, 0}, inv);

                            CHECK(t == geom::skal({x, y}, {0, 0}, inv));

                            REQUIRE(t == geom::geom{{1/x, 0,  0},
                                                    { 0, 1/y, 0},
                                                    { 0,  0,  1}});
                        } else {
                            geom::skal t(x, y, {0, 0}, inv);
                            REQUIRE(t == geom::geom{{x, 0, 0},
                                                    {0, y, 0},
                                                    {0, 0, 1}});
                        }
                    }
            }
    }

    GIVEN("Centar transformacije"){
        const auto x = 1;
        const auto y = 2;
        const geom::tacka t(1, 1);

        CHECK(geom::skal(x, y, t) == geom::trans(t) *
                                     geom::geom{{x, 0, 0},
                                                {0, y, 0},
                                                {0, 0, 1}} *
                                     geom::trans(t, true));

        CHECK(geom::skal(x, y, t) == geom::skal(x, y, t[0], t[1]));
        CHECK(geom::skal(x, y, t) == geom::skal({x, y}, t[0], t[1]));
        REQUIRE(geom::skal(x, y, t) == geom::skal({x, y}, t));
    }
}

SCENARIO("Moguce je konstruisati smicanje", "[smic]"){
    GIVEN("Dva realna broja i bool"){
        auto x = GENERATE_REF(-13.5, -0.5, 0., 1., 1.5, 100.);
            WHEN("Postoji par brojeva"){
                auto y = GENERATE_REF(-12., -2, 0., 1., 17.);
                    WHEN("Postoji i bool"){
                        auto inv = GENERATE_REF(true, false);

                        if (inv && x*y == 1.){
                            REQUIRE_THROWS_AS(geom::smic(x, y, {0, 0}, inv), geom::Exc);
                        } else if (inv){
                            geom::smic t(x, y, {0, 0}, inv);

                            CHECK(t == geom::smic({x, y}, {0, 0}, inv));

                            REQUIRE(t == 1/(1-x*y) * geom::geom{{ 1, -x,  0},
                                                                {-y,  1,  0},
                                                                { 0,  0,  1}});
                        } else {
                            geom::smic t(x, y, {0, 0}, inv);

                            CHECK(t == geom::smic({x, y}, {0, 0}, inv));

                            REQUIRE(t == geom::geom{{1, x, 0},
                                                    {y, 1, 0},
                                                    {0, 0, 1}});
                        }
                    }
            }
    }

    GIVEN("Centar transformacije"){
        const auto x = 1;
        const auto y = 2;
        const geom::tacka t(1, 1);

        CHECK(geom::smic(x, y, t) == geom::trans(t) *
                                     geom::geom{{1, x, 0},
                                                {y, 1, 0},
                                                {0, 0, 1}} *
                                     geom::trans(t, true));

        CHECK(geom::smic(x, y, t) == geom::smic(x, y, t[0], t[1]));
        CHECK(geom::smic(x, y, t) == geom::smic({x, y}, t[0], t[1]));
        REQUIRE(geom::smic(x, y, t) == geom::smic({x, y}, t));
    }
}

SCENARIO("Moguce je konstruisati rotaciju", "[rot]"){
    GIVEN("Realan broj i bool"){
        auto u = GENERATE_REF(-13.5, 0., 1., 1.5, 100.);
            WHEN("Postoji i bool"){
                auto inv = GENERATE_REF(true, false);

                const geom::rot t(u, {0, 0}, inv);
                const auto pom1 = cos(util::deg2rad(u));
                const auto pom2 = sin(util::deg2rad(u));

                if (inv){
                    REQUIRE(t == geom::geom{{ pom1, pom2,  0},
                                            {-pom2, pom1,  0},
                                            {  0,    0,    1}});
                } else {
                    REQUIRE(t == geom::geom{{pom1, -pom2,  0},
                                            {pom2,  pom1,  0},
                                            { 0,     0,    1}});
                }
            }
    }

    GIVEN("Centar transformacije"){
        const auto u = 90;
        const geom::tacka t(1, 1);
        const auto pom1 = cos(util::deg2rad(u));
        const auto pom2 = sin(util::deg2rad(u));

        CHECK(geom::rot(u, t) == geom::trans(t) *
                                  geom::geom{{pom1, -pom2,  0},
                                             {pom2,  pom1,  0},
                                             { 0,     0,    1}} *
                                  geom::trans(t, true));

        REQUIRE(geom::rot(u, t) == geom::rot(u, t[0], t[1]));
    }
}

SCENARIO("Moguce je konstruisati refleksiju", "[refl]"){
    GIVEN("Realan broj i bool"){
        auto u = GENERATE_REF(-13.5, 0., 1., 1.5, 100.);
            WHEN("Postoji i bool"){
                auto inv = GENERATE_REF(true, false);

                const geom::refl t(u, {0, 0}, inv);
                const auto pom1 = cos(util::deg2rad(2*u));
                const auto pom2 = sin(util::deg2rad(2*u));

                REQUIRE(t == geom::geom{{pom1,  pom2,  0},
                                        {pom2, -pom1,  0},
                                        { 0,     0,    1}});
            }
    }

    GIVEN("Centar transformacije"){
        const auto u = 90;
        const geom::tacka t(1, 1);
        const auto pom1 = cos(util::deg2rad(2*u));
        const auto pom2 = sin(util::deg2rad(2*u));

        CHECK(geom::refl(u, t) == geom::trans(t) *
                                  geom::geom{{pom1,  pom2,  0},
                                             {pom2, -pom1,  0},
                                             { 0,     0,    1}} *
                                  geom::trans(t, true));

        CHECK(geom::refl(u, t, true) == geom::refl(u, t, false));

        REQUIRE(geom::refl(u, t) == geom::refl(u, t[0], t[1]));
    }
}
