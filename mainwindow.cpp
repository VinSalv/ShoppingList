/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file mainwindow.cpp
 *
 *  @brief Implementation of the class MainWindow.
 *
 *  This file contains the implementation of the MainWindow class.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    // Init MainWindow
    ui->setupUi(this);
    setWindowTitle(titleMainwindow);

    // Init items container
    setup_from(fileItems, items);

    // Init the MainWindow's components
    scrollAreaWidgetShoppingList = ui->scrollAreaWidgetShoppingList;

    listWidgetItemsFrequentlyUsed = ui->listWidgetItemsFrequentlyUsed;
    listWidgetItemsFrequentlyUsed->setSelectionMode(QAbstractItemView::SingleSelection);
    // Double click on one of the most frequently used item to add it to the shopping list
    MainWindow::connect(listWidgetItemsFrequentlyUsed, &QListWidget::itemDoubleClicked, this, [&](QListWidgetItem* item){
        string chosenItem = item->text().toStdString();

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

                int quantity = iteratorItems.getQuantity();
                bool quantityExceedLimit = quantity >= maxNumberOfUse;
                if (quantityExceedLimit)
                    updatedItem.setQuantity(maxNumberOfQuantity);
                else
                    updatedItem.setQuantity(++quantity);

                updatedItem.setIsInShoppingList(true);

                int numberOfUses = iteratorItems.getNumberOfUses();
                numberOfUsesExceedLimit = numberOfUses >= maxNumberOfUse;
                updatedItem.setNumberOfUses(++numberOfUses);

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
                copyItem.reduceNumberOfUses();
                editedItems.emplace(copyItem);
            }
            items.clear();
            copy(editedItems.begin(), editedItems.end(), inserter(items, items.begin()));
        }

        // Update graphics shopping list
        update_shopping_list(scrollAreaWidgetShoppingList, items);

        // Update graphics frequently used list
        update_frequently_used_list(listWidgetItemsFrequentlyUsed, items);
    });

    actionSave = ui->actionSave;
    // Click to save items container
    MainWindow::connect(actionSave, &QAction::triggered, this, [&](){
        // Update file
        write_file_from(fileItems, items);
    });

    actionExit = ui->actionExit;
    // Click to exit
    MainWindow::connect(actionExit, &QAction::triggered, this, [&](){
        // Update file
        write_file_from(fileItems, items);

        // Close MainWindow
        exit(0);
    });

    actionAbout = ui->actionAbout;
    // Click to show information about the creator
    MainWindow::connect(actionAbout, &QAction::triggered, this, [&](){
        // Init aboutPopupWindow
        QWidget *aboutPopupWindow = new QWidget;
        aboutPopupWindow->setWindowTitle(titleAboutPopup);
        aboutPopupWindow->setWindowModality(Qt::ApplicationModal);

        // Set aboutPopupWindow's layout
        QVBoxLayout* aboutPopupLayout = new QVBoxLayout(aboutPopupWindow);

        // Init the aboutPopupWindow's components
        QLabel* aboutPopupLabel = new QLabel(QString::fromStdString(textAboutPopup));
        aboutPopupLabel->setFixedSize(widthComponentsPopup, heightComponentsPopup);

        QPushButton* aboutPopupOkButton = new QPushButton(okMessage);
        aboutPopupOkButton->setFixedSize(widthComponentsPopup, heightComponentsPopup);
        // Close aboutPopupWindow
        connect(aboutPopupOkButton, &QPushButton::clicked, aboutPopupWindow, [aboutPopupWindow](){
            aboutPopupWindow->close();
        });

        // Add components to popupLayout
        aboutPopupLayout->addWidget(aboutPopupLabel);
        aboutPopupLayout->addWidget(aboutPopupOkButton);

        // Show popupWindow
        aboutPopupWindow->show();
    });

    //Update graphics shopping list
    update_shopping_list(scrollAreaWidgetShoppingList, items);

    //Update graphics frequently used list
    update_frequently_used_list(listWidgetItemsFrequentlyUsed, items);
}

MainWindow::~MainWindow(){
    // Delete MainWindow
    delete ui;
}

void MainWindow::on_buttonAccessToStoredItems_clicked(){
    // Update file
    write_file_from(fileItems, items);

    // Open StoredItemsWindow
    StoredItemsWindow addStoreWindow(this);
    // Connect StoredItemsWindow closure to a slot
    connect(&addStoreWindow, &StoredItemsWindow::finished, this,  &MainWindow::accessToStoredItemsFinished);
    addStoreWindow.exec();
}

void MainWindow::accessToStoredItemsFinished(){
    // Update items container
    read_file_from(fileItems, items);

    //Update graphics shopping list
    update_shopping_list(scrollAreaWidgetShoppingList, items);

    //Update graphics frequently used list
    update_frequently_used_list(listWidgetItemsFrequentlyUsed, items);
}

void MainWindow::on_buttonAddItemToShoppingList_clicked(){
    // Update file
    write_file_from(fileItems, items);

    // Open AddItemToShoppingListWindow
    AddItemToShoppingListWindow addListWindow(this);
    // Connect AddItemToShoppingListWindow closure to a slot
    connect(&addListWindow, &AddItemToShoppingListWindow::finished, this,  &MainWindow::accessToSelectionItemFinished);
    addListWindow.exec();
}

void MainWindow::accessToSelectionItemFinished(){
    // Update items container
    read_file_from(fileItems, items);

    //Update graphics shopping list
    update_shopping_list(scrollAreaWidgetShoppingList, items);

    //Update graphics frequently used list
    update_frequently_used_list(listWidgetItemsFrequentlyUsed, items);
}

void MainWindow::on_buttonExit_clicked(){
    // Update file
    write_file_from(fileItems, items);

    // Close MainWindow
    exit(0);
}

void update_shopping_list(QWidget* scrollAreaWidgetShoppingList, set<Item>& items){
    // Fatch scrollAreaWidgetShoppingList's layout
    QVBoxLayout* scrollAreaWidgetShoppingListLayout = qobject_cast<QVBoxLayout*> (scrollAreaWidgetShoppingList->layout());
    bool scrollAreaWidgetShoppingListLayoutExist = scrollAreaWidgetShoppingListLayout != nullptr;

    // If scrollAreaWidgetShoppingListLayout exists, remove its components
    if (scrollAreaWidgetShoppingListLayoutExist)
        // Delete scrollAreaWidgetShoppingListLayout's components
        while (scrollAreaWidgetShoppingListLayout->count() > 0) {
            // Fetch first widget from scrollAreaWidgetShoppingListLayout
            QWidget* widget = scrollAreaWidgetShoppingListLayout->itemAt(0)->widget();
            bool widgetExist = widget != nullptr;

            // If scrollAreaWidgetShoppingListLayout's widget exists, remove it
            if (widgetExist) {
                scrollAreaWidgetShoppingListLayout->removeWidget(widget);
                delete widget;
            // Otherwise, delete widget's layout
            }else{
                QLayoutItem* item = scrollAreaWidgetShoppingListLayout->takeAt(0);
                delete item->layout();
                delete item;
            }
        }
    // Otherwise, generate a new QVBoxLayout for scrollAreaWidgetShoppingList
    else scrollAreaWidgetShoppingListLayout = new QVBoxLayout(scrollAreaWidgetShoppingList);

    // Update scrollAreaWidgetShoppingListLayout
    set<Item> copyItems{};
    copy(items.begin(), items.end(), inserter(copyItems, copyItems.begin()));
    for (const auto &iteratorItems : copyItems){
        bool itemInShoppingList = iteratorItems.getIsInShoppingList();
        if (itemInShoppingList){
            // Fetch item's information
            string nameItem = iteratorItems.getName();
            int quantityItem = iteratorItems.getQuantity();

            // Generate widget
            QWidget* widgetItem = generate_widget_item_from(items, nameItem, quantityItem);

            // Add widget to scrollAreaWidgetShoppingListLayout
            scrollAreaWidgetShoppingListLayout->addWidget(widgetItem);
        }
    }
}

QWidget* generate_widget_item_from(set<Item>& items,  string nameItem, int quantityItem){
    // Set new widget
    QWidget* widget = new QWidget();
    widget->setFixedSize(widthWidgetItem, heightWidgetItem);

    // Set widget's layout
    QHBoxLayout* widgetLayout = new QHBoxLayout(widget);

    // Init widget's components
    QCheckBox* widgetCheckBox = new QCheckBox(QString::fromStdString(nameItem));
    widgetCheckBox->setFixedSize(widthCheckBox, heightCheckBox);

    QPushButton* widgetDeleteButton = new QPushButton(deleteMessage);

    QSpinBox* widgetSpinBox = new QSpinBox();
    widgetSpinBox->setMinimum(0);
    widgetSpinBox->setMaximum(maxNumberOfQuantity);
    widgetSpinBox->setValue(quantityItem);

    // Check item which has been bought
    MainWindow::connect(widgetCheckBox, &QCheckBox::stateChanged, [&items, nameItem, widgetCheckBox, widgetSpinBox](){
        bool itemInShoppingListAndCheked = false;
        Item itemToRemove{}, updatedItem{};

        for (const auto &iteratorItems : items){
            // Find item in shopping list
            itemInShoppingListAndCheked = iteratorItems.getIsInShoppingList() && iteratorItems.getName()==nameItem;
            if (itemInShoppingListAndCheked){
                // Init the item to remove
                itemToRemove = iteratorItems;

                // Init the item to update items
                updatedItem.setName(iteratorItems.getName());
                updatedItem.setQuantity(0);
                updatedItem.setIsInShoppingList(true);
                updatedItem.setNumberOfUses(iteratorItems.getNumberOfUses());

                break;
            }
        }
        if (itemInShoppingListAndCheked){
            // Removal in items container
            items.erase(itemToRemove);

            // Update items container
            items.emplace(updatedItem);

            // Set widgetCheckBox's style
            QFont fontWidgetCheckBox = widgetCheckBox->font();
            fontWidgetCheckBox.setStrikeOut(true);
            widgetCheckBox->setFont(fontWidgetCheckBox);
            widgetCheckBox->setCheckState(Qt::Checked);
            widgetCheckBox->setEnabled(false);

            // Set widgetSpinBox's style
            widgetSpinBox->setValue(0);
            widgetSpinBox->setEnabled(false);
        }
    });

    // Delete item from list and reduce its quantity to 0
    MainWindow::connect(widgetDeleteButton, &QPushButton::clicked, [&items, nameItem,widgetSpinBox, widgetCheckBox, widgetDeleteButton, widgetLayout](){
        bool itemInShoppingListAndToRemove = false;
        Item itemToRemove{}, updatedItem{};

        for (const auto &iteratorItems : items){
            // Find item in shopping list
            itemInShoppingListAndToRemove = iteratorItems.getIsInShoppingList() && iteratorItems.getName()==nameItem;
            if (itemInShoppingListAndToRemove){
                // Init the item to remove
                itemToRemove = iteratorItems;

                // Init the item to update items
                updatedItem.setName(iteratorItems.getName());
                updatedItem.setQuantity(0);
                updatedItem.setIsInShoppingList(false);
                updatedItem.setNumberOfUses(iteratorItems.getNumberOfUses());

                break;
            }
        }
        if (itemInShoppingListAndToRemove){
            // Removal in items container
            items.erase(itemToRemove);

            // Update items container
            items.emplace(updatedItem);

            // Set widgetCheckBox's style
            QFont *fontWidgetCheckBox = new QFont(widgetCheckBox->font());
            fontWidgetCheckBox->setStrikeOut(true);
            widgetCheckBox->setFont(*fontWidgetCheckBox);
            widgetCheckBox->setCheckState(Qt::Checked);
            widgetCheckBox->setEnabled(false);

            // Set widgetSpinBox's style
            widgetSpinBox->setValue(0);
            widgetSpinBox->setEnabled(false);

            // Set widgetDeleteButton's style
            widgetDeleteButton->setEnabled(false);

            // Delete widgetLayout
            widgetLayout->deleteLater();
        }
    });

    // Change item's quantity
    MainWindow::connect(widgetSpinBox, &QSpinBox::valueChanged, [&items, nameItem, widgetCheckBox, widgetSpinBox](){
        bool itemInShoppingListAndUpdated = false;
        Item itemToRemove{}, updatedItem{};

        for (const auto &iteratorItems : items){
            // Find item in shopping list
            itemInShoppingListAndUpdated = iteratorItems.getIsInShoppingList() && iteratorItems.getName()==nameItem;
            if (itemInShoppingListAndUpdated){
                // Init the item to remove
                itemToRemove = iteratorItems;

                // Init the item to update items
                updatedItem.setName(iteratorItems.getName());
                updatedItem.setQuantity(widgetSpinBox->value());
                updatedItem.setIsInShoppingList(true);
                updatedItem.setNumberOfUses(iteratorItems.getNumberOfUses());

                break;
            }
        }
        if (itemInShoppingListAndUpdated){
            // Removal in items container
            items.erase(itemToRemove);

            // Update items container
            items.emplace(updatedItem);

            // Set widgetSpinBox's style
            if (widgetSpinBox->value()==0){
                QFont fontWidgetCheckBox = widgetCheckBox->font();
                fontWidgetCheckBox.setStrikeOut(true);
                widgetCheckBox->setFont(fontWidgetCheckBox);
                widgetCheckBox->setCheckState(Qt::Checked);
                widgetCheckBox->setEnabled(false);

                // Set widgetSpinBox's style
                widgetSpinBox->setEnabled(false);
            }
        }
    });

    // Set styles of widgetCheckBox and widgetSpinBox when an item has been bought
    bool itemBought = quantityItem == 0;
    if (itemBought){
        // Set checkBoxWidget's style
        QFont fontCheckBox = widgetCheckBox->font();
        fontCheckBox.setStrikeOut(true);
        widgetCheckBox->setFont(fontCheckBox);
        widgetCheckBox->setCheckState(Qt::Checked);
        widgetCheckBox->setEnabled(false);

        // Set spinBoxWidget's style
        widgetSpinBox->setEnabled(false);
    }

    // Add components to widgetLayout
    widgetLayout->addWidget(widgetCheckBox);
    widgetLayout->addWidget(widgetDeleteButton);
    widgetLayout->addWidget(widgetSpinBox);

    return widget;
}

void update_frequently_used_list(QListWidget* listWidgetItemsFrequentlyUsed, set<Item>& items){
    // Clear frequently used items list
    listWidgetItemsFrequentlyUsed->clear();

    // Set a vector containing stored items sorted by items' number of use
    vector<Item> itemsSortedByNumberOfUse(items.begin(), items.end());
    sort(itemsSortedByNumberOfUse.begin(), itemsSortedByNumberOfUse.end(), [](const Item& item1, const Item& item2) {
        return item1.getNumberOfUses() > item2.getNumberOfUses();
    });

    // Set a bound of items to show
    int bound = 5;
    bool numberOfItemsExceedingFive = itemsSortedByNumberOfUse.size() > 5;
    if (!numberOfItemsExceedingFive)
        bound = itemsSortedByNumberOfUse.size();

    // Show most frequently used items
    for (int i=0; i < bound; i++)
        listWidgetItemsFrequentlyUsed->addItem(new QListWidgetItem(QString::fromStdString(itemsSortedByNumberOfUse[i].getName())));
}
