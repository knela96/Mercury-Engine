#include "Application.h"
#include "ModuleFileSystem.h"
#include "lib/imgui/imgui.h"


ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) :Module(app, start_enabled)
{
	LOGC("Creating file system");
	RootFolderPath = std::experimental::filesystem::current_path(); //set root path
	RootFolder = LoadCurrentFolder(RootFolderPath);//get root folder (from root path)

	if (!RootFolder->childFolders.empty()) {
		LOGC("RootFoolder loaded correctly");
		LoadFilesToProject();
	}
	else
		LOGC("RootFoolder is Empty, impossible to initiate file system. ---> LoadCurrentFolder(args) called at ModuleFileSystem constructor");
}

ModuleFileSystem::~ModuleFileSystem()
{
}

//----
weFile::weFile(std::experimental::filesystem::path path, weFolder* parentfolder, weFileType wetype) {
	wePath = path.root_path();
	fileFolder = parentfolder;
	weType = wetype;

	path.make_preferred(); //barras ahora antibarras

	wePath = path.parent_path().generic_string(); //store the path starting at the parent directory (rootfolder) as a generic string()

	weAbsolutePath = path.generic_string(); //store the absolute path inside weabsolutepath as a string
	std::string temp = path.generic_string();
	std::experimental::filesystem::path a = App->filesystem->GetRootFolderPath();
	//temp.erase(0, App->filesystem->GetRootFolderPath().size() + 1);//not geting the root folder path as a string even if i put .as_genericstring()??????????
	wePath = temp;
	//temp-=App->filesystem->GetLabelAssetRoot();

	weFullName = path.filename().string();
	sprintf_s(labelID, "%s", weFullName.c_str());
	weNameNoExtension = weName = weFullName;

	weFileRID = App->RandomNumberGenerator.GetIntRN();

	if (weFullName.length() > 10) {
		weName.erase(10);
		weName += "...";
	}
	if (path.has_extension()) {
		ExtensionString = path.extension().string();
		weNameNoExtension.erase(weNameNoExtension.find(ExtensionString));
	}
	if (ExtensionString == ".obj") {
		weType = weFileType::FILE_OBJ;
		
	}
	else if (ExtensionString == ".fbx" || ExtensionString == ".FBX") {
		weType = weFileType::FILE_FBX;
		
	}
	else if (ExtensionString == ".cpp" || ExtensionString == ".h") {
		weType = weFileType::FILE_CPP;
		
	}
	else if (ExtensionString == ".png") {
		weType = weFileType::FILE_TEXTURE_PNG;
	}

}

weFile::~weFile() {
}

//---

weFolder::weFolder(std::experimental::filesystem::path path) {
	weFolderPath = path;
}

weFolder::~weFolder() {

}

//---

update_status ModuleFileSystem::Update(float dt)
{
	



	return UPDATE_CONTINUE;
}

void ModuleFileSystem::LoadFilesToProject() {
	
	/*ImGui::Begin("Folder Assets");
	ImGui::BeginMenuBar();
	ImGui::BeginMenu("Menu");
	ImGui::EndMenuBar();
	ImGui::End();*/

}

std::experimental::filesystem::path ModuleFileSystem::GetRootFolderPath() { 
	return RootFolderPath; }

weFolder* ModuleFileSystem::LoadCurrentFolder(std::experimental::filesystem::path path) {

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
				if (p.depth() <= lastDepth) {
					int nearID = 0;
					for (auto&co : weFoldersArray) {
						if (p.depth() > 0 && co->depthID == p.depth() - 1) {
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

