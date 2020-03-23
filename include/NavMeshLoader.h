#pragma once
#include <vector>
#include "glm/common.hpp"
#include "Cappuccino/HitBox.h"
#include <memory>

struct NavPoint {
public:
	NavPoint() {}
	NavPoint(std::shared_ptr<NavPoint>in);
	void reset();
	glm::vec3 _position = glm::vec3(0.0f);
	std::vector<std::shared_ptr<NavPoint>> _connections;


	std::shared_ptr<NavPoint> _lastConnection;
	float Hcost = -1.0f;
	float Gcost = -1.0f;
	float Fcost = -1.0f;
	bool start = false;
};


class NavMeshLoader
{
public:
	NavMeshLoader(const char* filename);
	std::vector <std::shared_ptr<NavPoint>> navmesh;
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

	void printMesh();
	std::vector <glm::vec3> _tempVerts;
	std::vector<Cappuccino::HitBox>_boxes;
	Cappuccino::HitBox overall;
};