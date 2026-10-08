#ifndef MAKEDBS_H
#define MAKEDBS_H

#include <string>

// Creates a new MongoDB database with the given name using mongosh
int makeDBS(const std::string& dbsName);
int makeDatabase(const std::string& dbsName);

#endif // MAKEDBS_H
