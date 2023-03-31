/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file utils.h
 *
 *  @brief Useful constants and functions.
 *
 *  This file contiens constants and functions useful for other classes.
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <QRegularExpression>
#include <QString>
#include <regex>
#include <set>
#include <fstream>

using namespace std;


const string fileItems = "items.txt";

const int maxNumberOfCarachter = 25;
const int maxNumberOfQuantity = 99999;
const int maxNumberOfUse = INT_MAX/2;

const QString regexInitialSpaces = "^[\\s]*";
const QString regexFinalSpaces = "\\s+$";
const string regexSpaces = "\\s+";

const int widthWidgetItem = 500;
const int heightWidgetItem = 50;

const int widthComponentsPopup = 300;
const int heightComponentsPopup = 30;

const QString titleMainwindow = "Shopping List";
const QString titleAboutPopup = "About";
const string textAboutPopup = "© 2023 Vincenzo Salvati Rights Reserved.";
const int widthCheckBox = 300;
const int heightCheckBox = 30;

const QString titleAddstorewindow = "Stored items";
const QString titleEditPopup = "Edit";
const string textEditPopup = "Digit the new name:";
const QString titleWarningPopup = "Warning!!";
const string textWarningPopup = "The item already exixst!!";

const QString titleAdditemtoshoppinglistwindow = "Select Item";

const QString okMessage = "Ok";
const QString deleteMessage = "Delete";
const QString editMessage = "Edit";

/**
 *  @brief  Check if the file exist.
 *
 *  @param  nameFile    Name of the file.
 *
 *  @return A boolean to indicate if the file exist.
 *
 *  Check the existence and/or the integrity of the file by using good() function.
 *
*/
bool exist_file(string nameFile);

/**
 *  @brief  Write or read the file.
 *
 *  @param  nameFile    Name of the file.
 *  @param  container   Set container of objects.
 *
 *  Write the container within the file in case the file does not exist,
 *  viceversa, read the file and fill a set container.
 *
*/
template <typename Type>
void setup_from(string nameFile, set<Type>& container);

template <typename Type>
void setup_from(string nameFile, set<Type>& container){
    if (exist_file(nameFile))
        read_file_from(nameFile, container);
    else
        write_file_from(nameFile, container);
}

/**
 *  @brief  Read the file.
 *
 *  @param  nameFile    Name of the file.
 *  @param  container   Set container of objects.
 *
 *  Read the file and fill a set container.
 *
*/
template <typename Type>
void read_file_from(string nameFile, set<Type>& container);

template <typename Type>
void read_file_from(string nameFile, set<Type>& container){
    container.clear();
    ifstream *inFile = new ifstream(nameFile);
    if (inFile->is_open()){
        string name;
        int quantity;
        bool isInShoppingList;
        int numberUse;
        while (*inFile >> name >> quantity >> isInShoppingList >> numberUse)
            container.emplace(name, quantity, isInShoppingList, numberUse);
        inFile->close();
    } else
        cerr << "Error opening file for reading." << endl;
}

/**
 *  @brief  Write the file.
 *
 *  @param  nameFile    Name of the file.
 *  @param  container   Set container of objects.
 *
 *  Write a set container within the file.
 *
*/
template <typename Type>
void write_file_from(string nameFile, set<Type> container);

template <typename Type>
void write_file_from(string nameFile, set<Type> container){
    ofstream *outFile = new ofstream(nameFile);
    if (outFile->is_open()){
        for (const auto& obj : container)
            *outFile << obj.getName() << " " << obj.getQuantity() << " " << obj.getIsInShoppingList() << " " << obj.getNumberOfUses() << endl;
        outFile->close();
    } else
        cerr << "Error opening file for writing." << endl;
}

/**
 *  @brief  Transform a whole string to lowercase.
 *  @param  string    String.
 *
 *  @return A lowercase string.
 *
 *  Transform a whole string to lowercase by using transform() function.
 *
*/
string to_lowercase_from(string string);

/**
 *  @brief  Remove initial spaces from a string.
 *  @param  string    String.
 *
 *  @return A string without initial spaces.
 *
 *  Remove initial spaces from a string by using regex expression.
 *
*/
string remove_initial_spaces_from(string string);

/**
 *  @brief  Remove final spaces from a string.
 *  @param  string    String.
 *
 *  @return A string without final spaces.
 *
 *  Remove final spaces from a string by using regex expression.
 *
*/
string remove_final_spaces_from(string string);

/**
 *  @brief  Replace spaces to "_" from a string.
 *  @param  string    String.
 *
 *  @return A string with replaced spaces to "_".
 *
 *  Replace spaces to "_" from a string by using regex_replace() function.
 *
*/
string replace_spaces_from(string string);

#endif // UTILS_H
