// Dai Nguyen
// FileSys.cpp
// CPSC 3500: File System
// Implements the file system commands that are available to the shell.

// BUGS: there are 2 minnor bugs:
// 1. Failed to report Directory is Full
// 2. Disk is full test failed

#include <cstring>
#include <iostream>
#include <math.h>
using namespace std;

#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"

// mounts the file system
void FileSys::mount() {
  bfs.mount();
  curr_dir = 1;
}

// unmounts the file system
void FileSys::unmount() {
  bfs.unmount();
}

// make a directory
void FileSys::mkdir(const char *name)
{
  // check name length
  if (strlen(name) <= MAX_FNAME_SIZE){
    // new directory block
    struct dirblock_t newDirBlock;
    
    // read current directory block
    struct dirblock_t dirblock;
    bfs.read_block(curr_dir, (void *) &dirblock);
    
    bool check = true;  // check directory name exist or not
    short blockNum;     // free block number
    int currEntry;      // current entry

    // check each name in the directory
    for(int i = 0; i < MAX_DIR_ENTRIES; i++){
      // check if file exists or not
      if (strcmp(name, dirblock.dir_entries[i].name) == 0){
        cout << name << " : File exist" << endl;
        check = false;
      }
    }
    
    // if file not exist,
    // check if block is full, then create directory
    if (check){
      blockNum = bfs.get_free_block();
      
      if (blockNum != 0){ // disk not full
        currEntry = dirblock.num_entries + 1;
        
        // new
        bfs.read_block(blockNum, (void *) &newDirBlock);
        newDirBlock.dir_entries[currEntry - 1].block_num = 0;
        newDirBlock.magic = DIR_MAGIC_NUM;
        newDirBlock.num_entries = 0;
        bfs.write_block(blockNum, (void *) &newDirBlock);
        
        // update current directory to include new one
        bfs.read_block(curr_dir, (void *) &dirblock);
        strcpy(dirblock.dir_entries[currEntry - 1].name, name);
        dirblock.dir_entries[currEntry - 1].block_num = blockNum;
        dirblock.magic = DIR_MAGIC_NUM;
        dirblock.num_entries = currEntry;
        bfs.write_block(curr_dir, (void *) &dirblock);
      } 
    }
    // else: embedded function prints out "File exist"

  } else {
    cout << "Filename should be less then or equal to 9 characters " << endl;
  }
}

// switch to a directory
void FileSys::cd(const char *name)
{
  short blockNum = 0; // get current block number of directory
  bool check = false; // check if name exist, and is a directory
  
  // read current directory
  struct dirblock_t dirblock;
  bfs.read_block(curr_dir, (void *) &dirblock);
  
  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not and if it is a directory
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      blockNum = dirblock.dir_entries[i].block_num;
      
      if (is_directory(blockNum)){
        check = true;
      }
    }
  }

  if (check){
    curr_dir = blockNum;
  } else {
    cout << name << " : No such file or directory." << endl;
  }
}

// switch to home directory
void FileSys::home()
{
  curr_dir = 1;
}

// remove a directory
void FileSys::rmdir(const char *name)
{
  short blockNum = 0;   // get current block number of directory
  short tempBlock = 0;  // keep previous block to shift up directory
  int currEntry = 0;    // current index of in the directory of the name
  char *tempString;     // use to swap directory name
  bool check = false;   // check if name exist or not

  struct dirblock_t dirblock;
  struct dirblock_t subdirr;

  bfs.read_block(curr_dir, (void *) &dirblock);

  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      check = true;
      blockNum = dirblock.dir_entries[i].block_num;
      currEntry = i;
    }
  }

  if (check){
    if (!is_directory(blockNum)){
      cout << name << " : Not a directory." << endl;
    } else {
      bfs.read_block(blockNum, (void *) &subdirr);

      if (subdirr.num_entries == 0){
        bfs.reclaim_block(blockNum);

        if (dirblock.num_entries == MAX_DIR_ENTRIES){
          dirblock.dir_entries[currEntry].block_num = 0;
        } else {
          // set the reclaim block in current directory to 0
          // shift up (swap)
          // also name
          dirblock.dir_entries[currEntry].block_num = 0;
          
          tempBlock = dirblock.dir_entries[dirblock.num_entries - 1].block_num;
          strcpy(tempString,
                 dirblock.dir_entries[dirblock.num_entries - 1].name);
          
          dirblock.dir_entries[currEntry].block_num = tempBlock;
          strcpy(dirblock.dir_entries[currEntry].name, tempString); 
          
        }  
        // update current directory
        dirblock.num_entries -= 1;
        bfs.write_block(curr_dir, (void *) &dirblock);
      } else {
        cout << "Directory is not empty!" << endl;
      }
    }
  } else {
    cout << name << " : No such file or directory." << endl;
  }
}

// list the contents of current directory
void FileSys::ls()
{
  // get current block number of directory
  short blockNum = 0;

  // read current directory
  struct dirblock_t dirblock;
  struct dirblock_t subdirr;
  bfs.read_block(curr_dir, (void *) &dirblock);

  // loop through current directory and list all items
  for(int i = 0; i < dirblock.num_entries; i++){
    blockNum = dirblock.dir_entries[i].block_num;
    if (is_directory(blockNum)){
      cout << dirblock.dir_entries[i].name << "/" << endl;
    } else {
      cout << dirblock.dir_entries[i].name <<  endl;
    }
  }
}

// create an empty data file
void FileSys::create(const char *name)
{
  // check name length
  if (strlen(name) <= MAX_FNAME_SIZE){
    // new directory block
    struct dirblock_t newDirBlock;
    
    // file inode
    struct inode_t fileInode;
    
    // read current directory block
    struct dirblock_t dirblock;
    bfs.read_block(curr_dir, (void *) &dirblock);

    bool check = true;  // check directory name exist or not
    short blockNum;     // free block number
    int currEntry;      // current entry

    // check each name in the directory
    for(int i = 0; i < MAX_DIR_ENTRIES; i++){
      // check if file exists or not
      if (strcmp(name, dirblock.dir_entries[i].name) == 0){
        cout << name << " : File exist" << endl;
        check = false;
      }
    }

    // if file not exist,
    // check if block is full, then create new file
    if (check){
      blockNum = bfs.get_free_block();

      if (blockNum != 0){ // disk not full
        currEntry = dirblock.num_entries + 1;

        // new file
        bfs.read_block(blockNum, (void *) &fileInode);
        fileInode.magic = INODE_MAGIC_NUM;
        fileInode.size = 0;
        bfs.write_block(blockNum, (void *) &fileInode);
        
        // update current directory to include new file
        bfs.read_block(curr_dir, (void *) &dirblock);
        strcpy(dirblock.dir_entries[currEntry - 1].name, name);
        dirblock.dir_entries[currEntry - 1].block_num = blockNum;
        dirblock.magic = DIR_MAGIC_NUM;
        dirblock.num_entries = currEntry;
        bfs.write_block(curr_dir, (void *) &dirblock);
      }
    }

  } else {
    cout << "Filename should be less than or equal to 9 characters " << endl;
  }

}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
  short blockNum = 0;     // get current block number of directory
  short newBlock = 0;     // get/hold a block if a fileblock not full
  int currBlock = 0;      // get current block of fileInode.blocks[];
  int fileLength = 0;     // get data size
  int fileInodeBlock = 0; // fileInode current block
  int fileInodeIndex = 0; // fileInode current block index
  int fileIndexFill = 0;  // use to fill up the dataBlock
  bool check = false;     // check if filename exist
  
  struct dirblock_t dirblock;
  struct inode_t fileInode;
  struct datablock_t dataBlock;
  
  // read current directory
  bfs.read_block(curr_dir, (void *) &dirblock);

  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      blockNum = dirblock.dir_entries[i].block_num;
      
      if (!is_directory(blockNum)){
        check = true;
      }
    }
  }

  if (check){
    bfs.read_block(blockNum, (void *) &fileInode);

    if ((fileInode.size / BLOCK_SIZE) == MAX_DATA_BLOCKS){ 
      cout << "No more space. File is full." << endl;
      
    } else {
      // calculate where to start
      fileLength = strlen(data);
      fileInodeBlock = ceil(float(fileInode.size) / BLOCK_SIZE);
      fileInodeIndex = fileInode.size % BLOCK_SIZE;

      if (fileInodeBlock == 0)
        fileInodeBlock++;
      
      // if file is empty, create new block
      if (fileInodeIndex == 0) {
        newBlock = bfs.get_free_block();
        if (newBlock == 0) {
          cout << "Disk Full." << endl;
          return;
        }
        // update new block created
        fileInode.blocks[fileInodeBlock - 1] = newBlock;
        
      // get the current block
      } else {
        newBlock = fileInode.blocks[fileInode.size / BLOCK_SIZE];
      }

      // read block
      // write data to datab block. fill up a block that is empty/not full
      bfs.read_block(newBlock, (void*)&dataBlock);
      for (fileIndexFill = 0;
           fileIndexFill < BLOCK_SIZE - fileInodeIndex; fileIndexFill++){
        
        if (fileIndexFill >= fileLength) {
          break;
        }
  
        // update fileInode size
        fileInode.size += 1;
        
        // write data to dataBlock
        dataBlock.data[fileInodeIndex + fileIndexFill] = data[fileIndexFill];
      }
      bfs.write_block(newBlock, (void*)&dataBlock);
            
      // fills other data blocks if data file is larger than BLOCK_SIZE
      // get current position block in fileInode.block[];
      currBlock = ceil(float(fileInode.size) / BLOCK_SIZE);
      
      // update fileInode blocks
      fileInode.blocks[currBlock - 1] = newBlock;
      
      // fill up other blocks
      while (fileIndexFill < fileLength) {
        newBlock = bfs.get_free_block();
        
        // check is disk full
        if (newBlock == 0) {
          cout << "Disk Full" << endl;
          return;
        }
        
        // check if file is full
        if ((fileInode.size / BLOCK_SIZE) == MAX_DATA_BLOCKS) {
          cout << "File is full." << endl;
          return;
        }

        // read datablock
        // write data to datablock
        bfs.read_block(newBlock, (void*) &dataBlock);
        for (int j = 0; j < BLOCK_SIZE; j++) {
          if (fileIndexFill >= fileLength) {
            break;
          }
          fileInode.size += 1;
          dataBlock.data[j] = *(data + fileIndexFill);
          fileIndexFill++;
        }
        // update fileInode.blocks[]
        fileInode.blocks[currBlock] = newBlock;
        currBlock++;
        bfs.write_block(newBlock, (void*) &dataBlock);
        
      }

      // update fileInode and dirBlock
      bfs.write_block(blockNum, (void*) &fileInode);
      bfs.write_block(curr_dir, (void*) &dirblock);
    }
  } else {
    cout << name << " : File not exist or not a file" << endl;
  }
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
  short blockNum = 0; // get current block number of directory
  int blockCount = 0; // number of blocks has been used in fileInode.blocks[]
  bool check = false; // check if filename exist

  struct dirblock_t dirblock;
  struct inode_t fileInode;
  struct datablock_t dataBlock;

  // read current directory
  bfs.read_block(curr_dir, (void *) &dirblock);

  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      check = true;
      blockNum = dirblock.dir_entries[i].block_num;
    }
  }

  if (check){
    if (is_directory(blockNum)){
      cout << name << " : Not a file" << endl;
    } else {
      bfs.read_block(blockNum, (void *) &fileInode);
      
      // count block
      blockCount = ceil(float(fileInode.size) / BLOCK_SIZE);
      
      // display file content
      // if display all data in 1 BLOCK_SIZE, move on to another block  
      for (int i = 0; i < blockCount; i++){
        bfs.read_block(fileInode.blocks[i], (void *) &dataBlock);
   
        for (int j = 0; j < BLOCK_SIZE; j++){
          
          if (j >= strlen(dataBlock.data)){
            break;
          }
          cout << dataBlock.data[j];
        }
      }
      
      // print a line after dislay output
      cout << endl;
    }
    
  } else {
    cout << name << " : File not exist" << endl;
  }
  
}

// delete a data file
void FileSys::rm(const char *name)
{
  short blockNum = 0; // get current block number of directory
  int blockCount = 0; // number of blocks has been used in fileInode.blocks[]
  int currEntry = 0;  // get current index of file in directory
  bool check = false; // check if filename exist
  
  struct dirblock_t dirblock;
  struct inode_t fileInode;
  struct datablock_t dataBlock;
  
  bfs.read_block(curr_dir, (void *) &dirblock);

  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      check = true;
      blockNum = dirblock.dir_entries[i].block_num;
      currEntry = i;
    }
  }

  if (check){
    if (is_directory(blockNum)){
      cout << name << " : Not a file." << endl;
    } else {
      bfs.read_block(blockNum, (void *) &fileInode);
      
      // count block
      blockCount = ceil(float(fileInode.size) / BLOCK_SIZE);
      
      // reclaim all iNode blocks inside iNode struct
      for (int i = 0; i < blockCount; i++){
        bfs.reclaim_block(fileInode.blocks[i]);
      }
      
      // reclaim iNode struct block
      bfs.reclaim_block(dirblock.dir_entries[currEntry].block_num);
      
      // check if deleting last entry, otherwise move all data up
      if (currEntry != dirblock.num_entries - 1){
        for (int i = currEntry; i < dirblock.num_entries; i++){
          strcpy(dirblock.dir_entries[i].name, dirblock.dir_entries[i+1].name);
          dirblock.dir_entries[i].block_num =
                                  dirblock.dir_entries[i+1].block_num;
        }
      }
      
      // update current directory block
      dirblock.dir_entries[dirblock.num_entries - 1].block_num = 0;
      dirblock.num_entries -= 1;
      bfs.write_block(curr_dir, (void *) &dirblock);
    }
  } else {
    cout << name << " : No such file or directory." << endl;
  }
}

// display stats about file or directory
void FileSys::stat(const char *name)
{
  short blockNum = 0; // get current block number of directory
  int blockCount = 0; // number of blocks has been used in fileInode.blocks[]
  int currEntry = 0;  // get current index of file in directory
  bool check = false; // check if filename exist

  struct dirblock_t dirblock;
  struct inode_t fileInode;
  
  // read curret directory
  bfs.read_block(curr_dir, (void *) &dirblock);

  // check each name in the current directory
  for(int i = 0; i < MAX_DIR_ENTRIES; i++){
    // check if file exists or not
    if (strcmp(name, dirblock.dir_entries[i].name) == 0){
      check = true;
      currEntry = i;
      blockNum = dirblock.dir_entries[i].block_num;
    }
  }

  if (check){
    if (is_directory(blockNum)){
      cout << "Directory block: "
           << dirblock.dir_entries[currEntry].block_num << endl;
      
    } else {
      // read curret directory
      bfs.read_block(blockNum, (void *) &fileInode);
      
      cout << "Inode block: "
           << dirblock.dir_entries[currEntry].block_num << endl;
      cout << "Bytes in file: " << fileInode.size << endl;

      // count block
      blockCount = ceil(float(fileInode.size) / BLOCK_SIZE) + 1;
      
      cout << "Number of blocks: " << blockCount << endl;
    }
  } else {
    cout << name << " : No such file or directory." << endl;
  }
  
}

// HELPER FUNCTIONS (optional)
bool FileSys::is_directory(short blockNum)
{
  struct dirblock_t dirblock;
  bfs.read_block(blockNum, (void *) &dirblock);

  if (dirblock.magic == DIR_MAGIC_NUM){
    return true;
  }

  return false;
}
