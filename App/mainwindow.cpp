#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geom_graphics_view.hpp"

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

