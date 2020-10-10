#include "G4GDMLReadStructure.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4GDMLReadSolids::SolidsRead(const xercesc::DOMElement *const solidsElement)
{
    //G4cout << "G4GDML: Reading solsffids..." << G4endl;

    for (xercesc::DOMNode *iter = solidsElement->getFirstChild();
         iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
            continue;

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        if (!child)
        {
            G4Exception("G4GDMLReadSolids::SolidsRead()",
                        "InvalidRead", FatalException, "No child found!");
            return;
        }
        const G4String tag = Transcode(child->getTagName());
        if (tag == "define")
            DefineRead(child);
        else if (tag == "box")
            BoxRead(child);
        else if (tag == "cone")
            ConeRead(child);
        else if (tag == "elcone")
            ElconeRead(child);
        else if (tag == "ellipsoid")
            EllipsoidRead(child);
        else if (tag == "eltube")
            EltubeRead(child);
        else if (tag == "xtru")
            XtruRead(child);
        else if (tag == "hype")
            HypeRead(child);
        else if (tag == "intersection")
            BooleanRead(child, INTERSECTION);
        else if (tag == "multiUnion")
            MultiUnionRead(child);
        else if (tag == "orb")
            OrbRead(child);
        else if (tag == "para")
            ParaRead(child);
        else if (tag == "paraboloid")
            ParaboloidRead(child);
        else if (tag == "polycone")
            PolyconeRead(child);
        else if (tag == "genericPolycone")
            GenericPolyconeRead(child);
        else if (tag == "polyhedra")
            PolyhedraRead(child);
        else if (tag == "genericPolyhedra")
            GenericPolyhedraRead(child);
        else if (tag == "reflectedSolid")
            ReflectedSolidRead(child);
        else if (tag == "sphere")
            SphereRead(child);
        else if (tag == "subtraction")
            BooleanRead(child, SUBTRACTION);
        else if (tag == "tessellated")
            TessellatedRead(child);
        else if (tag == "tet")
            TetRead(child);
        else if (tag == "torus")
            TorusRead(child);
        else if (tag == "arb8")
            GenTrapRead(child);
        else if (tag == "trap")
            TrapRead(child);
        else if (tag == "trd")
            TrdRead(child);
        else if (tag == "tube")
            TubeRead(child);
        else if (tag == "cutTube")
            CutTubeRead(child);
        else if (tag == "twistedbox")
            TwistedboxRead(child);
        else if (tag == "twistedtrap")
            TwistedtrapRead(child);
        else if (tag == "twistedtrd")
            TwistedtrdRead(child);
        else if (tag == "twistedtubs")
            TwistedtubsRead(child);
        else if (tag == "union")
            BooleanRead(child, UNION);
        else if (tag == "opticalsurface")
            OpticalSurfaceRead(child);
        else if (tag == "loop")
            LoopRead(child, &G4GDMLRead::SolidsRead);
        else
        {
            G4String error_msg = "Unknown tag in solids: " + tag;
            G4Exception("G4GDMLReadSolids::SolidsRead()", "ReadError",
                        FatalException, error_msg);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4GDMLReadSolids::
    OpticalSurfaceRead(const xercesc::DOMElement *const opticalsurfaceElement)
{
    G4String name;
    G4String smodel;
    G4String sfinish;
    G4String stype;
    G4double value = 0.0;

    G4MaterialPropertiesTable *OpSurfaceProperty = new G4MaterialPropertiesTable();

    const xercesc::DOMNamedNodeMap *const attributes = opticalsurfaceElement->getAttributes();
    XMLSize_t attributeCount = attributes->getLength();

    for (XMLSize_t attribute_index = 0;
         attribute_index < attributeCount; attribute_index++)
    {
        xercesc::DOMNode *attribute_node = attributes->item(attribute_index);

        if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
            continue;

        const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
        if (!attribute)
        {
            G4Exception("G4GDMLReadSolids::OpticalSurfaceRead()",
                        "InvalidRead", FatalException, "No attribute found!");
            return;
        }
        const G4String attName = Transcode(attribute->getName());
        const G4String attValue = Transcode(attribute->getValue());

        if (attName == "name")
            name = GenerateName(attValue);
        else if (attName == "model")
            smodel = attValue;
        else if (attName == "finish")
            sfinish = attValue;
        else if (attName == "type")
            stype = attValue;
        else if (attName == "value")
            value = eval.Evaluate(attValue);
    }

    G4OpticalSurfaceModel model;
    G4OpticalSurfaceFinish finish;
    G4SurfaceType type;
    if ((smodel == "glisur") || (smodel == "0"))
        model = glisur;
    else if ((smodel == "unified") || (smodel == "1"))
        model = unified;
    else if ((smodel == "LUT") || (smodel == "2"))
        model = LUT;
    else
        model = dichroic;

    if ((sfinish == "polished") || (sfinish == "0"))
        finish = polished;
    else if ((sfinish == "polishedfrontpainted") || (sfinish == "1"))
        finish = polishedfrontpainted;
    else if ((sfinish == "polishedbackpainted") || (sfinish == "2"))
        finish = polishedbackpainted;
    else if ((sfinish == "ground") || (sfinish == "3"))
        finish = ground;
    else if ((sfinish == "groundfrontpainted") || (sfinish == "4"))
        finish = groundfrontpainted;
    else if ((sfinish == "groundbackpainted") || (sfinish == "5"))
        finish = groundbackpainted;
    else if ((sfinish == "polishedlumirrorair") || (sfinish == "6"))
        finish = polishedlumirrorair;
    else if ((sfinish == "polishedlumirrorglue") || (sfinish == "7"))
        finish = polishedlumirrorglue;
    else if ((sfinish == "polishedair") || (sfinish == "8"))
        finish = polishedair;
    else if ((sfinish == "polishedteflonair") || (sfinish == "9"))
        finish = polishedteflonair;
    else if ((sfinish == "polishedtioair") || (sfinish == "10"))
        finish = polishedtioair;
    else if ((sfinish == "polishedtyvekair") || (sfinish == "11"))
        finish = polishedtyvekair;
    else if ((sfinish == "polishedvm2000air") || (sfinish == "12"))
        finish = polishedvm2000air;
    else if ((sfinish == "polishedvm2000glue") || (sfinish == "13"))
        finish = polishedvm2000glue;
    else if ((sfinish == "etchedlumirrorair") || (sfinish == "14"))
        finish = etchedlumirrorair;
    else if ((sfinish == "etchedlumirrorglue") || (sfinish == "15"))
        finish = etchedlumirrorglue;
    else if ((sfinish == "etchedair") || (sfinish == "16"))
        finish = etchedair;
    else if ((sfinish == "etchedteflonair") || (sfinish == "17"))
        finish = etchedteflonair;
    else if ((sfinish == "etchedtioair") || (sfinish == "18"))
        finish = etchedtioair;
    else if ((sfinish == "etchedtyvekair") || (sfinish == "19"))
        finish = etchedtyvekair;
    else if ((sfinish == "etchedvm2000air") || (sfinish == "20"))
        finish = etchedvm2000air;
    else if ((sfinish == "etchedvm2000glue") || (sfinish == "21"))
        finish = etchedvm2000glue;
    else if ((sfinish == "groundlumirrorair") || (sfinish == "22"))
        finish = groundlumirrorair;
    else if ((sfinish == "groundlumirrorglue") || (sfinish == "23"))
        finish = groundlumirrorglue;
    else if ((sfinish == "groundair") || (sfinish == "24"))
        finish = groundair;
    else if ((sfinish == "groundteflonair") || (sfinish == "25"))
        finish = groundteflonair;
    else if ((sfinish == "groundtioair") || (sfinish == "26"))
        finish = groundtioair;
    else if ((sfinish == "groundtyvekair") || (sfinish == "27"))
        finish = groundtyvekair;
    else if ((sfinish == "groundvm2000air") || (sfinish == "28"))
        finish = groundvm2000air;
    else
        finish = groundvm2000glue;

    if ((stype == "dielectric_metal") || (stype == "0"))
        type = dielectric_metal;
    else if ((stype == "dielectric_dielectric") || (stype == "1"))
        type = dielectric_dielectric;
    else if ((stype == "dielectric_LUT") || (stype == "2"))
        type = dielectric_LUT;
    else if ((stype == "dielectric_dichroic") || (stype == "3"))
        type = dielectric_dichroic;
    else if ((stype == "firsov") || (stype == "4"))
        type = firsov;
    else
        type = x_ray;

    for (xercesc::DOMNode *iter = opticalsurfaceElement->getFirstChild(); iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
        {
            continue;
        }

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        if (!child)
        {
            G4Exception("G4GDMLReadSolids::OpticalSurfaceRead()", "InvalidRead",
                        FatalException, "No child found!");
            return;
        }
        const G4String tag = Transcode(child->getTagName());

        if (tag == "property")
        {
            G4String opName;
            G4String ref;
            G4GDMLMatrix matrix;

            const xercesc::DOMNamedNodeMap *const opAttributes = child->getAttributes();
            XMLSize_t opAttributeCount = opAttributes->getLength();

            for (XMLSize_t attribute_index = 0;
                 attribute_index < opAttributeCount; attribute_index++)
            {
                xercesc::DOMNode *attribute_node = opAttributes->item(attribute_index);

                if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
                    continue;

                const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
                if (!attribute)
                {
                    G4Exception("OpticalSurfaceRead::PropertyRead()", "InvalidRead",
                                FatalException, "No attribute found!");
                    return;
                }
                const G4String attName = Transcode(attribute->getName());
                const G4String attValue = Transcode(attribute->getValue());

                if (attName == "name")
                {
                    opName = GenerateName(attValue);
                }
                else if (attName == "ref")
                {
                    matrix = GetMatrix(ref = attValue);
                }
            }

            if (matrix.GetRows() == 0)
                return;
            if (matrix.GetCols() == 1) // constant property assumed
            {
                OpSurfaceProperty->AddConstProperty(Strip(opName), matrix.Get(0, 0));
            }
            else // build the material properties vector
            {
                G4MaterialPropertyVector *propvect = new G4MaterialPropertyVector();
                for (size_t i = 0; i < matrix.GetRows(); i++)
                {
                    propvect->InsertValues(matrix.Get(i, 0), matrix.Get(i, 1));
                }
                OpSurfaceProperty->AddProperty(Strip(opName), propvect);
            }
        }
    }

    G4OpticalSurface *OpSurface = new G4OpticalSurface(name, model, finish, type, value);
    OpSurface->SetMaterialPropertiesTable(OpSurfaceProperty);
}
