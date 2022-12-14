//
// Created by bkisly on 14.12.22.
//

#include <fstream>
#include "../include/fileManager.h"

unsigned readStore() {
    std::ifstream ifs(STORE_LOCATION);
    int status;
    ifs >> status;
    ifs.close();
    return status;
}

void writeToStore(unsigned amount) {
    std::ofstream ofs(STORE_LOCATION);
    ofs << amount;
    ofs.close();
}

void appendLog(const std::string& fileName, const std::string& content) {
    std::ofstream ofs(fileName, std::ios::app);
    ofs << content << std::endl;
}