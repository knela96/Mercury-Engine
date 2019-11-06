#include "ModuleFileSystem.h"







weFile::weFile(std::experimental::filesystem::path wepath, weFolder* parentfolder, weFileType wetype){
	wePath = wepath;
	fileFolder = parentfolder;
	weType = wetype;
}

weFile::~weFile(){
}

//---

weFolder::weFolder(std::experimental::filesystem::path path) {
	weFolderPath = path;
}

weFolder::~weFolder() {

}

//---

weFileSystem::weFileSystem(Application * app, bool start_enabled):Module(app,start_enabled)
{
	LOGC("Creating file system");
	RootFolderPath = std::experimental::filesystem::current_path(); //set root path
	RootFolder = LoadCurrentFolder(RootFolderPath);//get root folder (from root path)

}

weFileSystem::~weFileSystem()
{
}
