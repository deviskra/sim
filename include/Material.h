#ifndef MATERIAL_H 
#define MATERIAL_H 1

#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "G4Material.hh"
class Material
{
	public:
		Material();
		~Material();
		G4Material*	get_Material(std::string Name);
		void	set_Material(std::string Name, G4Material* material);
	private:
		std::map<std::string, G4Material*> MaterialBase;
};
#endif