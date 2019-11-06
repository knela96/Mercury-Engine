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

weFolder* weFileSystem::LoadCurrentFolder(std::experimental::filesystem::path path) {
	
	static int lastDepth = 0;
	static int ID = 0;
	static int currentDepth = 0;

	path.make_preferred();// hace q las barras sean antibarras

	weFolder* solutionFolderTemp = new weFolder(path);
	solutionFolderTemp->ID = ID;

	weFoldersArray.push_back(solutionFolderTemp);

	weFolder* CurrentFolder = solutionFolderTemp;

	for (auto&p = std::experimental::filesystem::recursive_directory_iterator(solutionFolderTemp->weFolderPath); p != std::experimental::filesystem::recursive_directory_iterator(); ++p) {
		for (auto&path : p) {
			if (is_directory(path)) { //should be path.is_directory()
				ID++;
				weFolder* newPath = new weFolder(path.path());
				newPath->ID = ID;
				newPath->depthID = p.depth();
				//int debug = p.depth();
				if (p.depth <= lastDepth) {
					int nearID = 0;
					for (auto&co : weFoldersArray) {
						if (p.depth > 0 && co->depthID == p.depth() - 1) {
							if (co->ID > nearID) {
								nearID = co->ID;
								CurrentFolder = co;
							}
						}
						else if (p.depth() == 0) {
							CurrentFolder = solutionFolderTemp;
							break;
						}
					}
					CurrentFolder->childFolders.push_back(newPath);

					weFile* t = new weFile(path.path().string().c_str(), newPath, weFileType::FOLDER);
					CurrentFolder->childFiles.push_front(t);
					newPath->SetParentFolder(CurrentFolder);
					CurrentFolder = newPath;
				}
				else if (p.depth() > lastDepth) {
					CurrentFolder->childFolders.push_back(newPath);

					weFile* t = new weFile(path.path().string().c_str(), newPath, weFileType::FOLDER);

					CurrentFolder->childFiles.push_front(t);
					newPath->SetParentFolder(CurrentFolder);
					CurrentFolder = newPath;
				}

				lastDepth = p.depth();
				weFoldersArray.push_back(newPath);

			}
			else {
				weFile* t = new weFile(path.path().string().c_str(), CurrentFolder);
				CurrentFolder->childFiles.push_back(t);
				weFilesArray.push_back(t);
			}
			return solutionFolderTemp;
		}
	}
}