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
		if (lineNumber == EOF)
		{

		}
	}
}
