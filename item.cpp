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

Item::Item(string name, int quantity, bool isInShoppingList, int numberOfUses) : quantity(quantity), isInShoppingList(isInShoppingList), numberOfUses(numberOfUses){
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

int Item::getNumberOfUses() const{
    return numberOfUses;
}

void Item::setNumberOfUses(int numberOfUses){
    this->numberOfUses = numberOfUses;
}

void Item::reduceNumberOfUses(){
    try {
        numberOfUses = numberOfUses/2;
    } catch (...) {
        numberOfUses = 0;
    }
}

bool operator<(const Item& item1, const Item& item2){
    return item1.getName() < item2.getName();
}
