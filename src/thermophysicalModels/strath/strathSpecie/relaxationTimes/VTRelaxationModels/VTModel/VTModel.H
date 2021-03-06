/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016-2021 hyStrath
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of hyStrath, a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::VTModel

Description
    V-T relaxation model

SourceFiles
    VTModel.C

\*---------------------------------------------------------------------------*/

#ifndef VTModel_H
#define VTModel_H

#include "VTRelaxationModel.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                          Class VTModel Declaration
\*---------------------------------------------------------------------------*/

class VTModel
{

protected:

    // Protected data

        //- Two-temperature model dictionary
        const IOdictionary dict2T_;

        //- Thermophysical properties dictionary
        const IOdictionary dictThermoPhy_;

        //- List of molecules
        const wordList& molecules_;

        //- List of species
        const wordList& species_;

        //- Pressure [Pa]
        const volScalarField& p_;

        //- Trans-rotational temperature [K]
        const volScalarField& T_;

        //- Vibro-electronic temperatures [K]
        const PtrList<volScalarField>& Tv_;

        //- Species number density [m^-3]
        const PtrList<volScalarField>& nD_;

        //- V-T relaxation models
        PtrList<VTRelaxationModel> tauVTijModels_;

        //- Interspecies V-T relaxation time [s]
        PtrList<volScalarField> tauVTij_;
        
        //- Correction to the V-T relaxation time [s]
        PtrList<volScalarField> tauVTcorr_;


    // Private Member Functions

        //- Disallow copy construct
        VTModel(const VTModel&);

        //- Disallow default bitwise assignment
        void operator=(const VTModel&);


public:

    //- Runtime type information
    TypeName("VTModel");

    // Constructors

        //- Construct from components
        VTModel
        (
            const word& dict2T,
            const word& dictThermoPhy,
            const wordList& molecules,
            const wordList& species,
            const volScalarField& p,
            const volScalarField& T,
            const PtrList<volScalarField>& Tv,
            const PtrList<volScalarField>& nD
        );


    // Destructor

        virtual ~VTModel()
        {}


    // Member Functions

        //- Return const access to the interspecies V-T relaxation time
        inline const volScalarField& tauVT(label i, label j) const
        {
            label k = 0;
            
            if (i > j)
            {
                k = species_.size()*j + i;
            }
            else
            {
                k = species_.size()*i + j;
            }
            
            return tauVTij_[k];
        }
        
        //- Return const access to the V-T relaxation time correction term
        inline const volScalarField& tauVTcorr(label moli) const
        {
            return tauVTcorr_[moli];
        }

        //- Update the interspecies V-T relaxation times
        virtual void update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
