#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "dirent.h"
#include <iomanip>
#include "string.h"
#include "stdio.h"
#include "sys/stat.h"
#include "libgen.h"

#define FACEID_LEADING_ZEROS 4
#define SAMPLEID_LEADING_ZEROS 4
#define FACE_PREFIX "face"

namespace HelperFunctions {
    std::string getSampleFileName(const unsigned int faceID, const unsigned int sampleID);
    std::vector<std::string> listFiles(std::string pathToDir);
    std::vector<std::string> listFiles(std::string pathToDir, std::string extension);
    bool prepareDirectory(std::string dirName);
    bool stringEndsWith(std::string input, std::string test);
    std::string getBaseName(std::string path);
}

#endif // HELPERFUNCTIONS_H
