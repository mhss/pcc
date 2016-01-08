#ifndef _SUFFIXTREE_
#define _SUFFIXTREE_

#include <algorithm>
#include <bitset>
#include <cstdio>
#include <map>
#include <set>

const int MAXLEN = 60000000;

//texto completo
extern char *str;
//número de nós atual e tamanho da string
extern int node_count;
extern int s_len;


//quantidade e posições das quebras de linha
extern int lines;
extern int* line_end;
//quais linhas do texto têm ocorrências dos padrões procurados
extern std::set<int> occ;


struct TNode;
typedef TNode* Node;
struct TINode;
typedef TINode* INode;

//nós importantes
extern Node root;
extern Node empty;
//transição do empty pro root
extern INode empty_transition;

extern TNode *nodes;
//suffix link de cada nó
extern Node *suff_links;

//nó implícito na árvore
struct TINode {
	Node node;
	//intervalo do texto
	int l, r;
	
	TINode(Node n, int l, int r);
};

//nó explícito na árvore
struct TNode {
	//transições saindo deste nó
	//guardam o nó alvo e o intervalo do texto
	std::map<char, INode> *next;
	
	int id();
	TNode();
	void set_link(Node link);
	Node get_link();
	INode find(char c);
	INode find(int i);
	void remove(int i);
	void add(Node v, int l, int r);
	bool has_transitions();
};


//cria um novo nó
Node create();
//inicializa a árvore
void init(int nc);
//destrói a árvore pra liberar memória
void clear_st();


Node test_n_split(INode active, int i, bool &is_term);
void canonise(INode active);
void build_suffix_tree(int textSize);
void count_lines();


int count_distinct(Node v);
//encontra o nó mais baixo na árvore que venha depois do texto t
//ou NULL se não houver um
//e põe a altura dele em h
Node find_node(char *t, int &h);
//marca as linhas do texto que contêm ocorrências do rótulo de V
void mark_occurrences(Node v, int h);

void print_tree();

#endif
