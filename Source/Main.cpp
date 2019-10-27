/*
 PackMan - an open source virtual filesystem.
 Copyright (c) 2019 musicfarm.
*/

#include "PackMan.h"

#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>

#include <string.h>

void DisplayUsage()
{
    printf("Usage: PackManCMD pack <source> <destination>\n       PackManCMD extract <source> <destination>\n");
}

void LoadFiles(const std::string& Path, PackManNode* Node)
{
    cppfs::FileHandle Dir = cppfs::fs::open(Path);
    
    if (Dir.isDirectory())
    {
        for (cppfs::FileIterator It = Dir.begin(); It != Dir.end(); ++It)
        {
            std::string Path = *It;
            cppfs::FileHandle File = cppfs::fs::open(Dir.path() + Path);
            if (File.isFile())
            {
                printf("Loading file %s...\n", Path.c_str());
                PackManNode* FileNode = new PackManNode(Path, File.size(), File.readFile().c_str());
                Node->AddChild(FileNode);
            }
            else
            {
                PackManNode* Folder = new PackManNode(Path, 0, NULL);
                Node->AddChild(Folder);
                LoadFiles(Dir.path() + Path + "/", Folder);
            }
        }
    }
    else
    {
        printf("The specified path is not a directory.\n");
    }
}

void WriteFiles(const std::string& Path, PackManNode* Node)
{
    cppfs::FileHandle Dir = cppfs::fs::open(Path);
    if (!Dir.exists())
    {
        Dir.createDirectory();
    }
    
    for (PackManNode* Child : Node->GetChildren())
    {
        if (Child->GetChildren().size() != 0)
        {
            WriteFiles(Path + "/" + Child->GetName(), Child);
        }
        else
        {
            printf("Writing file %s...\n", Child->GetName().c_str());
            cppfs::FileHandle File = cppfs::fs::open(Path + "/" + Child->GetName());
            File.writeFile(std::string((char*) Child->GetData(), Child->GetSize()));
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        DisplayUsage();
        return 0;
    }
    
    if (strcmp(argv[1], "pack") == 0)
    {
        if (argc != 4)
        {
            DisplayUsage();
            return 0;
        }
        
        PackMan* PM = new PackMan();
        
        LoadFiles(argv[2], PM->GetRootNode());
        
        printf("Writing...\n");
        PM->Write(argv[3]);
        printf("Done.\n");
        
        delete PM;
    }
    else if (strcmp(argv[1], "extract") == 0)
    {
        if (argc != 4)
        {
            DisplayUsage();
            return 0;
        }
        
        printf("Loading...\n");
        PackMan* PM = new PackMan(argv[2]);
        printf("Done.\n");
        
        WriteFiles(argv[3], PM->GetRootNode());
        
        delete PM;
    }
    else
    {
        DisplayUsage();
        return 0;
    }
}
