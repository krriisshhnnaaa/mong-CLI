#include "setadmin.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace {
bool isValidIdentifier(const std::string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}

std::string escapeJs(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        if (c == '\\') escaped += "\\\\";
        else if (c == '\'') escaped += "\\'";
        else if (c == '\"') escaped += "\\\"";
        else escaped += c;
    }
    return escaped;
}
} // anonymous namespace

int setAdmin(const std::string& adminName, const std::string& adminPass, const std::string& dbsName) {
    if (!isValidIdentifier(adminName)) {
        std::cerr << "Error: Invalid admin username '" << adminName << "'. Use alphanumeric characters, '_', or '-'.\n";
        return 1;
    }

    if (adminPass.empty()) {
        std::cerr << "Error: Admin password cannot be empty.\n";
        return 1;
    }

    if (!isValidIdentifier(dbsName)) {
        std::cerr << "Error: Invalid database name '" << dbsName << "'. Use alphanumeric characters, '_', or '-'.\n";
        return 1;
    }

    // Check if mongosh is available
    if (std::system("which mongosh > /dev/null 2>&1") != 0) {
        std::cerr << "Error: mongosh is not available on this system.\n";
        return 1;
    }

    std::cout << "Setting admin credentials for database '" << dbsName << "' using mongosh...\n";

    std::string jsCmd = "try { "
                        "db.createUser({user: '" + escapeJs(adminName) + "', pwd: '" + escapeJs(adminPass) + "', roles: [{role: 'dbOwner', db: '" + escapeJs(dbsName) + "'}]}); "
                        "} catch (e) { "
                        "db.updateUser('" + escapeJs(adminName) + "', {pwd: '" + escapeJs(adminPass) + "', roles: [{role: 'dbOwner', db: '" + escapeJs(dbsName) + "'}]}); "
                        "}";

    std::string cmd = "mongosh \"" + dbsName + "\" --eval \"" + jsCmd + "\" --quiet";
    int ret = std::system(cmd.c_str());

    if (ret != 0) {
        std::cerr << "Error: Failed to set admin credentials for database '" << dbsName << "' using mongosh.\n";
        return 1;
    }

    // Ensure database directory exists inside mongoDATABASE
    if (fs::exists("mongoDATABASE")) {
        try {
            fs::create_directories(fs::path("mongoDATABASE") / dbsName);
        } catch (...) {
            // Ignore directory creation failure if mongosh succeeded
        }
    }

    std::cout << "Admin credentials for database '" << dbsName << "' successfully set.\n";
    return 0;
}
