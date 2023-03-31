/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file item.cpp
 *
 *  @brief Implementation of the class Item.
 *
 *  This file contains the implementation of the Item class.
 */

#include "item.h"


Item::Item(){}

Item::Item(string name){
    name = to_lowercase_from(name);
    name = remove_initial_spaces_from(name);
    name = remove_final_spaces_from(name);
    name = replace_spaces_from(name);
    this->name = name;
}

Item::Item(string name, int quantity, bool isInShoppingList, int numberUse) : quantity(quantity), isInShoppingList(isInShoppingList), numberUse(numberUse){
    name = to_lowercase_from(name);
    name = remove_initial_spaces_from(name);
    name = remove_final_spaces_from(name);
    name = replace_spaces_from(name);
    this->name = name;
}

string Item::getName() const{
    return name;
}

void Item::setName(string name){
    name = to_lowercase_from(name);
    name = remove_initial_spaces_from(name);
    name = remove_final_spaces_from(name);
    name = replace_spaces_from(name);
    this->name = name;
}

int Item::getQuantity() const{
    return quantity;
}

void Item::setQuantity(int quantity){
    this->quantity = quantity;
}

bool Item::getIsInShoppingList() const{
    return isInShoppingList;
}

void Item::setIsInShoppingList(bool isInShoppingList){
    this->isInShoppingList = isInShoppingList;
}

int Item::getNumberUse() const{
    return numberUse;
}

void Item::setNumberUse(int numberUse){
    this->numberUse = numberUse;
}

void Item::reduceNumberUse(){
    try {
        numberUse = numberUse/2;
    } catch (...) {
        numberUse = 0;
    }
}

bool operator<(const Item& item1, const Item& item2){
    return item1.getName() < item2.getName();
}
