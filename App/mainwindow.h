#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "geom_graphics_view.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void posalji_poruku(QString);

private slots:
    void on_pb_poligon_clicked();

    void on_pb_elipsa_clicked();

    void on_pb_krug_clicked();

    void on_pb_ocisti_ekran_clicked();

    void on_pushButton_clicked();

    void on_pb_primeni_transformacije_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
