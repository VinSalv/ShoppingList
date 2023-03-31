/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file storeditemswindow.cpp
 *
 *  @brief Implementation of the class StoredItemsWindow.
 *
 *  This file contains the implementation of the StoredItemsWindow class.
 */

#include "storeditemswindow.h"
#include "ui_storeditemswindow.h"


StoredItemsWindow::StoredItemsWindow(QWidget* parent) : QDialog(parent), ui(new Ui::StoredItemsWindow){
    // Init StoredItemsWindow
    ui->setupUi(this);
    setWindowTitle(titleAddstorewindow);

    // Update items container
    read_file_from(fileItems, items);

    // Init StoredItemsWindow's components
    scrollAreaWidgetStoredItems = ui->scrollAreaWidgetStoredItems;

    buttonAddNewItem = ui->buttonAddNewItem;
    buttonAddNewItem->setEnabled(false);

    textEditNewItem = ui->textEditNewItem;

    //Update graphics stored items list
    update_stored_list(scrollAreaWidgetStoredItems, items);
}

StoredItemsWindow::~StoredItemsWindow(){
    // Delete StoredItemsWindow
    delete ui;
}

void StoredItemsWindow::on_buttonNewItem_clicked(){
    string nameItem = get_formatted_text_from(textEditNewItem->toPlainText().toStdString());

    // If the item already exist, show warning popup
    if (item_exist_in(items, nameItem)){
        // Init warningPopupWindow
        QWidget* warningPopupWindow = new QWidget;
        warningPopupWindow->setWindowTitle(titleWarningPopup);
        warningPopupWindow->setWindowModality(Qt::ApplicationModal);

        // Set warningPopupWindow's layout
        QVBoxLayout* waringPopupLayout = new QVBoxLayout(warningPopupWindow);

        // Init the warningPopupWindow's components
        QLabel* waringPopupLabel = new QLabel(QString::fromStdString(textWarningPopup));
        waringPopupLabel->setFixedSize(widthComponentsPopup, heightComponentsPopup);

        QPushButton* waringPopupCloseButton = new QPushButton(okMessage);
        waringPopupCloseButton->setFixedSize(widthComponentsPopup, heightComponentsPopup);
        // Click to close warningPopupWindow
        QObject::connect(waringPopupCloseButton, &QPushButton::clicked, warningPopupWindow, [warningPopupWindow](){
            warningPopupWindow->close();
        });

        // Add components to warningPopupWindow
        waringPopupLayout->addWidget(waringPopupLabel);
        waringPopupLayout->addWidget(waringPopupCloseButton);

        // Show warningPopupWindow
        warningPopupWindow->show();
    // Otherwise, update items container and graphics stored items list
    }else{
        textEditNewItem->clear();

        // Init the item to update items container
        Item item{nameItem};

        // Update items container
        items.emplace(item);

        // Update graphics stored items list
        update_stored_list(scrollAreaWidgetStoredItems, items);
    }
}

void StoredItemsWindow::on_textEditNewItem_textChanged(){
    // If textEditNewItem is empty, lock buttonNewItem
    if (text_is_in(textEditNewItem))
        buttonAddNewItem->setEnabled(false);
    // Otherwise, unlock buttonNewItem
    else
        buttonAddNewItem->setEnabled(true);
}

void StoredItemsWindow::on_buttonBox_accepted(){
    // Update file
    write_file_from(fileItems, items);

    // Close StoredItemsWindow
    close();
}

void StoredItemsWindow::on_buttonBox_rejected(){
    // Close StoredItemsWindow
    close();
}

void update_stored_list(QWidget* scrollAreaWidgetStoredItems, set<Item>& items){
    // Fatch scrollAreaWidgetStoredItems's layout
    QVBoxLayout* scrollAreaWidgetStoredItemsLayout = qobject_cast<QVBoxLayout*> (scrollAreaWidgetStoredItems->layout());
    bool scrollAreaWidgetStoredItemsLayoutExist = scrollAreaWidgetStoredItemsLayout != nullptr;

    // If scrollAreaWidgetStoredItemsLayout exists, remove its components
    if (scrollAreaWidgetStoredItemsLayoutExist)
        // Delete scrollAreaWidgetStoredItemsLayout's components
        while (scrollAreaWidgetStoredItemsLayout->count() > 0) {
            // Fetch first widget from scrollAreaWidgetStoredItemsLayout
            QWidget* widget = scrollAreaWidgetStoredItemsLayout->itemAt(0)->widget();
            bool widgetExist = widget != nullptr;

            // If scrollAreaWidgetStoredItemsLayout's widget exists, remove it
            if (widgetExist) {
                scrollAreaWidgetStoredItemsLayout->removeWidget(widget);
                delete widget;
            // Otherwisse delete widget's layout
            }else{
                QLayoutItem* item = scrollAreaWidgetStoredItemsLayout->takeAt(0);
                delete item->layout();
                delete item;
            }
        }
    // Otherwise, generate a new QVBoxLayout for scrollAreaWidgetStoredItems
    else scrollAreaWidgetStoredItemsLayout = new QVBoxLayout(scrollAreaWidgetStoredItems);

    // Update scrollAreaWidgetStoredItemsLayout
    set<Item> copyItems{};
    copy(items.begin(), items.end(), inserter(copyItems, copyItems.begin()));
    for (const auto &iteratorItems : copyItems){
        // Fetch item's information
        string nameItem = iteratorItems.getName();

        // Generate widget
        QWidget* widgetItem = generate_widget_item_from(items, nameItem);

        // Add widget to scrollAreaWidgetStoredItemsLayout
        scrollAreaWidgetStoredItemsLayout->addWidget(widgetItem);
    }
}

QWidget* generate_widget_item_from(set<Item>& items, string nameItem){
    // Set new widget
    QWidget* widget = new QWidget();
    widget->setFixedSize(widthWidgetItem, heightWidgetItem);

    // Set widget's layout
    QHBoxLayout* widgetLayout = new QHBoxLayout(widget);

    // Init widget's components
    QLabel* widgetLabel = new QLabel(QString::fromStdString(nameItem));
    widgetLabel->setFixedSize(250,20);

    QPushButton* widgetDeleteButton = new QPushButton(deleteMessage);

    QPushButton* widgetEditButton = new QPushButton(editMessage);

    // Delete item from items container
    StoredItemsWindow::connect(widgetDeleteButton, &QPushButton::clicked,[&items, nameItem, widgetDeleteButton, widgetEditButton, widgetLabel, widgetLayout]() {
        // Find element in items to be deleted
        auto iteratorItems = find_if(items.begin(), items.end(), [=](const Item& item) {
            return item.getName() == nameItem;
        });
        bool itemFoundInItems = iteratorItems != items.end();
        if (itemFoundInItems)
            items.erase(iteratorItems);

        // Set widgetLabel's style
        QFont fontWidgetLabel = widgetLabel->font();
        fontWidgetLabel.setStrikeOut(true);
        widgetLabel->setFont(fontWidgetLabel);

        // Set widgetDeleteButton's style
        widgetDeleteButton->setEnabled(false);

        // Set widgetEditButton's style
        widgetEditButton->setEnabled(false);

        // Delete widgetLayout
        widgetLayout->deleteLater();
    });

    // Edit item from items container
    StoredItemsWindow::connect(widgetEditButton, &QPushButton::clicked,[&items, nameItem, widgetLabel]() {
        // Init editPopupWindow
        QWidget* editPopupWindow = new QWidget;
        editPopupWindow->setWindowTitle(titleEditPopup);
        editPopupWindow->setWindowModality(Qt::ApplicationModal);

        // Set editPopupWindow's layout
        QVBoxLayout* editPopupLayout = new QVBoxLayout(editPopupWindow);

        // Init the popupWindow's components
        QLabel* editPopupLabel = new QLabel(QString::fromStdString(textEditPopup));
        editPopupLabel->setFixedSize(widthComponentsPopup, heightComponentsPopup);

        QTextEdit* editPopupTextEdit = new QTextEdit();
        editPopupTextEdit->setFixedSize(widthComponentsPopup, heightComponentsPopup);

        QPushButton* editPopupOkButton = new QPushButton(okMessage);
        editPopupOkButton->setFixedSize(widthComponentsPopup, heightComponentsPopup);
        // Add new item if it is possible
        QObject::connect(editPopupOkButton, &QPushButton::clicked, editPopupWindow, [editPopupWindow, &items, nameItem, editPopupTextEdit, widgetLabel]() {
            string old_name = nameItem;
            string new_name = get_formatted_text_from(editPopupTextEdit->toPlainText().toStdString());

            // Init warningPopupWindow
            QWidget *warningPopupWindow = new QWidget;
            warningPopupWindow->setWindowTitle(titleWarningPopup);
            warningPopupWindow->setWindowModality(Qt::ApplicationModal);

            // Set warningPopupWindow's layout
            QVBoxLayout* waringPopupLayout = new QVBoxLayout(warningPopupWindow);

            // Init the warningPopupWindow's components
            QLabel* waringPopupLabel = new QLabel(QString::fromStdString(textWarningPopup));
            waringPopupLabel->setFixedSize(widthComponentsPopup, heightComponentsPopup);

            QPushButton* waringPopupCloseButton = new QPushButton(okMessage);
            waringPopupCloseButton->setFixedSize(widthComponentsPopup, heightComponentsPopup);
            // Close warningPopupWindow
            QObject::connect(waringPopupCloseButton, &QPushButton::clicked, warningPopupWindow, [warningPopupWindow](){
                warningPopupWindow->close();
            });

            // Add components to warningPopupWindow
            waringPopupLayout->addWidget(waringPopupLabel);
            waringPopupLayout->addWidget(waringPopupCloseButton);

            // If the item already exist show warning popup
            if (item_exist_in(items, new_name)){
                // Show warningPopupWindow
                warningPopupWindow->show();
            // Otherwise, update items and graphics stored item list
            }else{
                // Find element in items to be modified
                auto iteratorItems = find_if(items.begin(), items.end(), [=](const Item& item) {
                    return item.getName() == old_name;
                });
                bool itemFoundInItems = iteratorItems != items.end();
                if (itemFoundInItems){
                    // Remove the item to update items
                    items.erase(iteratorItems);

                    // Init the item to update items
                    Item item{};
                    // Set item's new name
                    item.setName(new_name);
                    item.setQuantity(iteratorItems->getQuantity());
                    item.setIsInShoppingList(iteratorItems->getIsInShoppingList());
                    item.setNumberOfUses(iteratorItems->getNumberOfUses());

                    // Update items container
                    items.emplace(item);

                    // Close editPopupWindow
                    editPopupWindow->close();

                    // Update widgetLabel's text
                    widgetLabel->setText(QString::fromStdString(new_name));
                }
            }
        });

        // Add components to editPopupLayout
        editPopupLayout->addWidget(editPopupLabel);
        editPopupLayout->addWidget(editPopupTextEdit);
        editPopupLayout->addWidget(editPopupOkButton);

        // Show editPopupWindow
        editPopupWindow->show();
    });

    // Add components to widgetLayout
    widgetLayout->addWidget(widgetLabel);
    widgetLayout->addWidget(widgetDeleteButton);
    widgetLayout->addWidget(widgetEditButton);

    return widget;
}

string get_formatted_text_from(string text){
    text = to_lowercase_from(text);
    text = remove_initial_spaces_from(text);
    text = remove_final_spaces_from(text);
    text = replace_spaces_from(text);
    return text;
}

bool text_is_in(const QTextEdit* textEdit) {
    return textEdit->toPlainText().trimmed().isEmpty();
}

bool item_exist_in(set<Item>items, string nameItem){
    auto iterator = items.find(nameItem);
    bool itemInItems = iterator != items.end();
    return itemInItems;
}


