/*
 PackMan - an open source virtual filesystem.
 Copyright (c) 2019 musicfarm.
*/

#include "PackMan.h"

#include <assert.h>

PackMan::PackMan()
{
    m_RootNode = new PackManNode("Root", 0, NULL);
}

PackMan::PackMan(const std::string& Path)
{
    FILE* FileHandle = fopen(Path.c_str(), "rb");
    
    ReadNode(NULL, FileHandle);
    
    fclose(FileHandle);
}

PackMan::~PackMan()
{
    delete m_RootNode;
}

PackManNode* PackMan::GetRootNode() const
{
    return m_RootNode;
}

void PackMan::Write(const std::string& Path) const
{
    FILE* FileHandle = fopen(Path.c_str(), "wb");
    
    WriteNode(m_RootNode, FileHandle);
    
    fclose(FileHandle);
}

void PackMan::ReadNode(PackManNode* Parent, FILE* FileHandle)
{
    std::string Name = "";
    
    char C;
    fread(&C, 1, 1, FileHandle);
    while (C != '\0')
    {
        Name += C;
        fread(&C, 1, 1, FileHandle);
    }
    
    size_t Size;
    fread(&Size, sizeof(size_t), 1, FileHandle);
    
    void* Data = malloc(Size);
    fread(Data, Size, 1, FileHandle);
    
    PackManNode* Node = new PackManNode(Name, Size, Data);
    
    free(Data);
    
    if (Parent)
    {
        Parent->AddChild(Node);
    }
    else
    {
        m_RootNode = Node;
    }
    
    size_t ChildCount;
    fread(&ChildCount, sizeof(size_t), 1, FileHandle);
    
    for (int i = 0; i < ChildCount; i++)
    {
        ReadNode(Node, FileHandle);
    }
}

void PackMan::WriteNode(PackManNode* Node, FILE* FileHandle) const
{
    size_t Size = Node->GetSize();
    size_t ChildCount = Node->GetChildren().size();
    
    fwrite(Node->GetName().c_str(), Node->GetName().length() + 1, 1, FileHandle);
    fwrite(&Size, sizeof(size_t), 1, FileHandle);
    fwrite(Node->GetData(), Size, 1, FileHandle);
    fwrite(&ChildCount, sizeof(size_t), 1, FileHandle);
    
    for (PackManNode* Child : Node->GetChildren())
    {
        WriteNode(Child, FileHandle);
    }
}

PackManNode::PackManNode(const std::string& Name, size_t Size, const void* Data)
{
    m_Name = Name;
    
    m_Size = Size;
    m_Data = malloc(Size);
    memcpy(m_Data, Data, Size);
}

PackManNode::~PackManNode()
{
    free(m_Data);
    
    for (PackManNode* Child : m_Children)
    {
        delete Child;
    }
}

const std::string& PackManNode::GetName() const
{
    return m_Name;
}

const size_t PackManNode::GetSize() const
{
    return m_Size;
}

const void* PackManNode::GetData() const
{
    return m_Data;
}

PackManNode* PackManNode::GetParent() const
{
    return m_Parent;
}

const std::vector<PackManNode*>& PackManNode::GetChildren() const
{
    return m_Children;
}

void PackManNode::AddChild(PackManNode* Child)
{
    assert(!Child->m_Parent);
    
    Child->m_Parent = this;
    m_Children.push_back(Child);
}

PackManNode* PackManNode::GetChild(const std::string& Path)
{
    std::string ChildName = Path.substr(0, Path.find_first_of("/"));
    
    for (PackManNode* Child : m_Children)
    {
        if (Child->m_Name == ChildName)
        {
            if (ChildName == Path)
            {
                return Child;
            }
            else
            {
                return Child->GetChild(Path.substr(Path.find_first_of("/") + 1));
            }
        }
    }
    
    return NULL;
}
