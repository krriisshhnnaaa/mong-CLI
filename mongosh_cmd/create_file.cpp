#include "create_file.h"
#include "checkDBS.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

namespace {
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

bool isSafeFileName(const std::string& name) {
    if (name.empty()) return false;
    if (name.front() == '/' || name.front() == '\\') return false;
    if (name.find("..") != std::string::npos) return false;
    return true;
}
} // anonymous namespace

int createFile(const std::string& dbsName, const std::string& fileName) {
    if (dbsName.empty()) {
        std::cerr << "Error: Database name cannot be empty.\n";
        return 1;
    }

    if (!isSafeFileName(fileName)) {
        std::cerr << "Error: Invalid or unsafe file name '" << fileName << "'.\n";
        return 1;
    }

    // Search for <dbs_name>
    if (!checkDatabaseExists(dbsName)) {
        std::cout << "database not found\n";
        return 1;
    }

    std::cout << "database found\n";

    fs::path baseDir = getMongoDatabaseDir();
    fs::path dbDir = baseDir / dbsName;

    // Ensure database directory exists
    try {
        if (!fs::exists(dbDir)) {
            fs::create_directories(dbDir);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Could not access database directory " << dbDir.string() << ": " << e.what() << "\n";
        return 1;
    }

    fs::path targetFile = dbDir / fileName;

    // In <dbs_name> : check if <file_name> exists
    if (fs::exists(targetFile)) {
        std::cout << "already exists\n";
        return 0;
    }

    // Create mongosh file <file_name>
    try {
        if (targetFile.has_parent_path()) {
            fs::create_directories(targetFile.parent_path());
        }
        std::ofstream outFile(targetFile);
        if (!outFile.is_open()) {
            std::cerr << "Error: Failed to create file " << targetFile.string() << "\n";
            return 1;
        }
        outFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to create file " << targetFile.string() << ": " << e.what() << "\n";
        return 1;
    }

    // Attempt to register file into MongoDB collection via mongosh if available
    if (std::system("which mongosh > /dev/null 2>&1") == 0) {
        std::string escapedFile = fileName;
        std::string jsCmd = "db.blogs.insertOne({ filename: '" + escapedFile + "', createdAt: new Date() })";
        std::string cmd = "mongosh \"" + dbsName + "\" --eval \"" + jsCmd + "\" --quiet > /dev/null 2>&1";
        std::system(cmd.c_str());
    }

    return 0;
}

int create_file(const std::string& dbsName, const std::string& fileName) {
    return createFile(dbsName, fileName);
}
