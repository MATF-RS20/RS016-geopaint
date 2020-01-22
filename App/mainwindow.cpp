#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geom_graphics_view.hpp"

#include "../Cons/geom.hpp"
#include "geom_graphics_view.hpp"
#include "crtanje.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::posalji_poruku(QString s)
{
    ui->statusbar->showMessage(s);
}

// FIXME: NEISPRAVNI ULAZI!!! **********
void MainWindow::on_pb_poligon_clicked()
{
    geom::poly p;
    auto s_tacke = ui->le_tacke->text().split(" ");

    for(auto s_tacka: s_tacke)
    {
        auto koordinate = s_tacka.split(",");
        geom::tacka t (koordinate[0].toFloat(), koordinate[1].toFloat());
        p.dodaj(t);
    }
    if(ui->cb_zatvorena_linija->isChecked())
        p.zatvori();
    ui->graphicsView->nacrtaj_poligon(p);
}

// FIXME: NEISPRAVNI ULAZI!!! **********
void MainWindow::on_pb_elipsa_clicked()
{
    auto s_koordinate_centra = ui->le_tacke->text().split(",");
    geom::tacka centar (s_koordinate_centra[0].toFloat(), s_koordinate_centra[1].toFloat());
    auto s_poluose = ui->le_poluprecnik->text().split(" ");
    auto a = s_poluose[0].toFloat();
    auto b = s_poluose[1].toFloat();

    geom::elipsa e (centar, a, b);
    ui->graphicsView->nacrtaj_elipsu(e);
}

// FIXME: NEISPRAVNI ULAZI!!! **********
void MainWindow::on_pb_krug_clicked()
{
    auto s_koordinate_centra = ui->le_tacke->text().split(",");
    geom::tacka centar (s_koordinate_centra[0].toFloat(), s_koordinate_centra[1].toFloat());
    auto s_poluprecnik = ui->le_poluprecnik->text().split(" ");
    auto r = s_poluprecnik[0].toFloat();

    geom::krug k (centar, r);
    ui->graphicsView->nacrtaj_krug(k);
}

void MainWindow::on_pb_ocisti_ekran_clicked()
{
    auto graficki_elementi = ui->graphicsView->items();

    for(auto element: graficki_elementi)
    {
        if (dynamic_cast<crtanje::cxosa*>(element) == nullptr
         && dynamic_cast<crtanje::cyosa*>(element) == nullptr)
            ui->graphicsView->scene()->removeItem(element);
    }
}

// Resetujemo sve podatke za transformacije
void MainWindow::on_pushButton_clicked()
{
    ui->le_rotacija_x->setText("");
    ui->le_rotacija_y->setText("");
    ui->le_ugao->setText("");
    ui->cb_rotacija->setChecked(false);

    ui->le_translacija_x->setText("");
    ui->le_translacija_y->setText("");
    ui->cb_translacija->setChecked(false);

    ui->le_skaliranje_x->setText("");
    ui->le_skaliranje_y->setText("");
    ui->le_skaliranje_tacka->setText("");
}

// FIXME: NEISPRAVNI ULAZI!!! **********
void MainWindow::on_pb_primeni_transformacije_clicked()
{
    std::vector<geom::geom> transformacije;
    if (ui->cb_rotacija->isChecked())
    {
         float ugao = ui->le_ugao->text().toFloat(); // TODO: Obrada neispravnih ulaza
         float x = ui->le_rotacija_x->text().toFloat();
         float y = ui->le_rotacija_y->text().toFloat();

         transformacije.emplace_back(geom::rot(ugao, x ,y));
    }

    if (ui->cb_translacija->isChecked())
    {
         float x = ui->le_translacija_x->text().toFloat(); // TODO: Obrada neispravnih ulaza
         float y = ui->le_translacija_y->text().toFloat();

         transformacije.emplace_back(geom::trans(x ,y));
    }

    if (ui->cb_skaliranje->isChecked())
    {
        float x = ui->le_skaliranje_x->text().toFloat(); // TODO: Obrada neispravnih ulaza
        float y = ui->le_skaliranje_y->text().toFloat();

        auto s_tacke = ui->le_skaliranje_tacka->text().split(",");
        geom::tacka t(s_tacke[0].toFloat(), s_tacke[1].toFloat());

        transformacije.emplace_back(geom::skal(x,y,t));
    }

    // Ako nijedna opcija nije cekirana, resetovacemo sve podatke
    if (transformacije.empty()) {
        on_pushButton_clicked();
        return;
    }

    auto graphicItems = ui->graphicsView->scene()->items();

    // HACK: Trenutno 'dovijanje' za nalazenje imena klase
    // Lose resenje usled nedostatka vremena
    // Lako se moze ispraviti dodavanjem nadklase za sve graficke elemente
    // koje predstavljaju oblik
    for(auto item: graphicItems){
        auto element = dynamic_cast<crtanje::cpoligon*>(item);
        if(element == nullptr){
            auto e1 = dynamic_cast<crtanje::celipsa*>(item);
                if (e1 == nullptr){
                    auto e2 = dynamic_cast<crtanje::ckrug*>(item);

                    if (e2 == nullptr){
                        continue;
                    }
                    // Kada nadjemo tip oblika, uzimamo njegov odgovarajuci geometrijski objekat
                    // i transformisemo ga. Brisemo trenutni graficki element sa
                    // scene i crtamo novi koji odgovara istom objektu sa transformisanim koordinatama.
                    auto odgovarajuci_oblik = e2->odgovarajuci_krug;
                    ui->graphicsView->scene()->removeItem(item);

                    for (auto& transformacija : transformacije){
                        auto kopija = odgovarajuci_oblik;
                        kopija.transformisi(transformacija);
                        ui->graphicsView->nacrtaj_krug(kopija);
                    }
                } else {
                    auto odgovarajuci_oblik = e1->odgovarajuca_elipsa;
                    ui->graphicsView->scene()->removeItem(item);

                    for (auto& transformacija : transformacije){
                        auto kopija = odgovarajuci_oblik;
                        kopija.transformisi(transformacija);
                        ui->graphicsView->nacrtaj_elipsu(kopija);
                    }
                }
        } else {
            auto odgovarajuci_oblik = element->odgovarajuci_poligon;
            ui->graphicsView->scene()->removeItem(item);

            for (auto& transformacija : transformacije){
                auto kopija = odgovarajuci_oblik;
                kopija.transformisi(transformacija);
                ui->graphicsView->nacrtaj_poligon(kopija);
            }
        }
    }
}
