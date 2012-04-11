#include "Material.h"

Material::Material()
{
  G4double a;  // atomic mass
  G4double Z;  // atomic number
  G4double density;
  G4Element* Ar = new G4Element("Argon", "Ar", Z=18., a=34.94*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", Z=1., a=1.0079*g/mole);
  G4Element* C = new G4Element("Carbon", "C", Z=6., a=12.011*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.00674*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", 8., 16.00000*g/mole);
  G4Element* Na = new G4Element("Natrium", "Na", Z=11., a=22.98977*g/mole);
  G4Element* Mg = new G4Element("Magnezium", "Mg", Z=12., a=24.305*g/mole);
  G4Element* Al = new G4Element("Aluminium", "Al", Z=13., a=26.981*g/mole);
  G4Element* Si = new G4Element("Silicium", "Si", Z=14., a=28.086*g/mole);
  G4Element* P = new G4Element("Phosphorus", "P", Z=15., a=30.973976*g/mole);
  G4Element* S = new G4Element("Sulfur", "S", Z=16., a=32.06*g/mole);
  G4Element* Cl = new G4Element("Chlorine","Cl", Z=17., a=35.453*g/mole);
  G4Element* K = new G4Element("Kalium", "K", Z=19., a=39.098*g/mole);
  G4Element* Ca = new G4Element("Calcium", "Ca", Z=20., a=40.08*g/mole);
  G4Element* Ti = new G4Element("Titanium", "Ti", Z=22., a=47.9*g/mole);
  G4Element* Cr = new G4Element("Chrome", "Cr", Z=24., a=51.996*g/mole);
  G4Element* Mn = new G4Element("Manganeze", "Mn", Z=25., a=54.938*g/mole);
  G4Element* Fe = new G4Element("Ferrum", "Fe", Z=26., a=55.847*g/mole);
  G4Element* Ni = new G4Element("Nickel", "Ni", Z=28., a=58.7*g/mole);
  G4Element* Cu = new G4Element("Cuprum", "Cu", Z=29., a=63.546*g/mole);
  G4Element* Zn = new G4Element("Zyncum", "Zn", Z=30., a=65.38*g/mole);
  G4Element* Ge = new G4Element("Germanium", "Ge", Z=32., a=72.59*g/mole);
  G4Element* Ag = new G4Element("Argentum", "Ag", Z=47., a=107.8682*g/mole);
  G4Element* I = new G4Element("Iodine", "I", Z=53., a=126.904*g/mole);
  G4Element* Cs = new G4Element("Cesium", "Cs", Z=55., a=132.905*g/mole);
  G4Element* Ba = new G4Element("Barium", "Ba", Z=56., a=133.*g/mole);
  G4Element* W = new G4Element("Wolfram", "W", Z=74., a=183.85*g/mole);
  G4Element* Pt = new G4Element("Platinum", "Pt", Z=78., a=195.08*g/mole);
  G4Element* Au = new G4Element("Aurum", "Au", Z=79., a=196.9665*g/mole);
  G4Element* Pb = new G4Element("Plumbum", "Pb", Z=82., a=207.2*g/mole);
  G4Element* Bi = new G4Element("Bismuth", "Bi", Z=83., a=208.9804*g/mole);
  G4Element* As = new G4Element("Arsenic", "As", Z=33., a=74.9216*g/mole);	
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


//  G4Material* Ar = 
 // new G4Material("ArgonGas", z= 18., a= 39.95*g/mole, density= 1.782*mg/cm3);

  // G4Material* Al = 
   // new G4Material("Aluminum", z= 13., a= 26.98*g/mole, density= 2.7*g/cm3);

   // G4Material* Pb = 
  // new G4Material("Lead", z= 82., a= 207.19*g/mole, density= 11.35*g/cm3);

  

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
G4Material*	NaI = new G4Material(                         
				"NaI",
				3.67*g/cm3,
				2);
	NaI->AddElement(Na,    1);
	NaI->AddElement(I,    1);

	MaterialBase["NaI"] = NaI;

//---------------------------------------------------


G4Material*	MgO = new G4Material(                         
				"MgO",
				1.754*g/cm3,
				2);
        MgO ->AddElement(Mg,   1);
	MgO ->AddElement(O,    1); 

	MaterialBase["MgO"] = MgO;

//-------------------------------------------------------

G4Material*	Fe_met = new G4Material(                         
				"Fe_mat",
				7.7*g/cm3,
				1);
        Fe_met ->AddElement(Fe,   1);

	MaterialBase["Fe"] = Fe_met;
	
//-----------------------------

G4Material*	Al_met = new G4Material(                         
				"Al_met",
				2.7*g/cm3,
				1);
        Al_met ->AddElement(Al,   1);

	MaterialBase["Al"] = Al_met;
	
//------------------------------

G4Material*     Pb_met = new G4Material( 
                                "Pb_met",
                                 11.3415*g/cm3,
                                 1);         
         Pb_met ->AddElement(Pb,  1);
        MaterialBase["Pb"] = Pb_met;

G4Material*     Ag_met = new G4Material( 
                                "Ag_met",
                                 10.5*g/cm3,
                                 1);         
         Ag_met ->AddElement(Ag,  1);
         MaterialBase["Ag"] = Ag_met;
	

G4Material*     Pt_met = new G4Material("Pt_met",21.45*g/cm3, 1);         
         Pt_met ->AddElement(Pt,  1);
         MaterialBase["Pt"] = Pt_met;
	

//-------------------------------

G4Material*	K8 = new G4Material(
				"K8",
				2.6*g/cm3,
				7);
	K8 ->AddElement(O, 0.598);
	K8 ->AddElement(Na, 0.103);
	K8 ->AddElement(Al, 0.014);
	K8 ->AddElement(Mg, 0.014);
	K8 ->AddElement(Fe, 0.001);
	K8 ->AddElement(Si, 0.247);
	K8 ->AddElement(Ca, 0.023);

	MaterialBase["K8"] = K8;

//-------------------------------------------------------

G4Material* 	D16 = new G4Material("D16", 2.8*g/cm3, 9);


	D16 ->AddElement(Al, 0.925);
	D16 ->AddElement(Cu, 0.04); 
	D16 ->AddElement(Mg, 0.015);
	D16 ->AddElement(Mn, 0.006); 
	D16 ->AddElement(Si, 0.005); 
	D16 ->AddElement(Fe, 0.004); 
	D16 ->AddElement(Zn, 0.003); 
	D16 ->AddElement(Ti, 0.001); 
	D16 ->AddElement(Ni, 0.001);

	MaterialBase["D16"] = D16;
//----------------------------------------------------------------

G4Material*	Air = new G4Material("Air", 0.0012041*g/cm3, 3);


	Air ->AddElement(N, 0.7804);
	Air ->AddElement(O, 0.2103);
	Air ->AddElement(Ar, 0.0093);

	MaterialBase["Air"] = Air;


G4Material*	wood = new G4Material("wood", 0.7*g/cm3, 4);


	wood ->AddElement(N, 0.0012);
	wood ->AddElement(C, 0.495);
	wood ->AddElement(O, 0.4408);
	wood ->AddElement(H, 0.063);
	MaterialBase["wood"] = wood;


G4Material*	cocaine = new G4Material("cocaine", 1.22*g/cm3, 4);


	cocaine ->AddElement(N, 0.046172301);
	cocaine ->AddElement(C, 0.673090996);
	cocaine ->AddElement(O, 0.2109645);
	cocaine ->AddElement(H, 0.069772203);
	MaterialBase["cocaine"] = cocaine;


G4Material*	concrete = new G4Material("concrete", 2.3*g/cm3, 10);

	concrete ->AddElement(C, 0.001);
	concrete ->AddElement(O, 0.529);
	concrete ->AddElement(H, 0.01);
	concrete ->AddElement(Si, 0.337);
	concrete ->AddElement(Al, 0.034);
	concrete ->AddElement(Fe, 0.014);
	concrete ->AddElement(Ca, 0.044);
	concrete ->AddElement(Mg, 0.002);
	concrete ->AddElement(K, 0.013);
	concrete ->AddElement(Na, 0.016);
	MaterialBase["concrete"] = concrete;


G4Material*	brick = new G4Material("brick", 1.5*g/cm3, 5);

	brick ->AddElement(Ca, 0.03395);
	brick ->AddElement(O, 0.53116);
	brick ->AddElement(H, 0.00336);
	brick ->AddElement(Si, 0.42996);
	brick ->AddElement(Mg, 0.00157);
	MaterialBase["brick"] = brick;
	

G4Material*	steel_45 = new G4Material("steel_45", 7.8*g/cm3, 10);

	steel_45 ->AddElement(C, 0.0045);
	steel_45 ->AddElement(Si, 0.002);
	steel_45 ->AddElement(As, 0.0008);
	steel_45 ->AddElement(Fe, 0.97845);
	steel_45 ->AddElement(Cu, 0.0025);
	steel_45 ->AddElement(Mn, 0.006);
	steel_45 ->AddElement(Ni, 0.0025);
	steel_45 ->AddElement(Cr, 0.0025);
	steel_45 ->AddElement(P, 0.00035);
	steel_45 ->AddElement(S, 0.0004);
	MaterialBase["steel_45"] = steel_45;
	
//-----------------------------------------------------------------
G4Material*     Au_met = new G4Material( 
                                "Au_met",
                                 19.32*g/cm3,
                                 1);         
         Au_met ->AddElement(Au,  1);
         MaterialBase["Au"] = Au_met;

//----------------------------------------------------------------
//Полипропилен. 

G4Material*	PP = new G4Material("PP", 0.91*g/cm3, 2);


	PP ->AddElement(C, 1);
	PP ->AddElement(H, 2);
	

	MaterialBase["PP"] = PP;
	


//-----------------------------------------------------------------
G4Material*	Vac = new G4Material("Vac", 1.0E-20*g/cm3, 1);


	Vac ->AddElement(H, 1.0);
	
	MaterialBase["Vac"] = Vac;



}

Material::~Material()
{

}
G4Material*	Material::get_Material(std::string Name)
{
	//G4Material	outMaterial;
	//MaterialBase.find(Name);
	return (*MaterialBase.find(Name)).second;
}



void	Material::set_Material(std::string Name, G4Material* material)
{


}	