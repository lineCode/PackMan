/*
 PackMan - an open source virtual filesystem.
 Copyright (c) 2019 musicfarm.
*/

#include <string>
#include <vector>

class PackManNode;

/**
 @brief The main filesystem class.
 
 This class contains functions for reading and writing PackMan files.
 */
class PackMan
{
public:
    /**
     Constructor that initializes an empty filesystem.
     */
    PackMan();
    
    /**
     Constructor that loads a filesystem from a path.
     
     @param Path The filepath.
     */
    PackMan(const std::string& Path);
    
    /**
     Destructor.
     */
    ~PackMan();
public:
    /**
     @return The root node of the filesystem.
    */
    PackManNode* GetRootNode() const;
    
    /**
     Writes the contents of the filesystem to the specified path.
     
     @param Path The filepath.
     */
    void Write(const std::string& Path) const;
private:
    void ReadNode(PackManNode* Parent, FILE* FileHandle);
    void WriteNode(PackManNode* Node, FILE* FileHandle) const;
private:
    PackManNode* m_RootNode;
};

/**
@brief The main node class.

This class contains the contents of specified nodes and other information about them.
*/
class PackManNode
{
public:
    /**
     Constructor that creates a node with the specified parameters.
     
     @param Name The name of the node.
     @param Size The size of the data to be stored.
     @param Data The data to be stored.
    */
    PackManNode(const std::string& Name, size_t Size, const void* Data);
    
    /**
     Destructor.
    */
    ~PackManNode();
public:
    /**
     @return The name of the node.
    */
    const std::string& GetName() const;
    
    /**
     @return The size of the node's data.
    */
    const size_t GetSize() const;
    
    /**
     @return The data of the node.
    */
    const void* GetData() const;
    
    /**
     @return The parent of the node.
    */
    PackManNode* GetParent() const;
    
    /**
     @return The children of the node.
    */
    const std::vector<PackManNode*>& GetChildren() const;
    
    /**
     Appends a child node to the node.
     
     @param Child The child node.
     */
    void AddChild(PackManNode* Child);
    
    /**
     Finds the child node with the specified path.
     
     @return The child node specified by the path.
     @param Path The path to search for. For example, "Node/ChildNode".
     */
    PackManNode* GetChild(const std::string& Path);
private:
    std::string m_Name;
    
    size_t m_Size;
    void* m_Data;
    
    PackManNode* m_Parent = NULL;
    
    std::vector<PackManNode*> m_Children;
};
