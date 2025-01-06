#pragma once
#include <vector>
#include "Components.hpp"

struct QuadNode {
	Hitbox area;

	std::vector<QuadNode*> nodes;

	enum {
		Leaf, Branch
	} type = Leaf;

	QuadNode(Hitbox area) : area(area) {}

	~QuadNode() {
		for (auto node : nodes) { delete node; }
	}
};

struct QuadTree {
	QuadNode node;

	QuadTree(Hitbox area);

	void insert(Hitbox box);

	void query(Hitbox box, std::vector<Hitbox>& out) const;
};