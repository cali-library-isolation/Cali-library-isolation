#ifndef CALI_CONFIGFILE_GENERATOR_H
#define CALI_CONFIGFILE_GENERATOR_H

#include <string>
#include <vector>

std::string generateBoilerplateConfiguration(std::vector<std::string> &arguments);

void writeBoilerplateConfiguration(std::vector<std::string> &arguments, const std::string &filename);

#endif //CALI_CONFIGFILE_GENERATOR_H
