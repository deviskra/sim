#ifndef ComplexObject_H
#define ComplexObject_H 1
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#include <boost/config.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_serialize.hpp> 
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Para.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4EllipticalTube.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalCone.hh"
#include "G4Paraboloid.hh"
#include "G4Hype.hh"
#include "G4Tet.hh"
#include "G4ExtrudedSolid.hh"
#include "G4TwistedBox.hh"
#include "G4TwistedTrap.hh"
#include "G4TwistedTrd.hh"
#include "G4GenericTrap.hh"
#include "G4TwistedTubs.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4String.hh"
#include "globals.hh"
#include "G4VSolid.hh"
#include "Material.h"

enum BaseSolidType 
{
	fBox, fTubs, fCons, fPara, fTrd, fTrap, fSphere, 
	fOrb, fTorus, fPolycone, fPolyhedra, fEllipticalTube,
	fEllipsoid, fEllipticalCone, fParaboloid, fHype, fTet, 
	fExtrudedSolid, fTwistedBox, fTwistedTrap, fTwistedTrd, 
	fGenericTrap, fTwistedTubs
};
enum BaseOperationType {fUnionSolid,fIntersectionSolid, fSubtractionSolid};
//enum axis{fX,fY,fZ};
struct SolidParam 
{		
		
		friend class boost::serialization::access;
		BaseSolidType sType;
		std::string sName;
		std::vector<double> sParam;
	private: 
		template<class Archive>
        void serialize(Archive & ar,const unsigned int version) 
		{
			//ar & boost::serialization::base_object<G4String>(*this);
			ar & BOOST_SERIALIZATION_NVP(sType); 
			ar & BOOST_SERIALIZATION_NVP(sName); 
			ar & BOOST_SERIALIZATION_NVP(sParam); 
		}
};
class Vector3d
{	
	friend class boost::serialization::access;
	public:
		Vector3d(double x_=0.0, double y_=0.0, double z_= 0.0)
		{
			x=x_;
			y=y_;
			z=z_;
		}
		Vector3d(const Vector3d & vec)
		{
			x=vec.x;
			y=vec.y;
			z=vec.z;
		}
		Vector3d(G4ThreeVector vec)
		{
			x=vec.x();
			y=vec.y();
			z=vec.z();
		}
		void set(double x_, double y_, double z_)
		{
			x=x_;
			y=y_;
			z=z_;
		}
		G4ThreeVector	getG4ThreeVector()
		{
			return G4ThreeVector(x,y,z);
		}

		double x;
		double y;
		double z;
	private:
		template<class Archive>
		void serialize(Archive & ar,const unsigned int version) 
		{	
			ar & BOOST_SERIALIZATION_NVP(x); 
			ar & BOOST_SERIALIZATION_NVP(y); 
			ar & BOOST_SERIALIZATION_NVP(z);
		}
};
struct RotMatrixSz
{
		RotMatrixSz()
		{
			colX.x = 1;
			colY.y = 1;
			colZ.z = 1;
		}
		friend class boost::serialization::access;
		Vector3d colX;
		Vector3d colY;
		Vector3d colZ;
	private:
		template<class Archive>
		void serialize(Archive & ar,const unsigned int version) 
		{	
			ar & BOOST_SERIALIZATION_NVP(colX); 
			ar & BOOST_SERIALIZATION_NVP(colY);
			ar & BOOST_SERIALIZATION_NVP(colZ);
		}
};
struct OperationOnSolids 
{	
		friend class boost::serialization::access;
		BaseOperationType oType;
		SolidParam oSolid;
		RotMatrixSz oRotMatrix;
		Vector3d oTransVector; 
	private: 
		template<class Archive>
        void serialize(Archive & ar,const unsigned int version) 
		{
			//ar & boost::serialization::base_object<G4ThreeVector>(*this);
			ar & BOOST_SERIALIZATION_NVP(oType); 
			ar & BOOST_SERIALIZATION_NVP(oSolid); 
			ar & BOOST_SERIALIZATION_NVP(oRotMatrix); 
			ar & BOOST_SERIALIZATION_NVP(oTransVector);
		}
};

class ComplexObject
{
		friend class boost::serialization::access;
	public:
		ComplexObject();
		ComplexObject(std::string pathfilename_);
		~ComplexObject();
	private:
		template<class Archive>
        void save(Archive & ar,const unsigned int version) const;
		template<class Archive>
		void load(Archive & ar,const unsigned int version);
		
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		void save_xml();
		void load_xml();

		G4VSolid*	Union_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector);
		G4VSolid*	Intersection_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector);
		G4VSolid*	Subtraction_solids(std::string Name, G4VSolid* pSolidObj1, G4VSolid* pSolidObj2, G4RotationMatrix oMatrix,G4ThreeVector oVector);
		G4VSolid* 	Create_solid(SolidParam);
		G4VSolid* 	Create_Box(SolidParam);
		
		std::vector<SolidParam>		VectorSolidParam;
		std::vector<OperationOnSolids>		VectorOperation;
		std::string 	xmlFileName;
		std::string 	NameMat;
		std::string 	NameCompObj;
		Material		MatObj;
	public:
		G4LogicalVolume*	get_Logical_Volume();
		void 	set_solid(OperationOnSolids);
		void 	set_material(std::string mat_)
		{
			NameMat=mat_;
			save_xml();
		}

};



#endif
