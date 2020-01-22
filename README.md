# GeoPaint :triangular_ruler::paintbrush:
Projekat na kursu Razvoj softvera – program za crtanje oblika u dvodimenzionom koordinatnom sistemu i vizuelizaciju geometrijskih transformacija. Korisnik pomoću elemenata grafičkog korisničkog interfejsa zadaje oblike u ravni, nad kojima zatim vrši proizvoljne afine transformacije: translaciju, rotaciju, refleksiju, skaliranje, smicanje. Sedmični izveštaji o toku izrade projekta, kao i budućim planovima na nivou nedelje, dostupni su na [wiki](https://github.com/MATF-RS20/RS016-geopaint/wiki) stranicama repozitorijuma.

![](https://raw.githubusercontent.com/MATF-RS20/RS016-geopaint/master/Ostalo/screenshot.png)

## Podešavanje :memo:
Nakon kloniranja (`git clone https://github.com/MATF-RS20/RS016-geopaint`) ili nekog drugog načina preuzimanja repozitorijuma, program je, odabirom odgovarajućeg PRO fajla, moguće učitati kao projekat pomoću [QtCreator](https://www.qt.io/offline-installers)-a.

Primera radi, ukoliko se želi učitavanje celog projekta, napravljenog pomoću „subdirs“ šablona, potrebno je odabrati GeoPaint.pro. Za glavnu aplikaciju treba učitati App.pro, za konzolnu Cons.pro, a za testove Test.pro.

Korišćeni jezik je C++, dok su od biblioteka korišćeni standardni moduli jezika, kao i prikladne Qt biblioteke za [grafičku scenu](https://doc.qt.io/qt-5/qgraphicsscene.html) i ostale delove grafičkog korisničkog interfejsa. Za testove je korišćena header-only biblioteka [Catch2](https://github.com/catchorg/Catch2).

## Članovi tima :girl::boy:
* [Milena Stojić](https://github.com/kate-97) – mi16096@alas.matf.bg.ac.rs
* [Lazar Vasović](https://github.com/matfija) – mi16099@alas.matf.bg.ac.rs
