#ifndef __FOLDERCONTAINER_H__
#define __FOLDERCONTAINER_H__
#include "Globals.h"
#include <string>
#include <vector>

struct FolderContainer
{
	FolderContainer() {};
	FolderContainer(std::string path) : path(path) {
		static uint cID = 0;
		ID = cID++;
	};
	std::string path;
	std::string localPath;

	std::vector<FolderContainer> folderchilds;
	std::vector<FolderContainer> filechilds;

	bool isFile = false;
	bool hasTexture = false;

	uint ID = 0;
	uint texID = -1;

};

#endif
