#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cctype>

namespace fs = std::filesystem;

// Embedded fallback of the blogwriter logo from logo.txt
const std::string EMBEDDED_LOGO = R"(╒════╗  ╒═══╗ ╒══════════╗ ╒══════╗ ╒═════════╗ ╒════╗╒════╗  ╒════╗   ╒════╗  ╒════════╗   ╒════╗   ╒════╗ ╒════════╗      ╒═════════╗  ╒═════════╗ ╒════╗    ╒════╗     
└┐  ╓╜ ╒╛ ╓─╜ └┐  ╓───┐  ║ └─┐  ╓─╜ │  ╓────┐ ║ └┐  ╓╜└┐  ╓╜  │    ╚╗  └┐  ╓╜  │  ╓──┐  ║   │    ╚╗  └┐  ╓╜ │  ╓───┐ ║      └┐  ╓───┐ ╚╗ └┐  ╓───┐ ║ └┐  ╓╜    └┐  ╓╜     
 │  ║ ╒╛ ╓╜    │  ║   │  ║   │  ║   │  ║    └─╜  │  ║  │  ║   │  ╟┐ ╚╗  │  ║   │  ║  │  ║   │  ╟┐ ╚╗  │  ║  │  ║   └─╜       │  ║   │  ║  │  ║   └─╜  └┐ ╚╗    ╒╛ ╓╜      
 │  ╚═╛  ╚═╗   │  ╚═══╛  ║   │  ║   │  ╚══════╗  │  ╚══╛  ║   │  ║└┐ ╚╗ │  ║   │  ╚══╛  ║   │  ║└┐ ╚╗ │  ║  │  ║             │  ║   │  ║  │  ╚══╗      └┐ ╚╗  ╒╛ ╓╜       
 │  ╓───┐  ║   │  ╓┐  ╓──╜   │  ║   └──────┐  ║  │  ╓──┐  ║   │  ║ └┐ ╚╗│  ║   │  ╓──┐  ║   │  ║ └┐ ╚╗│  ║  │  ║ ╒════╗      │  ║   │  ║  │  ╓──╜       └┐ ╚╗╒╛ ╓╜        
 │  ║   │  ║   │  ║└┐ ╚╗     │  ║   ╒═╗    │  ║  │  ║  │  ║   │  ║  └┐ ╚╡  ║   │  ║  │  ║   │  ║  └┐ ╚╡  ║  │  ║ └┐  ╓╜      │  ║   │  ║  │  ║   ╒═╗     └┐ ╚╛ ╓╜         
╒╛  ╚╗ ╒╛  ╚╗ ╒╛  ╚╗└┐ ╚═╗ ╒═╛  ╚═╗ │ ╚════╛  ║ ╒╛  ╚╗╒╛  ╚╗ ╒╛  ╚╗  └┐    ║  ╒╛  ╚╗╒╛  ╚╗ ╒╛  ╚╗  └┐    ║  │  ╚══╛  ║  ╒═╗ ╒╛  ╚═══╛ ╓╜ ╒╛  ╚═══╛ ║      └┐  ╓╜          
└────╜ └────╜ └────╜ └───╜ └──────╜ └─────────╜ └────╜└────╜ └────╜   └────╜  └────╜└────╜ └────╜   └────╜  └────────╜  └─╜ └─────────╜  └─────────╜       └──╜           
)";

void displayLogo(const std::string& exePath) {
    std::ifstream file("logo.txt");
    if (!file.is_open()) {
        try {
            fs::path exeDir = fs::canonical(exePath).parent_path();
            file.open(exeDir / "logo.txt");
        } catch (...) {
            // Fall back silently
        }
    }

    if (file.is_open()) {
        std::cout << file.rdbuf();
    } else {
        std::cout << EMBEDDED_LOGO;
        if (EMBEDDED_LOGO.empty() || EMBEDDED_LOGO.back() != '\n') {
            std::cout << '\n';
        }
    }
}

void printCommands() {
    std::cout << "Available Commands:\n"
              << "  blogwriter -o\n"
              << "      open and show the text once + show commands\n\n"
              << "  blogwriter -h\n"
              << "      show only available commands\n\n"
              << "  blogwriter -make <dbs_name> / blogwriter make <dbs_name>\n"
              << "      make a new mongoDB database using mongosh\n\n"
              << "  blogwriter -in <dbs> create <file_name>\n"
              << "      create a markdown file with name <file_name>\n\n"
              << "  blogwriter -in <dbs_name> open <file_name>\n"
              << "      open the file <file_name> from dbs <dbs_name> in nano\n\n"
              << "  blogwriter -connect <dbs_name> to <repository_name>\n"
              << "      move dbs to the repository <repository_name>\n\n"
              << "  blogwriter - show <file_name> from <dbs_name>\n"
              << "      shows file <file_name> from the dbs <dbs_name>\n\n"
              << "  blogwriter - check <dbs_name> / blogwriter check <dbs_name>\n"
              << "      verifies <dbs_name> exists\n\n"
              << "  blogwriter - set admin <admin_name> pass <admin_pass> for <dbs_name>\n"
              << "      sets admin credentials for a dbs <dbs_name>\n";
}

#include "mongosh_cmd/makeDBS.h"
#include "mongosh_cmd/checkDBS.h"
#include "mongosh_cmd/setadmin.h"

namespace {
bool parseAdminArgs(int startIdx, int argc, char* argv[], std::string& adminName, std::string& adminPass, std::string& dbsName) {
    bool hasKeywords = false;
    for (int i = startIdx; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "admin" || a == "pass" || a == "password" || a == "for") {
            hasKeywords = true;
            break;
        }
    }

    if (hasKeywords) {
        for (int i = startIdx; i < argc; ++i) {
            std::string a = argv[i];
            if (a == "admin" && i + 1 < argc) {
                adminName = argv[++i];
            } else if ((a == "pass" || a == "password") && i + 1 < argc) {
                adminPass = argv[++i];
            } else if (a == "for" && i + 1 < argc) {
                dbsName = argv[++i];
            }
        }
        return (!adminName.empty() && !adminPass.empty() && !dbsName.empty());
    }

    if (argc - startIdx == 3) {
        adminName = argv[startIdx];
        adminPass = argv[startIdx + 1];
        dbsName = argv[startIdx + 2];
        return true;
    }
    return false;
}
} // anonymous namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printCommands();
        return 0;
    }

    std::string cmd = argv[1];

    if (cmd == "-o" || cmd == "--open") {
        displayLogo(argv[0]);
        std::cout << "\n";
        printCommands();
        return 0;
    }

    if (cmd == "-h" || cmd == "--help") {
        printCommands();
        return 0;
    }

    if (cmd == "-make" || cmd == "make" || cmd == "--make") {
        if (argc < 3) {
            std::cerr << "Error: Missing database name.\n";
            std::cerr << "Usage: blogwriter -make <dbs_name>  (or blogwriter make <dbs_name>)\n";
            return 1;
        }
        return makeDatabase(argv[2]);
    }

    if (cmd == "-" && argc >= 3) {
        std::string subCmd = argv[2];
        if (subCmd == "check") {
            if (argc < 4) {
                std::cerr << "Error: Missing database name.\n";
                std::cerr << "Usage: blogwriter - check <dbs_name>  (or blogwriter check <dbs_name>)\n";
                return 1;
            }
            return checkDBS(argv[3]);
        } else if (subCmd == "set") {
            std::string adminName, adminPass, dbsName;
            if (!parseAdminArgs(3, argc, argv, adminName, adminPass, dbsName)) {
                std::cerr << "Error: Incomplete arguments for set admin.\n";
                std::cerr << "Usage: blogwriter - set admin <admin_name> pass <admin_pass> for <dbs_name>\n";
                return 1;
            }
            return setAdmin(adminName, adminPass, dbsName);
        } else if (subCmd == "show") {
            std::cout << "Command 'blogwriter - show' is not yet implemented.\n";
            return 1;
        }
    }

    if (cmd == "-check" || cmd == "check" || cmd == "--check") {
        if (argc < 3) {
            std::cerr << "Error: Missing database name.\n";
            std::cerr << "Usage: blogwriter - check <dbs_name>  (or blogwriter check <dbs_name>)\n";
            return 1;
        }
        return checkDBS(argv[2]);
    }

    if (cmd == "-set" || cmd == "set" || cmd == "--set" || cmd == "-setadmin" || cmd == "setadmin") {
        std::string adminName, adminPass, dbsName;
        if (!parseAdminArgs(2, argc, argv, adminName, adminPass, dbsName)) {
            std::cerr << "Error: Incomplete arguments for set admin.\n";
            std::cerr << "Usage: blogwriter - set admin <admin_name> pass <admin_pass> for <dbs_name>\n";
            return 1;
        }
        return setAdmin(adminName, adminPass, dbsName);
    }

    if (cmd == "-in" || cmd == "in") {
        std::cout << "Command 'blogwriter -in' is not yet implemented.\n";
        return 1;
    }

    if (cmd == "-connect" || cmd == "connect") {
        std::cout << "Command 'blogwriter -connect' is not yet implemented.\n";
        return 1;
    }

    if (cmd == "-show" || cmd == "show" || cmd == "--show") {
        std::cout << "Command 'blogwriter - show' is not yet implemented.\n";
        return 1;
    }

    std::cerr << "Unknown command: " << cmd << "\n\n";
    printCommands();
    return 1;
}
