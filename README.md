# GeoPaint :triangular_ruler::paintbrush:
Projekat na kursu Razvoj softvera – program za crtanje oblika u dvodimenzionom koordinatnom sistemu i vizuelizaciju geometrijskih transformacija. Korisnik pomoću elemenata grafičkog korisničkog interfejsa zadaje oblike u ravni, nad kojima zatim vrši proizvoljne afine transformacije: translaciju, rotaciju, refleksiju, skaliranje, smicanje. Sedmični izveštaji o toku izrade projekta, kao i budućim planovima na nivou nedelje, dostupni su na [wiki](https://github.com/MATF-RS20/RS016-geopaint/wiki) stranicama repozitorijuma.

<img src = "https://giwiz-content.c.yimg.jp/im_siggp.zARDh5mmSGAAh71nx1Vg---priy-x703-y470-yc73-xc0-hc393-wc700-n1/r/iwiz-amd/20190928-00019799-mimollet-000-1-view.jpg" width = 450px />

## Podešavanje :memo:
Nakon kloniranja (`git clone https://github.com/MATF-RS20/RS016-geopaint`) ili nekog drugog načina preuzimanja repozitorijuma, program je, odabirom odgovarajućeg PRO fajla, moguće učitati kao projekat pomoću [QtCreator](https://www.qt.io/offline-installers)-a.

Primera radi, ukoliko se želi učitavanje celog projekta, napravljenog pomoću „subdirs“ šablona, potrebno je odabrati GeoPaint.pro. Za glavnu aplikaciju treba učitati App.pro, za konzolnu Cons.pro, a za testove Test.pro.

Korišćeni jezik je C++, dok su od biblioteka korišćeni standardni moduli jezika, kao i prikladne Qt biblioteke za [grafičku scenu](https://doc.qt.io/qt-5/qgraphicsscene.html) i ostale delove grafičkog korisničkog interfejsa. Za testove je korišćena header-only biblioteka [Catch2](https://github.com/catchorg/Catch2).

## Članovi tima :girl::girl::boy:
* [Milena Stojić](https://github.com/kate-97) – mi16096@alas.matf.bg.ac.rs
* [Jelena Milivojević](https://github.com/ratspeaker) – mi16004@alas.matf.bg.ac.rs
* [Lazar Vasović](https://github.com/matfija) – mi16099@alas.matf.bg.ac.rs
