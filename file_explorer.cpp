#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;

/* DAY 1: Display Folder Contents */

void browseDirectory(const string &currentPath) {
    DIR *directory;
    struct dirent *content;

    directory = opendir(currentPath.c_str());
    if (!directory) {
        perror("Cannot open the directory");
        return;
    }

    cout << "\nDirectory: " << currentPath << endl;
    cout << "-------------------------------------------" << endl;

    while ((content = readdir(directory)) != nullptr) {
        if (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0)
            continue;

        struct stat fileInfo;
        string itemPath = currentPath + "/" + content->d_name;
        stat(itemPath.c_str(), &fileInfo);

        if (S_ISDIR(fileInfo.st_mode))
            cout << "[DIR]   ";
        else
            cout << "[FILE]  ";

        cout << content->d_name << endl;
    }

    closedir(directory);
}

/* DAY 2: Change Working Directory */

void goToDirectory(const string &path) {
    if (chdir(path.c_str()) == 0)
        cout << "Changed directory to: " << path << endl;
    else
        perror("Error changing directory");
}

/* DAY 3: File Handling (Create, Delete, Rename, Copy) */

void createNewFile(const string &filename) {
    ofstream outFile(filename);
    if (outFile) {
        cout << "File created: " << filename << endl;
        outFile.close();
    } else {
        cerr << "Error creating file." << endl;
    }
}

void deleteExistingFile(const string &filename) {
    if (remove(filename.c_str()) == 0)
        cout << "File deleted: " << filename << endl;
    else
        perror("Error deleting file");
}

void renameOrMoveFile(const string &oldFile, const string &newFile) {
    if (rename(oldFile.c_str(), newFile.c_str()) == 0)
        cout << "File renamed or moved successfully." << endl;
    else
        perror("Error renaming or moving file");
}

void copyExistingFile(const string &sourceFile, const string &targetFile) {
    ifstream source(sourceFile, ios::binary);
    ofstream target(targetFile, ios::binary);
    if (!source || !target) {
        cerr << "Error reading or writing file." << endl;
        return;
    }
    target << source.rdbuf();
    cout << "File copied successfully." << endl;
}

/* DAY 4: Recursive File Search */
void searchInFolders(const string &rootPath, const string &fileName) {
    DIR *dir = opendir(rootPath.c_str());
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        string fullPath = rootPath + "/" + entry->d_name;
        struct stat info;
        if (stat(fullPath.c_str(), &info) == -1)
            continue;

        if (S_ISDIR(info.st_mode)) {
            searchInFolders(fullPath, fileName);
        } else if (fileName == entry->d_name) {
            cout << "Found: " << fullPath << endl;
        }
    }
    closedir(dir);
}

/* DAY 5: View and Change File Permission */

void modifyFileAccess(const string &filePath) {
    struct stat details;
    if (stat(filePath.c_str(), &details) != 0) {
        perror("Error retrieving file info");
        return;
    }

    cout << "\nCurrent Permissions for " << filePath << ": ";
    cout << ((details.st_mode & S_IRUSR) ? "r" : "-")
         << ((details.st_mode & S_IWUSR) ? "w" : "-")
         << ((details.st_mode & S_IXUSR) ? "x" : "-")
         << ((details.st_mode & S_IRGRP) ? "r" : "-")
         << ((details.st_mode & S_IWGRP) ? "w" : "-")
         << ((details.st_mode & S_IXGRP) ? "x" : "-")
         << ((details.st_mode & S_IROTH) ? "r" : "-")
         << ((details.st_mode & S_IWOTH) ? "w" : "-")
         << ((details.st_mode & S_IXOTH) ? "x" : "-") << endl;

    char permissionChange;
    cout << "\nDo you want to modify permissions? (y/n): ";
    cin >> permissionChange;

    if (permissionChange == 'y' || permissionChange == 'Y') {
        string newPermission;
        cout << "Enter new permission in octal format (e.g., 755): ";
        cin >> newPermission;
        string command = "chmod " + newPermission + " " + filePath;
        system(command.c_str());
        cout << "Permissions updated successfully." << endl;
    }
}

/* MAIN MENU */
int main() {
    int choice;
    string fileA, fileB;

    while (true) {
        cout << "\n-------------------------------" << endl;
        cout << "          FILE EXPLORER            " << endl;
        cout << "-------------------------------" << endl;
        cout << endl;
        cout << "1. View Folder Contents" << endl;
        cout << "2. Change Directory" << endl;
        cout << "3. Create a File" << endl;
        cout << "4. Delete a File" << endl;
        cout << "5. Rename or Move a File" << endl;
        cout << "6. Copy a File" << endl;
        cout << "7. Search for a File" << endl;
        cout << "8. Edit File Permissions" << endl;
        cout << "9. Exit Program" << endl;
        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number between 1 and 9." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                browseDirectory(".");
                break;
            case 2:
                cout << "Enter directory path: ";
                cin >> fileA;
                goToDirectory(fileA);
                break;
            case 3:
                cout << "Enter file name to create: ";
                cin >> fileA;
                createNewFile(fileA);
                break;
            case 4:
                cout << "Enter file name to delete: ";
                cin >> fileA;
                deleteExistingFile(fileA);
                break;
            case 5:
                cout << "Enter current file name: ";
                cin >> fileA;
                cout << "Enter new file name or path: ";
                cin >> fileB;
                renameOrMoveFile(fileA, fileB);
                break;
            case 6:
                cout << "Enter source file: ";
                cin >> fileA;
                cout << "Enter destination file: ";
                cin >> fileB;
                copyExistingFile(fileA, fileB);
                break;
            case 7:
                cout << "Enter file name to search: ";
                cin >> fileA;
                searchInFolders(".", fileA);
                break;
            case 8:
                cout << "Enter file name: ";
                cin >> fileA;
                modifyFileAccess(fileA);
                break;
            case 9:
                cout << "Exiting File Explorer. Goodbye." << endl;
                return 0;
            default:
                cout << "Invalid option. Please select a number between 1 and 9." << endl;
        }
    }
}
