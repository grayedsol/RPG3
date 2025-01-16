#include "QuadTree.hpp"
#include "GRY_Log.hpp"

static const unsigned int THRESHOLD = 6;
static const unsigned int NUM_QUADRANTS = 4;
static const float MIN_BOX_SIZE = 2.0f;

static void query(const QuadNode* node, Hitbox box, ECS::entity e, std::vector<Hitbox>& out);
static void insert(QuadNode* node, Hitbox box, ECS::entity e);
static void mergeLeaves(QuadNode* node);

static Hitbox quadrantRect(Hitbox box, int quadrant);
static bool isWithin(const Hitbox outerBox, const Hitbox innerBox);

QuadTree::QuadTree(Hitbox area) : node(area) {
}

void QuadTree::insert(Hitbox box, ECS::entity e) {
	::insert(&node, box, e);
}

void QuadTree::query(Hitbox box, ECS::entity e, std::vector<Hitbox>& out) const {
	::query(&node, box, e, out);
}

void QuadTree::reset() {
	for (auto child : node.nodes) { delete child; }
	node.nodes.clear();
}

void query(const QuadNode *node, Hitbox box, ECS::entity e, std::vector<Hitbox> &out) {
	auto collides = [box](const Hitbox other) {
		return
			box.x + box.w > other.x &&
			box.x < other.x + other.w &&
			box.y + box.h > other.y &&
			box.y < other.y + other.h;
	};

	for (auto child : node->nodes) {
		if (!collides(child->area)) { continue; }

		if (child->type == QuadNode::Leaf && child->e != e) {
			out.push_back(child->area);
		}
		else { query(child, box, e, out); }
	}
}

void insert(QuadNode *node, Hitbox box, ECS::entity e) {
	for (auto child : node->nodes) {
		if (child->type == QuadNode::Leaf) { continue; }
		if (isWithin(child->area, box)) {
			insert(child, box, e);
			return;
		}
	}

	node->nodes.push_back(new QuadNode(box, e));
	if (node->nodes.size() > THRESHOLD) { mergeLeaves(node); }
}

void mergeLeaves(QuadNode* node) {
	/* Make sure the area won't turn out too small when we create a new node */
	if (node->area.w < MIN_BOX_SIZE || node->area.h < MIN_BOX_SIZE) {
		GRY_Log("QuadTree: Minimum size reached for area."); { return; }
	}

	for (int i = 0; i < NUM_QUADRANTS; i++) {
		Hitbox box = quadrantRect(node->area, i);
		unsigned count = 0;
		for (auto& child : node->nodes) {
			if (child->type == QuadNode::Branch || !isWithin(box, child->area)) { continue; }
			count++;
		}
		if (count < 2) { continue; }

		QuadNode* newNode = new QuadNode(box);
		newNode->type = QuadNode::Branch;
		for (int j = node->nodes.size() - 1; j >= 0; j++) {
			QuadNode* child = node->nodes.at(j);
			if (child->type == QuadNode::Branch || !isWithin(box, child->area)) { continue; }
			insert(newNode, child->area, child->e);

			std::swap(node->nodes.at(j), node->nodes.back());
			delete node->nodes.back();
			node->nodes.pop_back();
		}
		node->nodes.push_back(newNode);
		break;
	}

	if (node->nodes.size() > THRESHOLD) {
		GRY_Log("[QuadTree] Could not reduce below threshold.");
	}
}

static Hitbox quadrantRect(Hitbox box, int quadrant) {
	if (quadrant < 0 || quadrant > 3) {
		GRY_Log("QuadTree::quadrantRect: quadrant must be a number from 0 to 3 (was %d)", quadrant);
		quadrant = 0;
	}
	return Hitbox{
		box.x + ((quadrant % 2) * (box.w * 0.5f)),
		box.y + ((quadrant / 2) * (box.h * 0.5f)),
		box.w * 0.5f, box.h * 0.5f
	};
}

static bool isWithin(const Hitbox outerBox, const Hitbox innerBox) {
	return
		innerBox.x >= outerBox.x &&
		innerBox.y >= outerBox.y &&
		innerBox.x + innerBox.w <= outerBox.x + outerBox.w &&
		innerBox.y + innerBox.h <= outerBox.y + outerBox.h;
}
