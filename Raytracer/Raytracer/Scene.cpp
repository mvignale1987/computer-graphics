#include "Scene.h"
#include "SceneObjectSphere.h"
#include "SceneObjectCyllinder.h"
#include "SceneObjectTriangle.h"
#include "SceneObjectQuad.h"
#include <stdexcept>
#include <sstream>
#include <shlobj.h>

using namespace std;
using namespace pugi;

Scene::Scene():
	m_imageWidth(0),
	m_imageHeight(0)
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
	res.m_lights = readLights(sceneNode);
	res.materials = readMaterials(sceneNode);
	res.shapeDefinitions = readShapeDefinitions(sceneNode);
	res.m_objects = readSceneObjects(res, sceneNode); 
	res.m_outputDir = readOutputDir(sceneNode);
	readSceneResolution(sceneNode, res.m_imageWidth, res.m_imageHeight);
	res.m_rayDepth = (short) intFromChild(sceneNode, "rayDepth", 3);
	res.m_supersampling = (short) intFromChild(sceneNode, "superSampling", 1);

	return res;
}

Vector3 Scene::backgroundColor() const
{
	return bgColor;
}

Camera Scene::camera() const
{
	return cam;
}

int Scene::imageWidth() const
{
	return m_imageWidth;
}

int Scene::imageHeight() const
{
	return m_imageHeight;
}

short Scene::rayDepth() const
{
	return m_rayDepth;
}

short Scene::supersampling() const
{
	return m_supersampling;
}

string Scene::outputDir() const
{
	return m_outputDir;
}

vector<SceneObject *>& Scene::objects()
{
	return m_objects;
}

vector<Light>& Scene::lights()
{
	return m_lights;
}


Vector3 Scene::readBackgroundColor(const xml_node &scene)
{
	return colorFromChild(scene, "backgroundColor");
}

Camera Scene::readCamera(const xml_node &scene)
{
	xml_node node = scene.child("camera");
	if(!node)
		throw domain_error("readCamera: Couldn't found <camera> node");

	return Camera (
		vectorFromChild(node, "position"),
		vectorFromChild(node, "lookAt"),
		vectorFromChild(node, "up"),
		floatFromChild(node, "near"),
		floatFromChild(node, "far"),
		floatFromChild(node, "fieldOfView")
		);
}

void Scene::readSceneResolution(const xml_node &scene, int& outWidth, int& outHeight)
{
	xml_node node = scene.child("resolution");
	if(!node)
		throw domain_error("readSceneResolution: Couldn't found <resolution> node");

	stringstream ss;
	ss << node.text().as_string();
	ss >> outWidth >> outHeight;
	if(ss.fail())
		throw domain_error("Bad resolution format. Couldn't read resolution x & y");
}


string stringReplace( string src, string const& target, string const& repl)
{
	if (target.length() == 0) {
		// searching for a match to the empty string will result in
		// an infinite loop
		// it might make sense to throw an exception for this case
		return src;
	}
 
	if (src.length() == 0) {
		return src; // nothing to match against
	}
 
	for (size_t idx = src.find( target); idx != string::npos; idx = src.find( target, idx)) {
		src.replace( idx, target.length(), repl);
		idx += repl.length();
	}
 
	return src;
}

string Scene::readOutputDir(const xml_node &scene)
{
	xml_node node = scene.child("outputDir");
	if(!node)
		throw domain_error("readSceneResolution: Couldn't found <outputDir> node");

	string dir = node.text().as_string();
	char desktopPathWide[MAX_PATH+1];
	desktopPathWide[MAX_PATH] = '\0';
	if (!SHGetSpecialFolderPath(HWND_DESKTOP, desktopPathWide, CSIDL_DESKTOP, FALSE))
	{ 
		throw domain_error("Couldn't get Desktop path");
    }

	return stringReplace(dir, "%DESKTOP%", desktopPathWide);
}

vector<Light> Scene::readLights(const xml_node &scene)
{
	xml_node lightsNode = scene.child("lights");
	if(!lightsNode)
		throw domain_error("readLights: Couldn't found <lights> node");

	vector<Light> res;

	for (xml_node node = lightsNode.first_child(); node; node = node.next_sibling())
	{
		if(string(node.name()) != "light")
		{
			stringstream ss;
			ss << "Encountered an invalid <" << node.name() << "> element in the lights element.";
			throw domain_error(ss.str().c_str());
		}

		Light light(
			vectorFromChild(node, "position"),
			colorFromChild(node, "ambientColor"),
			colorFromChild(node, "diffuseColor"),
			floatFromChild(node, "linearAttenuation"),
			floatFromChild(node, "quadAttenuation")
			);

		res.push_back(light);
	}

	if(res.empty()){
		throw domain_error("No lights found. The scene must have at least one light.");
	}

	return res;
}

map<string, Material *> Scene::readMaterials(const xml_node &scene)
{
	xml_node materialsNode = scene.child("materials");
	if(!materialsNode)
		throw domain_error("readMaterials: Couldn't found <materials> node");

	map<string, Material *> res;

	for (xml_node node = materialsNode.first_child(); node; node = node.next_sibling())
	{
		if(string(node.name()) != "material")
		{
			stringstream ss;
			ss << "Encountered an invalid <" << node.name() << "> element in the material element.";
			throw domain_error(ss.str().c_str());
		}
		
		string materialId = idFromNode(node);

		Material *material = new Material(
			materialId,
			colorFromChild(node, "ambientColor", Vector3::zero),
			floatFromChild(node, "ambientCoefficient", 0),
			colorFromChild(node, "diffuseColor", Vector3::zero),
			floatFromChild(node, "diffuseCoefficient", 0),
			colorFromChild(node, "specularColor", Vector3::zero),
			floatFromChild(node, "specularCoefficient", 0),
			intFromChild(node, "specularExponent", 1),
			stringAttributeFromNode(node, "mirrored", "false") == "true",
			stringAttributeFromNode(node, "refractive", "false") == "true",
			floatFromChild(node, "refractionIndex", 1),
			floatFromChild(node, "transparency", 0),
			stringAttributeFromNode(node, "texture", "")
			);

		pair<map<string, Material *>::iterator, bool> ret = 
			res.insert (pair<string , Material *>(materialId, material));
		if (ret.second == false) {
			stringstream ss;
			ss << "Duplicate material '" << materialId << "' encountered." <<
				"Each material id attribute must be unique";
			throw domain_error(ss.str().c_str());
		}
	}

	return res;
}


map<string, ShapeDefinition *> Scene::readShapeDefinitions(const xml_node &scene)
{
	xml_node defsNode = scene.child("defs");
	if(!defsNode)
		throw domain_error("readShapeDefinitions: Couldn't found <defs> node");

	map<string, ShapeDefinition *> res;

	for (xml_node node = defsNode.first_child(); node; node = node.next_sibling())
	{
		string nodeName = node.name();

		ShapeDefinition *shape;

		if(nodeName == "sphere") {
			shape = readSphere(node);
		} else if(nodeName == "cyllinder") {
			shape = readCyllinder(node);
		} else if(nodeName == "quad") {
			shape = readQuad(node);
		} else if(nodeName == "triangle") {
			shape = readTriangle(node);
		} else {
			stringstream ss;
			ss << "Encountered an invalid <" << nodeName << "> element in the defs element.";
			throw domain_error(ss.str().c_str());
		}
		
		pair<map<string, ShapeDefinition *>::iterator, bool> ret = 
			res.insert (pair<string , ShapeDefinition *>(shape->id(), shape));
		if (ret.second == false) {
			stringstream ss;
			ss << "Duplicate shape '" << shape->id() << "' encountered." <<
				"Each shape id attribute must be unique";
			throw domain_error(ss.str().c_str());
		}
	}

	return res;
}

vector<SceneObject *> Scene::readSceneObjects(Scene& scene, const xml_node &sceneNode)
{
	xml_node objectsNode = sceneNode.child("objects");
	if(!objectsNode)
		throw domain_error("readSceneObjects: Couldn't found <objects> node");

	vector<SceneObject *> res;

	for (xml_node node = objectsNode.first_child(); node; node = node.next_sibling())
	{
		if(string(node.name()) != "object")
		{
			stringstream ss;
			ss << "Encountered an invalid <" << node.name() << "> element in the objects element.";
			throw domain_error(ss.str().c_str());
		}

		string shapeDefinitionId = stringAttributeFromNode(node, "def");
		ShapeDefinition *shapeDefinition = scene.getShapeDefinition(shapeDefinitionId);

		string materialId = stringAttributeFromNode(node, "material");
		Material *material = scene.getMaterial(materialId);
		
		xml_node positionNode = node.child("position");
		Vector3 position = positionNode ? vectorFromNode(positionNode) : Vector3::zero;
		xml_node rotationNode = node.child("rotation");
		Vector3 rotation = rotationNode ? vectorFromNode(rotationNode) : Vector3::zero;



		switch(shapeDefinition->shapeType())
		{
		case Shape::SPHERE:
			res.push_back(new SceneObjectSphere(material, position, shapeDefinition->asSphere().radius()));
			break;
		case Shape::CYLLINDER:
			res.push_back(new SceneObjectCyllinder(material, position,
				shapeDefinition->asCyllinder().radius(),
				shapeDefinition->asCyllinder().height()
			));
			break;
		case Shape::QUAD:
			res.push_back(new SceneObjectQuad(
				material,
				shapeDefinition->asQuad().points()[0] + position,
				shapeDefinition->asQuad().points()[1] + position,
				shapeDefinition->asQuad().points()[2] + position,
				shapeDefinition->asQuad().points()[3] + position
			));
			break;
		case Shape::TRIANGLE:
			res.push_back(new SceneObjectTriangle(
				material,
				shapeDefinition->asTriangle().points()[0] + position,
				shapeDefinition->asTriangle().points()[1] + position,
				shapeDefinition->asTriangle().points()[2] + position
			));
			break;
		default:
			break;
		}
	}

	return res;
}

Cyllinder *Scene::readCyllinder(const xml_node &node)
{
	string id = idFromNode(node);
	return new Cyllinder(
		id,
		floatFromChild(node, "height"),
		floatFromChild(node, "radius")
		);
}

Sphere *Scene::readSphere(const xml_node &node)
{
	string id = idFromNode(node);
	return new Sphere(
		id,
		floatFromChild(node, "radius")
		);
}

Triangle *Scene::readTriangle(const xml_node &node)
{
	string id = idFromNode(node);
	vector<Vector3> positions;
	for (xml_node positionNode = node.child("position");
		positionNode;
		positionNode = positionNode.next_sibling("position"))
	{
		positions.push_back(vectorFromNode(positionNode));
	}
	if(positions.size() != 3){
		stringstream ss;
		ss << "Expected 3 position elements in node but encountered " << positions.size();
		throw domain_error(ss.str().c_str());
	}
	return new Triangle(id, positions[0], positions[1], positions[2]);
}

Quad *Scene::readQuad(const xml_node &node)
{
	string id = idFromNode(node);
	vector<Vector3> positions;
	for (xml_node positionNode = node.child("position");
		positionNode;
		positionNode = positionNode.next_sibling("position"))
	{
		positions.push_back(vectorFromNode(positionNode));
	}
	if(positions.size() != 4){
		stringstream ss;
		ss << "Expected 4 position elements in node but encountered " << positions.size();
		throw domain_error(ss.str().c_str());
	}
	return new Quad(id, positions[0], positions[1], positions[2], positions[3]);
}

Vector3 Scene::vectorFromNode(const xml_node &node)
{
	stringstream ss;
	ss << node.text().as_string();
	float x,y,z;
	ss >> x >> y >> z;
	if(ss.fail())
		throw domain_error("Bad vector format. Couldn't read all coordinates");
	return Vector3(x, y, z);
}

Vector3 Scene::vectorFromChild(const xml_node &node, const string &child, const Vector3& defaultValue)
{
	xml_node childNode = node.child(child.c_str());
	return childNode ? vectorFromNode(childNode) : defaultValue;	
}

Vector3 Scene::vectorFromChild(const xml_node &node, const string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node <"<< node.name() << "> has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}
	return vectorFromNode(childNode);	
}

Vector3 Scene::colorFromChild(const xml_node &node, const string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node <"<< node.name() << "> has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}

	return Vector3::fromHTML(childNode.text().as_string());
}

Vector3 Scene::colorFromChild(const xml_node &node, const string &child, const Vector3& defaultColor)
{
	xml_node childNode = node.child(child.c_str());
	return childNode ? Vector3::fromHTML(childNode.text().as_string()) : defaultColor;
}

float Scene::floatFromChild(const xml_node &node, const string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node <"<< node.name() << "> has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}
	return childNode.text().as_float();
}

float Scene::floatFromChild(const xml_node &node, const string &child, float defaultValue)
{
	xml_node childNode = node.child(child.c_str());
	return childNode ? childNode.text().as_float() : defaultValue;
}

int Scene::intFromChild(const xml_node &node, const string &child)
{
	xml_node childNode = node.child(child.c_str());
	if(!childNode){
		stringstream ss;
		ss << "node <"<< node.name() << "> has no child named '" << child << "'";
		throw invalid_argument(ss.str().c_str());
	}
	return childNode.text().as_int();
}

int Scene::intFromChild(const xml_node &node, const string &child, int defaultValue)
{
	xml_node childNode = node.child(child.c_str());
	return childNode ? childNode.text().as_int() : defaultValue;
}

string Scene::idFromNode(const xml_node &node)
{
	return stringAttributeFromNode(node, "id");
}

string Scene::stringAttributeFromNode(const xml_node &node, const string& attributeName)
{
	xml_attribute attr = node.attribute(attributeName.c_str());
	if(!attr){
		stringstream ss;
		ss << "node <"<< node.name() << "> has no '" << attributeName << "' attribute";
		throw invalid_argument(ss.str().c_str());
	}
	return attr.as_string();
}

string Scene::stringAttributeFromNode(const xml_node &node, const string& attributeName, const string& defaultValue)
{
	xml_attribute attr = node.attribute(attributeName.c_str());
	return attr ? attr.as_string() : defaultValue;
}

ShapeDefinition *Scene::getShapeDefinition(const string& id)
{
	map<string, ShapeDefinition *>::iterator it;
	it = shapeDefinitions.find(id);
	if(it == shapeDefinitions.end())
	{
		stringstream ss;
		ss << "Unknown shape definition: " << id;
		throw invalid_argument(ss.str().c_str());
	}
	return it->second;
}

Material *Scene::getMaterial(const string& id)
{
	map<string, Material *>::iterator it;
	it = materials.find(id);
	if(it == materials.end())
	{
		stringstream ss;
		ss << "Unknown material: " << id;
		throw invalid_argument(ss.str().c_str());
	}
	return it->second;
}
