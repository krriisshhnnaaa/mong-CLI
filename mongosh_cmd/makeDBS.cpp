#include "makeDBS.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <filesystem>

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

fs::path getMongoDatabaseDir() {
    if (fs::exists("mongoDATABASE") && fs::is_directory("mongoDATABASE")) {
        return "mongoDATABASE";
    }
    if (fs::exists("mongoDATABSE") && fs::is_directory("mongoDATABSE")) {
        return "mongoDATABSE";
    }
    fs::create_directories("mongoDATABASE");
    return "mongoDATABASE";
}
} // anonymous namespace

int makeDBS(const std::string& dbsName) {
    if (!isValidDbName(dbsName)) {
        std::cerr << "Error: Invalid database name '" << dbsName << "'. Use alphanumeric characters, '_', or '-'.\n";
        return 1;
    }

    // Check if mongosh is available
    if (std::system("which mongosh > /dev/null 2>&1") != 0) {
        std::cerr << "Error: mongosh is not available on this system.\n";
        return 1;
    }

    std::cout << "Creating MongoDB database '" << dbsName << "' using mongosh...\n";
    std::string cmd = "mongosh \"" + dbsName + "\" --eval \"db.createCollection('blogs')\" --quiet";
    int ret = std::system(cmd.c_str());

    if (ret != 0) {
        std::cerr << "Error: Failed to create database '" << dbsName << "' using mongosh.\n";
        return 1;
    }

    // Ensure database directory is created inside mongoDATABASE
    fs::path baseDir = getMongoDatabaseDir();
    fs::path targetDir = baseDir / dbsName;
    try {
        fs::create_directories(targetDir);
        std::cout << "Database directory created at " << targetDir.string() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not create database directory " << targetDir.string() << ": " << e.what() << "\n";
    }

    std::cout << "Database '" << dbsName << "' successfully created.\n";
    return 0;
}

int makeDatabase(const std::string& dbsName) {
    return makeDBS(dbsName);
}
