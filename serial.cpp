#include "serial.h"

//--------------------serialização--------------------

void serialize_int(File &file, int v) {
	long long x = fibonacci_encoding(v);
//	printf("     serialize_int %3d ~> %lld\n", v, x);
	while(x) {
//		printf("          %10lld %d\n", x, (int) (x & 1));
		file.write_bit(x & 1);
		x >>= 1;
	}
}

void serialize_inode(File &file, INode inode) {
	//escreve a posição do nó explícito no array
	serialize_int(file, inode->node - nodes);
	serialize_int(file, inode->l);
	serialize_int(file, inode->r);
}

void serialize_node(File &file, Node node, Alphabet &alpha) {
	if (!node->has_transitions()) {
		serialize_int(file, 0);
		return;
	}
	
	serialize_int(file, node->next->size());
	
	for(std::map<char, INode>::iterator it = node->next->begin(); it != node->next->end(); it++) {
		serialize_int(file, alpha.getIndex(it->first));
		serialize_inode(file, it->second);
	}
}

void serialize_st(File &file, Alphabet &alpha) {
	serialize_int(file, node_count);
	for(int i = 0; i < node_count; i++) 
		serialize_node(file, nodes+i, alpha);
}


//--------------------desserialização--------------------

int deserialize_int(File &file) {
	bool one = false;
	int bit = 0;
	long long v = 1;
	long long r = 0;
	while(! one | ! (bit = file.read_bit())) {
//		printf(" bit %d, r %5lld, v %5lld\n", bit, r, v);
		if(bit) r |= v;
		v <<= 1;
		one = bit;
	}
	
//	printf("     deserialize.int::read %3lld ~> %d\n", r, fibonacci_decoding(r));
	return fibonacci_decoding(r);
}

INode deserialize_inode(File &file) {
	Node node = nodes + deserialize_int(file);
	int l = deserialize_int(file);
	int r = deserialize_int(file);
	
	return new TINode(node, l, r);
}

TNode deserialize_node(File &file, Alphabet &alpha) {
	TNode node;
	
	int next_count = deserialize_int(file);
	if (next_count > 0) {
		node.next = new std::map<char, INode>();
	
		for(int i = 0; i < next_count; i++) {
			char key = alpha.getChar(deserialize_int(file));
			INode value = deserialize_inode(file);
			(*(node.next))[key] = value;
		}
	}
	
	return node;
}

void deserialize_st(File &file, Alphabet &alpha) {
	int nc = deserialize_int(file);
	init(nc);
	empty = nodes+0;
	root = nodes+1;
	node_count = nc;
	
	for(int i = 0; i < node_count; i++) 
		nodes[i] = deserialize_node(file, alpha);
}

