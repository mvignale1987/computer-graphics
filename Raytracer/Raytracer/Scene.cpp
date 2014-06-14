#include "Scene.h"
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace pugi;

Scene::Scene()
{
}

Scene Scene::readFromPath(const string &path)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(path.c_str());

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

	res.bgColor = readBackgroundColor(doc);

	return res;
}

Vector3 Scene::backgroundColor()
{
	return bgColor;
}

Vector3 Scene::readBackgroundColor(const xml_document &doc)
{
	return Vector3::fromHTML(doc.child("scene").child("backgroundColor").text().as_string());
}