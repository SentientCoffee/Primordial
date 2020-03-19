#pragma once
#include <vector>
#include "glm/common.hpp"
#include "Cappuccino/HitBox.h"
#include <memory>

struct navPoint {
public:
	glm::vec3 _position;
	std::vector<std::shared_ptr<navPoint>> connections;

};

class NavMeshLoader
{
public:
	NavMeshLoader(const char* filename);
	std::vector <std::shared_ptr<navPoint>> navmesh;
private:
	/*
	Purp:This function will find the center of the given verts
	Pre: None
	Post: A vec3 of the center of the given verts
	*/
	glm::vec3 findCenter();
	/*
	Purp: This function will find the size of cube hitbox
	Pre: None
	Post: A vec3 of the size of the cube hitBox
	*/
	glm::vec3 findBox();
	std::vector <glm::vec3> _tempVerts;
	std::vector<Cappuccino::HitBox>_boxes;
	Cappuccino::HitBox overall;
};