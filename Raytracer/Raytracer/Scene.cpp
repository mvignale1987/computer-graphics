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

	xml_node sceneNode = doc.child("scene");
	if(!sceneNode)
	{
		throw domain_error("Couldn't found <scene> node");
	}

	res.bgColor = readBackgroundColor(sceneNode);
	res.cam = readCamera(sceneNode);

	return res;
}

Vector3 Scene::backgroundColor()
{
	return bgColor;
}

Vector3 Scene::readBackgroundColor(const xml_node &scene)
{
	xml_node node = scene.child("backgroundColor");
	if(!node)
		throw domain_error("readBackgroundColor: Couldn't found <backgroundColor> node");
	return Vector3::fromHTML(node.text().as_string());
}

Camera Scene::readCamera(const xml_node &scene)
{
	xml_node node = scene.child("camera");
	if(!node)
		throw domain_error("readCamera: Couldn't found <camera> node");

	return Camera (
		vectorFromChild(node, "position"),
		vectorFromChild(node, "rotation"),
		vectorFromChild(node, "up"),
		floatFromChild(node, "near"),
		floatFromChild(node, "far"),
		floatFromChild(node, "fieldOfView")
		);
}


Vector3 Scene::vectorFromChild(const pugi::xml_node &node, const std::string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}

	stringstream ss;
	ss << childNode.text().as_string();
	float x,y,z;
	ss >> x >> y >> z;
	if(ss.fail())
		throw domain_error("Bad vector format. Couldn't read all coordinates");
	return Vector3(x, y, z);
}

float Scene::floatFromChild(const pugi::xml_node &node, const std::string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}
	return childNode.text().as_float();
}