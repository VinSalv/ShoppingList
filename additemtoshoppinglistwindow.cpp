/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file additemtoshoppinglistwindow.cpp
 *
 *  @brief Implementation of the class AddItemToShoppingListWindow.
 *
 *  This file contains the implementation of the AddItemToShoppingListWindow class.
 */

#include "additemtoshoppinglistwindow.h"
#include "ui_additemtoshoppinglistwindow.h"


AddItemToShoppingListWindow::AddItemToShoppingListWindow(QWidget* parent) : QDialog(parent), ui(new Ui::AddItemToShoppingListWindow){
    // Init AddItemToShoppingListWindow
    ui->setupUi(this);
    setWindowTitle(titleAdditemtoshoppinglistwindow);

    // Update items container
    read_file_from(fileItems, items);

    // Init the AddItemToShoppingListWindow's components
    comboBoxStoredItems = ui->comboBoxStoredItems;
    for (const auto &iteratorItems : items)
        itemsNames.append(QString::fromStdString(iteratorItems.getName()));
    comboBoxStoredItems->addItems(itemsNames);
}

AddItemToShoppingListWindow::~AddItemToShoppingListWindow(){
    // Delete AddItemToShoppingListWindow
    delete ui;
}

void AddItemToShoppingListWindow::on_comboBoxStoredItems_currentIndexChanged(int index){
    // Fetch element from comboBoxStoredItems
    chosenItem = comboBoxStoredItems->itemText(index).toStdString();
}

void AddItemToShoppingListWindow::on_dialogButtonBox_accepted(){
    bool selectedElement = chosenItem != "None";
    if (selectedElement){
        bool itemInStoredItems = false;
        bool numberOfUsesExceedLimit = false;

        Item itemToRemove{}, updatedItem{};
        for (auto iteratorItems : items){
            //Find item in items container
            itemInStoredItems = iteratorItems.getName()==chosenItem;
            if (itemInStoredItems){
                // Init the item to remove
                itemToRemove = iteratorItems;

                // Init the item to update items container
                updatedItem.setName(iteratorItems.getName());

                int number_of_quantities = iteratorItems.getQuantity();
                bool number_of_quantities_exceed_limit = number_of_quantities >= maxNumberOfUse;
                if (number_of_quantities_exceed_limit)
                    updatedItem.setQuantity(maxNumberOfQuantity);
                else
                    updatedItem.setQuantity(++number_of_quantities);

                updatedItem.setIsInShoppingList(true);

                int number_of_uses = iteratorItems.getNumberUse();
                numberOfUsesExceedLimit = number_of_uses >= maxNumberOfUse;
                updatedItem.setNumberUse(++number_of_uses);

                break;
            }
        }
        if (itemInStoredItems){
            // Removal in items container
            items.erase(itemToRemove);

            // Update items container
            items.emplace(updatedItem);
        }

        // Reduce the number of uses of each element by half in order not to exceed the maximum number of integers
        if (numberOfUsesExceedLimit){
            set<Item> editedItems{};
            for (const Item& item :items){
                Item copyItem = item;
                copyItem.reduceNumberUse();
                editedItems.emplace(copyItem);
            }
            items.clear();
            copy(editedItems.begin(), editedItems.end(), inserter(items, items.begin()));
        }

        // Update file
        write_file_from(fileItems, items);
    }

    close();
}

void AddItemToShoppingListWindow::on_dialogButtonBox_rejected(){
    // Close AddItemToShoppingListWindow
    close();
}
