/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file additemtoshoppinglistwindow.h
 *
 *  @brief Declaration of the class AddItemToShoppingListWindow.
 *
 *  This file contains the declaration of the AddItemToShoppingListWindow class.
 */

#ifndef ADDITEMTOSHOPPINGLISTWINDOW_H
#define ADDITEMTOSHOPPINGLISTWINDOW_H

#include <item.h>
#include <QComboBox>
#include <QDialog>


namespace Ui {
class AddItemToShoppingListWindow;
}

class AddItemToShoppingListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemToShoppingListWindow(QWidget *parent = nullptr);
    ~AddItemToShoppingListWindow();

private slots:
    void on_comboBoxStoredItems_currentIndexChanged(int index);

    void on_dialogButtonBox_accepted();

    void on_dialogButtonBox_rejected();

private:
    Ui::AddItemToShoppingListWindow* ui;

    set<Item> items{};

    QComboBox* comboBoxStoredItems{};
    QStringList itemsNames{"None"};

    string chosenItem{};
};

#endif // ADDITEMTOSHOPPINGLISTWINDOW_H
