/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file main.cpp
 *
 *  @brief Start program.
 *
 *  This file contains istructions to start program.
 */

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
