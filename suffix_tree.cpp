#include "suffix_tree.h"

//texto completo
char *str;
//número de nós atual e tamanho da string
int node_count = 0;
int s_len = 0;


//quantidade e posições das quebras de linha
int lines = 0;
int* line_end;
//quais linhas do texto têm ocorrências dos padrões procurados
std::set<int> occ;


//nós importantes
Node root;
Node empty;
//transição do empty pro root
INode empty_transition;

TNode *nodes;
//suffix link de cada nó
Node *suff_links = NULL;

TINode::TINode(Node n, int ll, int rr):
	node(n), l(ll), r(rr) {}


int TNode::id() {
	return this-nodes;
}

TNode::TNode() {
	this->next = NULL;
}

void TNode::set_link(Node link) {
	suff_links[id()] = link;
}

Node TNode::get_link() {
	return suff_links[id()];
}

INode TNode::find(char c) {
	if (this == empty)
		return empty_transition;
	
	if (next == NULL || !next->count(c))
		return NULL;
	else
		return (*next)[c];
}

INode TNode::find(int i) {
	return this->find(str[i]);
}

void TNode::remove(int i) {
	char c = str[i];
	if (next != NULL && next->count(c)) {
		free((*next)[c]);
		next->erase(c);
	}
}

void TNode::add(Node v, int l, int r) {
	if (this->next == NULL)
		this->next = new std::map<char, INode>();
	
	this->remove(l);
	(*next)[str[l]] = new TINode(v, l, r);
}

bool TNode::has_transitions() {
	return (this->next != NULL) &&
			!(this->next->empty());
}


Node create() {
	Node ret = nodes + (node_count++);
	*ret = TNode();
	ret->set_link(NULL);
	return ret;
}

void init_tree(int nc) {
	nodes = (TNode*) malloc(nc*sizeof(TNode));
	node_count = 0;
}

void clear_st() {
	for(int i = 0; i < node_count; i++)
		if (nodes[i].next != NULL) {
			for(std::map<char, INode>::iterator it = nodes[i].next->begin(); it != nodes[i].next->end(); it++)
				free(it->second);
			
			nodes[i].next->clear();
			free(nodes[i].next);
		}
	
	free(nodes);
}

//--------------------construção--------------------

void canonise(INode active) {
	int l = active->l, r = active->r;
	Node u = active->node;
	
	if (r <= l) return;
	
	INode next = u->find(l);
	
	//só funcionou fazendo <=
	while ((next->r - next->l) <= (r-l)) {
		l += (next->r - next->l);
		u = next->node;
		
		if (l < r) 
			next = u->find(l);
	}
	
	active->l = l;
	active->node = u;
}

Node test_n_split(INode active, int i, bool &is_term) {
	//só funcionou fazendo isso aqui
	if (active->node == empty) {
		active->node = root;
		active->l++;
	}
	
	int l = active->l, r = active->r;
	Node u = active->node;
	
	if (l < r) {
		INode edge = u->find(l);
		Node v = edge->node;
		int _l = edge->l, _r = edge->r;
		
		if (str[_l + (r-l)] == str[i]) {
			is_term = true;
			return u;
		} else {
			is_term = false;
			
			int mid = _l + (r-l);
			Node w = create();
			w->add(v, mid, _r);
			u->remove(l);
			u->add(w, _l, mid);
			return w;
		}
	} else {
		INode edge = u->find(i);
		is_term = (edge != NULL);
		return u;
	}
}

void build_suffix_tree(int textSize) {
	int maxnodes = textSize*2 + 3;
	init_tree(maxnodes);
	suff_links = (Node*) malloc(maxnodes*sizeof(Node));
	
	empty = create();
	root = create();
	empty_transition = new TINode(root, -2, -1);
	root->set_link(empty);
	
	INode active = new TINode(root, 0, 0);
	
	s_len = textSize;
	for(int i = 0; i <= s_len; i++) {
		active->r = i;
		//só funcionou fazendo assim
		canonise(active);
		
		//update
		//no artigo original ele faz com root
		Node oldr = root;
		bool is_term;
		Node w = test_n_split(active, i, is_term);

		while (!is_term) {
			Node v = create();
			w->add(v, i, s_len+1);
	
			if (oldr != root)
				oldr->set_link(w);
			oldr = w;
	
			active->node = active->node->get_link();
			canonise(active);
			w = test_n_split(active, i, is_term);
		}

		if (oldr != root)
			oldr->set_link(active->node);
	}
	
	free(suff_links);
	free(active);
}

//--------------------utilização--------------------

Node find_node(char *t, int &h) {
	Node v = root;
	h = 0;
	
	while (*t != '\0') {
		INode next = v->find(*t);
		//não tem a aresta
		if (next == NULL) 
			return NULL;
	
		int l = next->l, r = next->r;
		h += r-l;
		while (*t != '\0' && l < r && *t == str[l]) {
			t++;
			l++;
		}
	
		//mismatch
		if (*t != '\0' && l < r)
			return NULL;
		
		//se o padrão não terminou, continua com o próximo vértice
		v = next->node;
		//se terminou, tudo bem, vai sair do laço
	}
	
	//o padrão acabou
	return v;
}

void mark_occurrences(Node v, int h) {
	if (!v->has_transitions()) {
		//se for uma folha, calcula a posição de começo da ocorrência
		int pos = s_len + 1 - h;
		
		//e encontra a linha correspondente
		int line = std::upper_bound(line_end, line_end+lines, pos) - line_end;
		if (line < lines)
			occ.insert(line);
	} else {
		for(std::map<char, INode>::iterator it = v->next->begin(); it != v->next->end(); it++) {
			int l = it->second->l, r = it->second->r;
			mark_occurrences(it->second->node, h + (r-l));
		}
	}
}

void print_tree() {
	puts(str);
	printf("%d\n", node_count);
	
	for(int i = 0; i < node_count; i++) {
		printf("NO %d\n", i);
		
		if (nodes[i].has_transitions()) {
			for(std::map<char, INode>::iterator it = nodes[i].next->begin(); it != nodes[i].next->end(); it++) {
				int l = it->second->l, r = it->second->r;
				printf("(%d %d)\n", l, r);
				while (l < r) 
					putchar(str[l++]);
				printf(" %d\n", it->second->node-nodes);
			}
		}
		
		puts("");
	}
}

