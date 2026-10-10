#ifndef CREATE_FILE_H
#define CREATE_FILE_H

#include <string>

// Creates a file in the specified MongoDB database directory
// Checks if the database exists; if so, checks if the file exists.
// If the file exists, reports that it already exists; otherwise creates it.
int createFile(const std::string& dbsName, const std::string& fileName);
int create_file(const std::string& dbsName, const std::string& fileName);

#endif // CREATE_FILE_H
