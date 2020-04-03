#pragma once
#include "LevelComponents.h"

#include <glm/glm.hpp>
#include <vector>

class LevelLoader {
public:
	LevelLoader(const char* filename);
	/*
	Purp: This function rotates all the level data
	Pre: a float for rotation
	Post: None
	*/
	void rotate(float rotation);

	DoorLoc _entrance;
	std::vector<DoorLoc> _exits;
	std::vector<glm::vec4> _lights;
	std::vector<glm::vec3> chests;
	glm::vec3 _respawnPoint;
	glm::vec3 _shopLocation;
	std::vector<HurtBox>_hurtboxes;
	std::vector<GravLift>_lifts;
	std::vector<TeleporterLoc>_teleporterLoc;
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
	std::vector<glm::vec3> _tempVerts;
};
