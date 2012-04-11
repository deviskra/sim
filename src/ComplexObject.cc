#include "ComplexObject.hh"

ComplexObject::ComplexObject()
{
	xmlFileName = "ComplexObject.xml";
}
ComplexObject::ComplexObject(std::string pathfilename_)
{
	if (boost::filesystem::exists(pathfilename_) == true)
	{
		xmlFileName = pathfilename_ ;
		load_xml();
	}else{}
}
ComplexObject::~ComplexObject()
{

}
template<class Archive>
void ComplexObject::save(Archive & ar,const unsigned int version) const
{
	ar & BOOST_SERIALIZATION_NVP(VectorOperation); 
	ar & BOOST_SERIALIZATION_NVP(NameMat);
	ar & BOOST_SERIALIZATION_NVP(NameCompObj);
}
template<class Archive>
void ComplexObject::load(Archive & ar,const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(VectorOperation);
	ar & BOOST_SERIALIZATION_NVP(NameMat);
	ar & BOOST_SERIALIZATION_NVP(NameCompObj);
}
void ComplexObject::save_xml()
{
	std::ofstream ofs(xmlFileName.c_str());
    	assert(ofs.good());
    	boost::archive::xml_oarchive oa(ofs);	
	oa << boost::serialization::make_nvp("class_obj",*this);
}
void ComplexObject::load_xml()
{
	std::ifstream ifs(xmlFileName.c_str());
    	assert(ifs.good());
    	boost::archive::xml_iarchive ia(ifs);
	ia >> boost::serialization::make_nvp("class_obj",*this);
}


G4VSolid*	ComplexObject::Union_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector)
{
	return  new G4UnionSolid(Name, pSolidObj1, pSolidObj2, &oMatrix, oVector);
}

G4VSolid*	ComplexObject::Intersection_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector)
{
	return  new G4IntersectionSolid(Name, pSolidObj1, pSolidObj2, &oMatrix, oVector);
}

G4VSolid*	ComplexObject::Subtraction_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector)
{
	return  new G4SubtractionSolid(Name, pSolidObj1, pSolidObj2, &oMatrix, oVector);
}

G4VSolid* 	ComplexObject::Create_solid(SolidParam solid)
{
	if (solid.sType == fBox)
	{
		return Create_Box(solid);
	}
	return NULL;
}
G4VSolid* 	ComplexObject::Create_Box(SolidParam solid)
{	
	G4double x,y,z; 
	if(solid.sParam.size() ==3)
	{
		x = solid.sParam[0];
		y = solid.sParam[1];
		z = solid.sParam[2];	
	}else { return NULL; }

	return new G4Box(solid.sName , x, y, z);
}
G4LogicalVolume*	ComplexObject::get_Logical_Volume()
{
	G4VSolid* pSolidObj1 = NULL;
	G4VSolid* pSolidObj2 = NULL;
	G4VSolid* pSolidObjBuf = NULL;
	std::string Name;
	for (int i=1; i<VectorOperation.size();i++)
	{
	
		pSolidObj1 = Create_solid(VectorOperation[i].oSolid);
		if (i==1)
			pSolidObj2 = Create_solid(VectorOperation[i-1].oSolid);
		G4ThreeVector oTransVector (VectorOperation[i-1].oTransVector.getG4ThreeVector()); 
		G4ThreeVector colX(VectorOperation[i-1].oRotMatrix.colX.getG4ThreeVector());
		G4ThreeVector colY(VectorOperation[i-1].oRotMatrix.colY.getG4ThreeVector());
		G4ThreeVector colZ(VectorOperation[i-1].oRotMatrix.colZ.getG4ThreeVector());		
		G4RotationMatrix 	oMatrix(colX,colY,colZ);
		Name.clear();
		Name = pSolidObj1->GetName() +'_'+ pSolidObj2->GetName();
		if (VectorOperation[i].oType == fUnionSolid)
		{
			 pSolidObjBuf = Union_solids(Name,pSolidObj1,pSolidObj2,oMatrix,oTransVector);	
		}
		if (VectorOperation[i].oType == fIntersectionSolid)
		{
			 pSolidObjBuf = Intersection_solids(Name,pSolidObj1,pSolidObj2,oMatrix,oTransVector);	
		}
		if (VectorOperation[i].oType == fSubtractionSolid)
		{
			 pSolidObjBuf = Subtraction_solids(Name,pSolidObj1,pSolidObj2,oMatrix,oTransVector);
			std::cout << "subsolid="<<pSolidObjBuf<<std::endl;
		}
		//delete pSolidObj1;
		//delete pSolidObj2;
		pSolidObj2 = pSolidObjBuf;
		pSolidObjBuf = NULL;
	}
	G4LogicalVolume* ptr = NULL; 
	std::cout << "ptr="<<ptr<<std::endl;
	ptr	=new G4LogicalVolume(pSolidObj2, MatObj.get_Material(NameMat),Name,0,0,0);
	std::cout << "ptr="<<ptr<<std::endl;
	return ptr;
}
void 	ComplexObject::set_solid(OperationOnSolids opSolid)
{
	VectorOperation.push_back(opSolid);
	save_xml();
}
