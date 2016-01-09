#ifndef _SERIAL_
#define _SERIAL_

#include <cstdio>
#include <map>
#include <algorithm>
#include "coding_integers.h"
#include "suffix_tree.h"
#include "suffix_array_manber.h"
#include "file_manager.h"
#include "alphabet_index.h"

//--------------------serialização--------------------
void serialize_int(File &file, int v);
void serialize_inode(File &file, INode inode);
void serialize_node(File &file, Node node, Alphabet &alpha);
void serialize_st(File &file, Alphabet &alpha);
void serialize_sa(File &file, Manber *manber);

//--------------------desserialização--------------------
int deserialize_int(File &file);
INode deserialize_inode(File &file);
TNode deserialize_node(File &file, Alphabet &alpha);
void deserialize_st(File &file, Alphabet &alpha);
void deserialize_sa(File &file, Manber *manber);

#endif


