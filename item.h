/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file item.h
 *
 *  @brief Declaration of the class Item.
 *
 *  This file contains the declaration of the Item class.
 */

#ifndef ITEM_H
#define ITEM_H

#include <utils.h>


class Item{

public:
    Item();
    Item(string);
    Item(string, int, bool, int);

    string getName() const;
    void setName(string);

    int getQuantity() const;
    void setQuantity(int);

    bool getIsInShoppingList() const;
    void setIsInShoppingList(bool);

    int getNumberUse() const;
    void setNumberUse(int);

    void reduceNumberUse();

private:
    string name = "";
    int quantity = 0;
    bool isInShoppingList = false;
    int numberUse = 0;
};

bool operator<(const Item&, const Item&);

#endif // ITEM_H
