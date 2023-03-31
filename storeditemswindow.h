/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file storeditemswindow.h
 *
 *  @brief Declaration of the class StoredItemsWindow.
 *
 *  This file contains the declaration of the StoredItemsWindow class.
 */

#ifndef STOREDITEMSWINDOW_H
#define STOREDITEMSWINDOW_H

#include <item.h>
#include <QDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <set>
#include <iostream>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QRegularExpression>
#include <regex>


namespace Ui {
class StoredItemsWindow;
}

class StoredItemsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StoredItemsWindow(QWidget* parent = nullptr);
    ~StoredItemsWindow();

private slots:

    void on_buttonNewItem_clicked();

    void on_textEditNewItem_textChanged();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::StoredItemsWindow *ui;

    set<Item> items{};

    QWidget* scrollAreaWidgetStoredItems{};

    QPushButton* buttonAddNewItem{};

    QTextEdit* textEditNewItem{};

};

/**
 *  @brief  Update stored items list layout.
 *
 *  @param  scrollAreaWidgetStoredItems    Stored items list area.
 *  @param  items                          Stored items.
 *
 *  Update stored items list layout adding, removing or editing (name) items.
 *
*/
void update_stored_list(QWidget* scrollAreaWidgetStoredItems, set<Item>& items);

/**
 *  @brief  Generate the widget to add items in stored items list.
 *
 *  @param  items       Stored items.
 *  @param  nameItem    Name of the item.
 *
 *  @return A QWidget* puntator of the generated widget.
 *
 *  Generate the widget to add items in stored items list by using item's information fetched in storedItems.
 *
*/
QWidget* generate_widget_item_from(set<Item>& items, string nameItem);

/**
 *  @brief  Format a text.
 *
 *  @param  text    Text.
 *
 *  @return A string of formatted text.
 *
 *  Format a text in order to avoid inserting spaces and transform characters to lowercase.
 *
*/
string get_formatted_text_from(string text);

/**
 *  @brief  Check if a text exists.
 *
 *  @param  textEdit    QTextEdit.
 *
 *  @return A bool to indicate if the text is present in QTextEdit.
 *
 *  Check if a text is present in QTextEdit.
 *
*/
bool text_is_in(const QTextEdit* textEdit);

/**
 *  @brief  Check if an object exists in a set container.
 *
 *  @param  items       Set Container of objects Item.
 *  @param  nameItem    Name of the item.
 *
 *  @return A bool to indicate if the set container contain an object Item.
 *
 *   Check if an object Item exists within a set container.
 *
*/
bool item_exist_in(set<Item>items, string nameItem);


#endif // STOREDITEMSWINDOW_H
