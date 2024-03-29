/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                Copyright (C) 2023 Oak Ridge National Laboratory                
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
    Foam::heatSourceModels::heatSourceModel

Description
    Abstract base class for heatSource models

SourceFiles
    heatSourceModel.C

\*---------------------------------------------------------------------------*/

#ifndef heatSourceModel_H
#define heatSourceModel_H

#include "dictionary.H"
#include "volFields.H"
#include "movingBeam.H"
#include "absorptionModel.H"
#include "labelVector.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class heatSourceModel Declaration
\*---------------------------------------------------------------------------*/

class heatSourceModel
:
    public IOdictionary
{
    // Private member functions
    
        //- Construct the base IO object
        IOobject createIOobject
        (
            const dictionary& dict,
            const fvMesh& mesh
        ) const;
    
protected:

    // Protected data
        
        //- Name of heat source
        const word& sourceName_;
        
        //- Main heat source dictionary
        const dictionary& heatSourceDict_;
        
        //- Sub-dict for individual heat source
        dictionary sourceDict_;
        
        //- Heat source model coefficient sub-dict
        dictionary heatSourceModelCoeffs_;
        
        //- Pointer to mesh info
        const fvMesh& mesh_;

        //- Flag for transient or static heat source dimensions
        Switch transient_;

        //- Isotherm depth tracked in transient heat source model
        scalar isoValue_;

        //- Dimensions of the heat source (dynamic)
        vector dimensions_;

        //- Dimensions of the heat source (static)
        vector staticDimensions_;

        //- Number of points along heat source dimensions resolved by sampling
        labelVector nPoints_;

        //- AutoPtrs to absorption model and moving beam instances
        autoPtr<absorptionModel> absorptionModel_;
        autoPtr<movingBeam> movingBeam_;

public:

    //- Runtime type information
    TypeName("heatSourceModel");
    
    //- Default heatSourceProperties dictionary name
    static const word heatSourceDictName;
    
    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        heatSourceModel,
        dictionary,
        (
            const word& sourceName,
            const dictionary& dict,
            const fvMesh& mesh
        ),
        (sourceName, dict, mesh)
    );


    // Constructors

        //- Construct for mixture
        heatSourceModel
        (
            const word& type,
            const word& sourceName,
            const dictionary& dict,
            const fvMesh& mesh
        );
        
        //- Disallow default bitwise copy construction
        heatSourceModel(const heatSourceModel&);
        
    // Selectors

        //- Return a reference to the selected heatSource model
        static autoPtr<heatSourceModel> New
        (
            const word& sourceName,
            const dictionary& dict,
            const fvMesh& mesh
        );

    //- Destructor
    virtual ~heatSourceModel()
    {}


    // Member Functions
    
        //- Return access to the movingBeam instance
        movingBeam& beam()
        {
            return movingBeam_();
        }

        //- Return boolean of heat source type
        Switch transient()
        {
            return transient_;
        }

        //- Return scalar of the isovalue
        scalar isoValue()
        {
            return isoValue_;
        }

        //- Return vector of current heat source dimensions
        vector dimensions()
        {
            return dimensions_;
        }

        //- Return vector of original heat source dimensions
        vector staticDimensions()
        {
            return staticDimensions_;
        }

        //- Update the transient heat source dimensions
        void updateDimensions();

        //- Return the volumetric heating field
        virtual tmp<volScalarField> qDot();

        //- Return the weight of the heat source distribution at a given point
        virtual scalar weight(const vector& d) = 0;

        //- Return the normalization volume for the integrated distribution
        virtual dimensionedScalar V0() = 0;

        //- Read the heat source dictionary
        virtual bool read() = 0;
        
        
    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const heatSourceModel&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
