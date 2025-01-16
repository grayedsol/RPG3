/**
 * @file QuadTree.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief QuadTree
 * @copyright Copyright (c) 2025
 */
#pragma once
#include <vector>
#include "Components.hpp"
#include "ECS.hpp"

/**
 * @brief A node of a QuadTree.
 * 
 */
struct QuadNode {
	/**
	 * @brief The area that the node encompasses.
	 * 
	 * @details
	 * If the node is a leaf, this is the area of the entity's hitbox.
	 * If the node is a branch, this is the area that surrounds the contained leaf nodes.
	 */
	Hitbox area;

	/**
	 * @brief Child nodes of the node.
	 * 
	 */
	std::vector<QuadNode*> nodes;

	/**
	 * @brief The type of the node.
	 * 
	 */
	enum {
		Leaf, Branch
	} type = Leaf;

	/**
	 * @brief The entity id associated with the node's hitbox.
	 * 
	 * @details
	 * If the node is a leaf, this should never be ECS::NONE.
	 * If the node is a branch, this should always be ECS::NONE.
	 */
	ECS::entity e = ECS::NONE;

	QuadNode(Hitbox area) : area(area), type(QuadNode::Branch) {}
	QuadNode(Hitbox area, ECS::entity e) : area(area), e(e) {}

	~QuadNode() {
		for (auto node : nodes) { delete node; }
	}
};

/**
 * @brief Data structure that efficiently detects collisions.
 * 
 */
struct QuadTree {
	/**
	 * @brief Starting node of the QuadTree.
	 * 
	 */
	QuadNode node;

	/**
	 * @brief Constructor.
	 * 
	 * @param area Area that the QuadTree encompasses.
	 */
	QuadTree(Hitbox area);

	/**
	 * @brief Insert a value into the QuadTree.
	 * 
	 * @param box Hitbox of the entity
	 * @param e The id of the entity
	 */
	void insert(Hitbox box, ECS::entity e);

	/**
	 * @brief Query for collisions within the QuadTree.
	 * 
	 * @param box Hitbox of the entity
	 * @param e The id of the entity (collisions with itself will be ignored)
	 * @param out A vector of Hitboxes that collide with the entity
	 */
	void query(Hitbox box, ECS::entity e, std::vector<Hitbox>& out) const;

	/**
	 * @brief Safely resets the QuadTree to an empty state.
	 * 
	 */
	void reset();
};