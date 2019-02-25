#include "helperFunctions.h"

///Returns a string representing the base name of the input/output file for the given face index and sample index
///Does not append the extension!
std::string HelperFunctions::getSampleFileName(const unsigned int faceID, const unsigned int sampleID) {
    //See beginning of this code for the constants referenced;
    std::stringstream paddedFaceID;
    paddedFaceID << std::setfill('0') << std::setw(FACEID_LEADING_ZEROS) << faceID;
    std::stringstream paddedSampleID;
    paddedSampleID << std::setfill('0') << std::setw(SAMPLEID_LEADING_ZEROS) << sampleID;
    return FACE_PREFIX + paddedFaceID.str() + "_" + paddedSampleID.str();
}

/// Checks to see if directory exists, if it does, returns true. If it doesn't exist, it attempts to create the directory. If successful, returns true, if failed, returns false.
bool HelperFunctions::prepareDirectory(const std::string pathToDir) {
    DIR *dir;
    if ((dir = opendir (pathToDir.c_str())) != NULL) { // Directory already exists
      closedir (dir);
      return true;
    } else { // Directory does not exist already
        return mkdir(pathToDir.c_str(), 0777) == 0; // If mkdir returns 0, then it means the directory was sucessfully created.
    }
}

/// Returns the base file name of the file pointed to by the path
std::string HelperFunctions::getBaseName(const std::string path) {
    char p[50];
    strcpy(p, path.c_str());
    char * bname = basename(p);
    return std::string(bname);
}

/// List all files in a directory. Does not output . and ..
std::vector<std::string> HelperFunctions::listFiles(std::string pathToDir) {
    std::vector<std::string> ls;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (pathToDir.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        if(strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0) {
            ls.push_back(std::string(ent->d_name));
        }
      }
      closedir (dir);
    } else {
      /* could not open directory */
      std::cerr << "Failed to open directory " << pathToDir << "!" << std::endl;
      perror ("");
      return ls;
    }

    return ls;
}

/// List all files in a directory that have the given ending/file extension
std::vector<std::string> HelperFunctions::listFiles(std::string pathToDir, std::string extension) {
    std::vector<std::string> ls;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (pathToDir.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        if(HelperFunctions::stringEndsWith(ent->d_name, extension)) {
            ls.push_back(std::string(ent->d_name));
        }
      }
      closedir (dir);
    } else {
      /* could not open directory */
      std::cerr << "Failed to open directory " << pathToDir << "!" << std::endl;
      perror ("");
      return ls;
    }

    return ls;
}

/// https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool HelperFunctions::stringEndsWith(const std::string input, const std::string test) {
    if (input.length() >= test.length()) {
        return (0 == input.compare (input.length() - test.length(), test.length(), test));
    } else {
        return false;
    }
}

