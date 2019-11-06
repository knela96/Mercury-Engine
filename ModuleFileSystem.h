#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

//weFile, a personal file system for this 3d engine
//#include <filesystem>
#include "Module.h"
#include "Application.h"
#include "Globals.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>
#include <iomanip>

enum class weFileType {
	FILE_NONE = 0,
	FILE_FBX,
	FILE_TEXTURE,
	FILE_MAX
};


class weFolder;

//------------------------------------------------------------------------------

class weFile
{
public:
	weFile(std::experimental::filesystem::path wepath, weFolder* parentfolder, weFileType wetype = weFileType::FILE_NONE);
	~weFile();

	weFileType weType = weFileType::FILE_NONE;
	std::string weName;
	std::experimental::filesystem::path wePath;
	uint64 weRID;
	
public:

	weFileType GetType() { return weType; }

	uint64 GetRID() { return weRID; }

	std::string GetName() { return weName; }
	void SetName(std::string name) { weName = name; }	

	std::experimental::filesystem::path GetPath() { return wePath; }
	void SetPath(std::string path) { wePath = path; }

	weFolder* GetFolder() { return fileFolder; }
	void SetFolder(weFolder* folder) { fileFolder = folder; }

	weFolder* fileFolder = nullptr;


};

//------------------------------------------------------------------------------

class weFolder
{
public:
	weFolder(std::experimental::filesystem::path path);
	~weFolder();

public:

	weFolder* GetParentFolder() { return weParentFolder; }
	void SetParentFolder(weFolder* folder) {weParentFolder = folder; }

	std::experimental::filesystem::path GetFolderPath() { return weFolderPath; }
	void SetFolderPath(std::experimental::filesystem::path path) { weFolderPath = path; }

	std::experimental::filesystem::path weFolderPath;
	
	int depthID;
	int ID;
	weFolder* weParentFolder = nullptr;

public:

	std::list<weFolder> childFolders;
	std::list<weFile> childFiles;
};

//------------------------------------------------------------------------------

class weFileSystem : public Module
{
public:
	weFileSystem(Application* app, bool start_enabled = true);
	~weFileSystem();

	weFolder* GetRootFolder() { return RootFolder; }
	std::experimental::filesystem::path GetRootFolderPath() { return RootFolderPath; }

	weFolder *LoadCurrentFolder(std::experimental::filesystem::path path);

public:

	std::vector<weFile*> weFilesArray;
	std::vector<weFolder*> weFoldersArray;

	weFolder* RootFolder;
	std::experimental::filesystem::path RootFolderPath;
	
};

#endif