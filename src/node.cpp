#include "node.h"

#include <dirent.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

unsigned long long fsize(const std::string &dir) {
	std::ifstream in(dir, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

node::node() {
	parent = nullptr;
	size = 0;
}

node::~node() {
	for (auto i : children) {
		delete i;
	}
}

bool node::traverse(node *cur_node) {
	DIR *cur_dir;
	struct dirent *cur;

	cur_dir = opendir(cur_node->dir.c_str());

	if (cur_dir == NULL) {
		return true;
	}

	if (cur_node->dir.back() == '/') {
		cur_node->dir.pop_back();
	}
	unsigned long long tot_sz = 0;

	if (cur_dir) {
		while ((cur = readdir(cur_dir))) {
			std::string cur_sub = cur->d_name;
			if (cur_sub == "." || cur_sub == ".." || cur_sub == "proc") {
				continue;
			}

			switch (cur->d_type) {
				case DT_REG: {
					unsigned long long cur_sz =
						fsize(cur_node->dir + '/' + cur->d_name);
					tot_sz += cur_sz;
					cur_node
						->new_cild(cur_node->dir + '/' + cur->d_name,
								   cur->d_name, cur_node)
						->terminate(cur_sz);
					break;
				}
				case DT_DIR: {
					std::string new_dir = cur_node->dir + '/' + cur->d_name;
					node *child =
						cur_node->new_cild(new_dir, cur->d_name, cur_node);
					traverse(child);
					tot_sz += child->size;
					break;
				}
			}
		}
		closedir(cur_dir);
	}

	cur_node->size = tot_sz;

	return false;
}

void set_sz(node *cur) {
	for (auto i : cur->children) {
	}
}

node *node::new_cild(std::string name, std::string fname, node *in_parent) {
	node *new_node = new node;
	new_node->dir = name;
	new_node->parent = in_parent;
	new_node->name = fname;
	children.push_back(new_node);
	return new_node;
}

bool node::terminate(unsigned long long sz) {
	size = sz;
	return false;
}

node::node(std::string in) {
	size = 0;
	dir = in;
	parent = nullptr;
}