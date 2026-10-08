#ifndef SETADMIN_H
#define SETADMIN_H

#include <string>

// Sets admin credentials for a MongoDB database using mongosh
int setAdmin(const std::string& adminName, const std::string& adminPass, const std::string& dbsName);

#endif // SETADMIN_H
