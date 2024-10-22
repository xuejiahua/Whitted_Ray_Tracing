#include "../include/Primitive.h"

AABB& Primitive::get_AABB_reference() {
    return this->aabb;
}

AABB Primitive::get_AABB() {
    return this->aabb;
}
