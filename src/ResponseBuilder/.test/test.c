#include <dirent.h>
#include <iostream>
#include <cstring>

int main() {
    const char *directory_path = "."; // Specify the directory path (current directory)
    DIR *dir = opendir(directory_path); // Open the directory

    if (dir == nullptr) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    
    // Read entries from the directory
    while ((entry = readdir(dir)) != nullptr) {
        std::cout << "Found: " << entry->d_name << std::endl; // Print the entry name
		std::cout << "Type : " << entry->d_ino << std::endl;
		std::cout << "Type : " << entry->d_off << std::endl;
		std::cout << "Type : " << entry->d_reclen << std::endl;
		std::cout << "Type : " << entry->d_type << std::endl;
    }

    // Close the directory
    closedir(dir);

    return 0;
}
