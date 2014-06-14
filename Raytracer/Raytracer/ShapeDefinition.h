#pragma once
#include <string>

class ShapeDefinition
{
private:
	std::string m_id;
protected:
	ShapeDefinition();
	ShapeDefinition(const std::string& id);
public:
	std::string id() const;
	virtual ~ShapeDefinition();
};

