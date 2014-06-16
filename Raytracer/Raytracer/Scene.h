#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL.h>
#include <pugixml.hpp>
#include "Vector3.h"
#include "Camera.h"
#include "Cyllinder.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Quad.h"
#include "Light.h"
#include "Material.h"
#include "ShapeDefinition.h"
#include "SceneObject.h"

class Scene 
{
private:
	std::vector<Light> lights;
	std::map<std::string, Material *> materials;
	std::map<std::string, ShapeDefinition *> shapeDefinitions;
	std::vector<SceneObject> objects;
	Vector3 bgColor;
	Camera cam;
public:
	Scene();
	static Scene readFromPath(const std::string &path);
	
	Vector3 backgroundColor();
private:
	// métodos para lectura de los componentes principales de la escena
	static std::vector<Light> readLights(const pugi::xml_node &scene);
	static std::map<std::string, Material *> readMaterials(const pugi::xml_node &scene);
	static std::map<std::string, ShapeDefinition *> readShapeDefinitions(const pugi::xml_node &scene);
	static std::vector<SceneObject> readSceneObjects(Scene& scene, const pugi::xml_node &sceneNode);
	static Vector3 readBackgroundColor(const pugi::xml_node &scene);
	static Camera  readCamera(const pugi::xml_node &scene);

	// para leer formas básicas
	static Cyllinder *readCyllinder(const pugi::xml_node &node);
	static Sphere *readSphere(const pugi::xml_node &node);
	static Triangle *readTriangle(const pugi::xml_node &node);
	static Quad	*readQuad(const pugi::xml_node &node);

	// métodos auxiliares para leer elementos comunes
	static Vector3 vectorFromChild(const pugi::xml_node &node, const std::string &child);
	static Vector3 vectorFromNode(const pugi::xml_node &node);
	static Vector3 colorFromChild(const pugi::xml_node &node, const std::string &child);
	static float  floatFromChild(const pugi::xml_node &node, const std::string &child);
	static int  intFromChild(const pugi::xml_node &node, const std::string &child);
	static std::string idFromNode(const pugi::xml_node &node);
	static std::string stringAttributeFromNode(const pugi::xml_node &node, const std::string& attributeName);

	// métodos de acceso internos
	ShapeDefinition *getShapeDefinition(const std::string& id);
	Material *getMaterial(const std::string& id);
};