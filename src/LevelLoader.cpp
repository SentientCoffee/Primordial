#include "LevelLoader.h"

LevelLoader::LevelLoader(const char* filename)
{
	char tempName[256];
	bool start = true;

	FILE* file = fopen(filename, "r");
	if (file == NULL)
		printf("Failed to open file\n");
	bool moreFile = true;
	while (moreFile)
	{
		char line[1024];
		int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF){//if end of file
			moreFile = false;
		}
		else if (strcmp(line, "o") == 0){//if new object
			fscanf(file, "%s\n", &tempName);//get name of object
		}
		else if (strcmp(line, "v") == 0){//if vertex
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
		}
		else if (strcmp(line, "s") == 0)//end of new object
		{

		}
	}
}
