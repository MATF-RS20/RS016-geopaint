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

   // ui->statusbar->showMessage("pozzzzzzz", 5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


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

void MainWindow::on_pb_krug_clicked()
{
    auto s_koordinate_centra = ui->le_tacke->text().split(",");
    geom::tacka centar (s_koordinate_centra[0].toFloat(), s_koordinate_centra[1].toFloat());
    auto s_poluprecnik = ui->le_poluprecnik->text().split(" ");
    auto r = s_poluprecnik[0].toFloat();

    geom::krug k (centar, r);
    ui->graphicsView->nacrtaj_krug(k);
}
