//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Previous authors: G. Guerrieri, S. Guatelli and M. G. Pia, INFN Genova, Italy
// Authors (since 2007): S. Guatelli, University of Wollongong, Australia
// 
//
#include "G4MIRDUterus.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Ellipsoid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4HumanPhantomColour.hh"

#include "G4SubtractionSolid.hh"

G4MIRDUterus::G4MIRDUterus()
{
}

G4MIRDUterus::~G4MIRDUterus()
{
}


G4VPhysicalVolume* G4MIRDUterus::Construct(const G4String& volumeName,G4VPhysicalVolume* mother,  
					   const G4String& colourName,G4bool wireFrame, G4bool)
{
 
  G4cout<<"Construct "<<volumeName<<" with mother volume "<<mother->GetName()<<G4endl;
 
  G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
  G4Material* soft = material -> GetMaterial("soft_tissue");
  delete material;

  //Uterus
  G4double ax= 6*cm; // Largo hombro a hombro
  G4double by= 6*cm; //Largo pies a cabeza
  G4double cz= 8.*cm; //Ancho espalda a pecho 
  G4double zcut1= -8.*cm; // Corte superior
  G4double zcut2= 5*cm; // Corte inferior


  G4Ellipsoid* uterus = new G4Ellipsoid("Uterus",
					ax, by, cz,
					zcut1, zcut2);

  //Fetus
  G4double radio_max = 1.8*cm;
  G4double dz = 5.*cm;
  G4VSolid* fetus =new G4Tubs("Fetus",0,radio_max,dz,0*deg,360*deg);

  G4VSolid* Uterus1 = new G4SubtractionSolid("Uterus1", uterus, fetus);


  G4LogicalVolume* logicUterus = new G4LogicalVolume(Uterus1,
						     soft,
						     "logical" + volumeName,
  						     0, 0, 0);


  // Define rotation and position here!
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm->rotateX(120.*degree); // 90 grados elipsoide en dirección Z
  //<90 grados cabeza del elipsoide hacia los pies. 0 grados apuntando a los pies
  //>90 grados elipsoide apuntando al torso

  G4VPhysicalVolume* physUterus = new G4PVPlacement(rm,
						    G4ThreeVector(0. *cm, -2.4*cm, -21 *cm),
						    "physicalUterus", // Componente 1 positivo a la derecha. 0 en el origen
						                      // Componente 2 positivo hacia abajo
						                      // Componente 3 positivo hacia los pies
						    logicUterus,
						    mother,
						    false,
						    0, true);



  // Visualization Attributes

  //G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  //G4Colour colour = colourPointer -> GetColour(colourName);
  //G4VisAttributes* UterusVisAtt = new G4VisAttributes(colour);
  
  G4VisAttributes* UterusVisAtt = new G4VisAttributes(G4Colour(0.85,0.44,0.84,0.5));
  UterusVisAtt->SetForceSolid(wireFrame);
  logicUterus->SetVisAttributes(UterusVisAtt);

  G4cout << "Uterus created !!!!!!" << G4endl;

  // Testing Uterus Volume
  G4double UterusVol = logicUterus->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Uterus = " << UterusVol/cm3 << " cm^3" << G4endl;
  
  // Testing Uterus Material
  G4String UterusMat = logicUterus->GetMaterial()->GetName();
  G4cout << "Material of Uterus = " << UterusMat << G4endl;
  
  // Testing Density
  G4double UterusDensity = logicUterus->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << UterusDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double UterusMass = (UterusVol)*UterusDensity;
  G4cout << "Mass of Uterus = " << UterusMass/gram << " g" << G4endl;  
  
  return physUterus;
}






 

  

  
