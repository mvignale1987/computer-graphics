#include "ShapeDefinition.h"

using namespace std;

ShapeDefinition::ShapeDefinition()
{
}

ShapeDefinition::ShapeDefinition(const string& id):
	m_id(id)
{
}

string ShapeDefinition::id() const
{
	return m_id;
}

ShapeDefinition::~ShapeDefinition()
{
}
