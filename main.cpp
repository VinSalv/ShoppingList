/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file main.cpp
 *
 *  @brief Start the program.
 *
 *  This file contains istructions to start the program.
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
