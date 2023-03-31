/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file mainwindow.h
 *
 *  @brief Declaration of the class MainWindow.
 *
 *  This file contains the declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QListWidget>
#include "storeditemswindow.h"
#include "additemtoshoppinglistwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonAccessToStoredItems_clicked();
    void accessToStoredItemsFinished();

    void on_buttonAddItemToShoppingList_clicked();
    void accessToSelectionItemFinished();

    void on_buttonExit_clicked();

private:
    Ui::MainWindow* ui;

    set<Item> items{};

    QWidget* scrollAreaWidgetShoppingList{};

    QListWidget* listWidgetItemsFrequentlyUsed{};

    QAction* actionSave{};

    QAction* actionExit{};

    QAction* actionAbout{};
};

/**
 *  @brief  Update shopping list layout.
 *
 *  @param  scrollAreaWidgetShoppingList    Shopping list area.
 *  @param  items                           Stored items.
 *
 *  Update shopping list layout adding, removing or editing (quantity) items.
 *
*/
void update_shopping_list(QWidget* scrollAreaWidgetShoppingList, set<Item>& items);

/**
 *  @brief  Generate the widget to add items in shopping list.
 *
 *  @param  items           Stored items.
 *  @param  nameItem        Name of the item.
 *  @param  quantityItem    Quantity of the item.
 *
 *
 *  @return A QWidget* punntator of the generated widget.
 *
 *  Generate the widget to add items in shopping list by using item's information fetched in storedItems.
 *
*/
QWidget* generate_widget_item_from(set<Item>& items,  string nameItem, int quantityItem);

/**
 *  @brief  Update list of frequently used items.
 *
 *  @param  listWidgetItemsFrequentlyUsed    List of frequently used items.
 *  @param  items                            Stored items.
 *
 *  Update ist of frequently used items adding, removing or editing items.
 *
*/
void update_frequently_used_list(QListWidget* listWidgetItemsFrequentlyUsed, set<Item>& items);

#endif // MAINWINDOW_H
