/**
 *  @author Vincenzo Salvati - vincenzosalvati@hotmail.it
 *
 *  @file utils.cpp
 *
 *  @brief Useful constants and functions.
 *
 *  This file contiens constants and functions useful for other classes.
 */

#include"utils.h"


bool exist_file(string nameFile){
    ifstream* file = new ifstream(nameFile);
    if (file->good())
        return true;
    else
        return false;
}

template <typename Type>
void setup_from_from(string nameFile, set<Type>& container);

template <typename Type>
void read_file_from(string nameFile, set<Type>& container);

template <typename Type>
void write_file_from(string nameFile, set<Type> container);

string to_lowercase_from(string string){
    transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}

string remove_initial_spaces_from(string string) {
    QString qString = QString::fromStdString(string);
    qString.remove(QRegularExpression(regexInitialSpaces));
    return qString.toStdString();
}

string remove_final_spaces_from(string string) {
    QString qString = QString::fromStdString(string);
    qString.remove(QRegularExpression(regexFinalSpaces));
    return qString.toStdString();
}

string replace_spaces_from(string string) {
    regex pattern(regexSpaces);
    return regex_replace(string, pattern, "_");
}


