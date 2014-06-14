#include "Scene.h"
#include <pugixml.hpp>
#include <stdexcept>
#include <sstream>

using namespace std;

Scene::Scene()
{
}

Scene Scene::readFromPath(const string &path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
    {
		stringstream ss;
		ss << "Couldn't parse '" << path << "'" << ": " << result.description();
		if(result.offset > 0)
		{
			ss << ". Offset: " << result.offset;
		}

		throw invalid_argument(ss.str().c_str());
    }

	Scene res;
	return res;
}
