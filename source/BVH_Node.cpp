#include "../include/BVH_Node.h"

BVH_Node::BVH_Node(int index, int quantity,
				   AABB aabb,
				   std::shared_ptr<BVH_Node> left,
				   std::shared_ptr<BVH_Node> right) {
	this->index    = index;
	this->quantity = quantity;
	this->aabb     = aabb;
	this->left     = left;
	this->right    = right;
}

BVH_Node::BVH_Node(std::shared_ptr<BVH_Node> node) {
	this->index    = node->get_index();
	this->quantity = node->get_quantity();
	this->aabb     = node->aabb;
	this->left     = node->get_left();
	this->right    = node->get_right();
}

BVH_Node::~BVH_Node() {
}

int BVH_Node::get_index() const {
	return this->index;
}

int BVH_Node::get_quantity() const {
	return this->quantity;
}

AABB& BVH_Node::get_AABB_reference() {
	return this->aabb;
}

int BVH_Node::get_longest_span_axis() const {
	double x_span = aabb.get_x_span();
	double y_span = aabb.get_y_span();
	double z_span = aabb.get_z_span();

	if (x_span >= y_span && x_span >= z_span) {
		return 0;
	} else if (y_span >= x_span && y_span >= z_span) {
		return 1;
	} else {
		return 2;
	}

	return 0;
}

std::shared_ptr<BVH_Node> BVH_Node::get_left() const {
	return this->left;
}

std::shared_ptr<BVH_Node> BVH_Node::get_right() const {
	return right;
}

void BVH_Node::set_index(const int index) {
	this->index = index;
}

void BVH_Node::set_quantity(const int quantity) {
	this->quantity = quantity;
}

void BVH_Node::set_left(std::shared_ptr<BVH_Node> left) {
	this->left = left;
}

void BVH_Node::set_right(std::shared_ptr<BVH_Node> right) {
	this->right = right;
}

bool BVH_Node::is_leaf() const {
	return this->left == NULL && this->right == NULL;
}

bool BVH_Node::hit(const Ray& ray) {
	return this->aabb.hit(ray);
}