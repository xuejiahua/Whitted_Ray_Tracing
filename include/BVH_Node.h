#pragma once

#include "AABB.h"
#include "Point3.h"
#include "constants.h"
#include "Primitive.h"

class BVH_Node {
public:
	// Constructors and Destructor
	// ---------------------------
	BVH_Node(int index, int quantity,
			 AABB aabb = AABB(Point3( INF,  INF,  INF), 
						      Point3(-INF, -INF, -INF)),
			 std::shared_ptr<BVH_Node> left = NULL,
			 std::shared_ptr<BVH_Node> right = NULL);
	BVH_Node(std::shared_ptr<BVH_Node> node);
	~BVH_Node();

	// Getters and Setters
	// -------------------
	int get_index() const;
	int get_quantity() const;
	AABB& get_AABB_reference();
	int get_longest_span_axis() const;
	std::shared_ptr<BVH_Node> get_left() const;
	std::shared_ptr<BVH_Node> get_right() const;

	void set_index(const int index);
	void set_quantity(const int quantity);
	void set_left(std::shared_ptr<BVH_Node> left);
	void set_right(std::shared_ptr<BVH_Node> right);

	// Other functions
	// ---------------
	bool is_leaf() const;

	bool hit(const Ray& ray);

private:
	int index;           // Range [index, index + quantity) in the arry Scene::primitives were recorded in this node.
	int quantity;        // The quantity of primitives recorded in this node.

	AABB aabb;           // Axis-Aligning Bounding Box.

	std::shared_ptr<BVH_Node> left, right; // Pointers to the left/right subtrees.
};