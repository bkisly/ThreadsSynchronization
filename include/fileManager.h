//
// Created by bkisly on 14.12.22.
//

#ifndef THREADSSYNCHRONIZATION_FILEMANAGER_H
#define THREADSSYNCHRONIZATION_FILEMANAGER_H

#endif //THREADSSYNCHRONIZATION_FILEMANAGER_H

#define STORE_LOCATION "store.txt"

#include <string>

unsigned readStore();
void writeToStore(unsigned amount);
void appendLog(const std::string &fileName, const std::string &content);