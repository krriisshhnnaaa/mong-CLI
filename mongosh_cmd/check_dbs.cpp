#include "checkDBS.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <filesystem>
#include <cstdio>
#include <string>

namespace fs = std::filesystem;

namespace {
bool isValidDbName(const std::string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}
} // anonymous namespace

bool checkDatabaseExists(const std::string& dbsName) {
    // Check directory in mongoDATABASE or mongoDATABSE
    bool dirExists = false;
    if ((fs::exists("mongoDATABASE") && fs::exists(fs::path("mongoDATABASE") / dbsName)) ||
        (fs::exists("mongoDATABSE") && fs::exists(fs::path("mongoDATABSE") / dbsName))) {
        dirExists = true;
    }

    // Check in MongoDB via mongosh
    bool mongoExists = false;
    std::string cmd = "mongosh --eval \"db.getMongo().getDBNames().includes('" + dbsName + "')\" --quiet 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        std::string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        if (result.find("true") != std::string::npos) {
            mongoExists = true;
        }
    }

    return mongoExists || dirExists;
}

int checkDBS(const std::string& dbsName) {
    if (!isValidDbName(dbsName)) {
        std::cerr << "Error: Invalid database name '" << dbsName << "'. Use alphanumeric characters, '_', or '-'.\n";
        return 1;
    }

    if (checkDatabaseExists(dbsName)) {
        std::cout << "Database '" << dbsName << "' exists.\n";
        return 0;
    } else {
        std::cout << "Database '" << dbsName << "' does not exist.\n";
        return 1;
    }
}
