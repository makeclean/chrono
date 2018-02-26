// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Track assembly (band-ANCF) model constructed from a JSON specification file
//
// =============================================================================

#include "chrono_vehicle/tracked_vehicle/track_assembly/TrackAssemblyBandANCF.h"

#include "chrono_vehicle/tracked_vehicle/sprocket/SprocketBand.h"
#include "chrono_vehicle/tracked_vehicle/track_shoe/TrackShoeBandANCF.h"

#include "chrono_vehicle/tracked_vehicle/brake/TrackBrakeSimple.h"

#include "chrono_vehicle/tracked_vehicle/idler/DoubleIdler.h"
#include "chrono_vehicle/tracked_vehicle/idler/SingleIdler.h"

#include "chrono_vehicle/tracked_vehicle/suspension/LinearDamperRWAssembly.h"

#include "chrono_vehicle/tracked_vehicle/roller/DoubleRoller.h"

#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/utils/ChUtilsJSON.h"

#include "chrono_thirdparty/rapidjson/document.h"
#include "chrono_thirdparty/rapidjson/filereadstream.h"

using namespace rapidjson;

namespace chrono {
namespace vehicle {

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadSprocket(const std::string& filename, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is a sprocket specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("Sprocket") == 0);

    // Check sprocket type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();
    assert(subtype.compare("SprocketBand") == 0);

    // Create the sprocket using the appropriate template.
    m_sprocket = std::make_shared<SprocketBand>(d);

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_sprocket->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadBrake(const std::string& filename, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is a brake specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("TrackBrake") == 0);

    // Extract brake type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();

    // Create the brake using the appropriate template.
    if (subtype.compare("TrackBrakeSimple") == 0) {
        m_brake = std::make_shared<TrackBrakeSimple>(d);
    }

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_brake->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadIdler(const std::string& filename, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is an idler specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("Idler") == 0);

    // Extract idler type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();

    // Create the idler using the appropriate template.
    if (subtype.compare("SingleIdler") == 0) {
        m_idler = std::make_shared<SingleIdler>(d);
    } else if (subtype.compare("DoubleIdler") == 0) {
        m_idler = std::make_shared<DoubleIdler>(d);
    }

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_idler->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadSuspension(const std::string& filename, int which, bool has_shock, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is a road-wheel assembly specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("RoadWheelAssembly") == 0);

    // Extract road-wheel assembly type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();

    // Create the road-wheel assembly using the appropriate template.
    if (subtype.compare("LinearDamperRWAssembly") == 0) {
        m_suspensions[which] = std::make_shared<LinearDamperRWAssembly>(d, has_shock);
    }

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_suspensions[which]->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadRoller(const std::string& filename, int which, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is a roller specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("Roller") == 0);

    // Extract roller type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();

    // Create the roller using the appropriate template.
    if (subtype.compare("DoubleRoller") == 0) {
        m_rollers[which] = std::make_shared<DoubleRoller>(d);
    }

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_rollers[which]->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TrackAssemblyBandANCF::LoadTrackShoes(const std::string& filename, int num_shoes, int output) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    // Check that the given file is a track shoe specification file.
    assert(d.HasMember("Type"));
    std::string type = d["Type"].GetString();
    assert(type.compare("TrackShoe") == 0);

    // Check track shoe type.
    assert(d.HasMember("Template"));
    std::string subtype = d["Template"].GetString();
    assert(subtype.compare("TrackShoeBandANCF") == 0);

    // Create the track shoes using the appropriate template.
    for (size_t it = 0; it < num_shoes; it++) {
        m_shoes.push_back(std::make_shared<TrackShoeBandANCF>(d));
    }

    // A non-zero value of 'output' indicates overwriting the subsystem's flag
    if (output != 0) {
        m_shoes[0]->SetOutput(output == +1);
    }

    GetLog() << "  Loaded JSON: " << filename.c_str() << "\n";
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
TrackAssemblyBandANCF::TrackAssemblyBandANCF(const std::string& filename) : ChTrackAssemblyBandANCF("", LEFT) {
    FILE* fp = fopen(filename.c_str(), "r");

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    fclose(fp);

    Document d;
    d.ParseStream<ParseFlag::kParseCommentsFlag>(is);

    Create(d);

    GetLog() << "Loaded JSON: " << filename.c_str() << "\n";
}

TrackAssemblyBandANCF::TrackAssemblyBandANCF(const rapidjson::Document& d) : ChTrackAssemblyBandANCF("", LEFT) {
    Create(d);
}

void TrackAssemblyBandANCF::Create(const rapidjson::Document& d) {
    // Invoke base class method.
    ChPart::Create(d);

    // Create the sprocket
    {
        assert(d.HasMember("Sprocket"));
        std::string file_name = d["Sprocket"]["Input File"].GetString();
        int output = 0;
        if (d["Sprocket"].HasMember("Output")) {
            output = d["Sprocket"]["Output"].GetBool() ? +1 : -1;
        }
        LoadSprocket(vehicle::GetDataFile(file_name), output);
        m_sprocket_loc = LoadVectorJSON(d["Sprocket"]["Location"]);
    }

    // Create the brake
    {
        assert(d.HasMember("Brake"));
        std::string file_name = d["Brake"]["Input File"].GetString();
        int output = 0;
        if (d["Brake"].HasMember("Output")) {
            output = d["Brake"]["Output"].GetBool() ? +1 : -1;
        }
        LoadBrake(vehicle::GetDataFile(file_name), output);
    }

    // Create the idler
    {
        assert(d.HasMember("Idler"));
        std::string file_name = d["Idler"]["Input File"].GetString();
        int output = 0;
        if (d["Idler"].HasMember("Output")) {
            output = d["Idler"]["Output"].GetBool() ? +1 : -1;
        }
        LoadIdler(vehicle::GetDataFile(file_name), output);
        m_idler_loc = LoadVectorJSON(d["Idler"]["Location"]);
    }

    // Create the suspensions
    assert(d.HasMember("Suspension Subsystems"));
    assert(d["Suspension Subsystems"].IsArray());
    m_num_susp = d["Suspension Subsystems"].Size();
    m_suspensions.resize(m_num_susp);
    m_susp_locs.resize(m_num_susp);
    for (int i = 0; i < m_num_susp; i++) {
        std::string file_name = d["Suspension Subsystems"][i]["Input File"].GetString();
        bool has_shock = d["Suspension Subsystems"][i]["Has Shock"].GetBool();
        int output = 0;
        if (d["Suspension Subsystems"][i].HasMember("Output")) {
            output = d["Suspension Subsystems"][i]["Output"].GetBool() ? +1 : -1;
        }
        LoadSuspension(vehicle::GetDataFile(file_name), i, has_shock, output);
        m_susp_locs[i] = LoadVectorJSON(d["Suspension Subsystems"][i]["Location"]);
    }

    // Create the rollers
    m_num_rollers = 0;
    if (d.HasMember("Rollers")) {
        assert(d["Rollers"].IsArray());
        m_num_rollers = d["Rollers"].Size();
        m_rollers.resize(m_num_rollers);
        m_roller_locs.resize(m_num_rollers);
        for (int i = 0; i < m_num_rollers; i++) {
            std::string file_name = d["Rollers"][i]["Input File"].GetString();
            int output = 0;
            if (d["Rollers"][i].HasMember("Output")) {
                output = d["Rollers"][i]["Output"].GetBool() ? +1 : -1;
            }
            LoadRoller(vehicle::GetDataFile(file_name), i, output);
            m_roller_locs[i] = LoadVectorJSON(d["Rollers"][i]["Location"]);
        }
    }

    // Create the track shoes
    {
        assert(d.HasMember("Track Shoes"));
        std::string file_name = d["Track Shoes"]["Input File"].GetString();
        int output = 0;
        if (d["Track Shoes"].HasMember("Output")) {
            output = d["Track Shoes"]["Output"].GetBool() ? +1 : -1;
        }
        m_num_track_shoes = d["Track Shoes"]["Number Shoes"].GetInt();
        LoadTrackShoes(vehicle::GetDataFile(file_name), m_num_track_shoes, output);
    }

    // Read web mesh materials
    assert(d.HasMember("Layer Materials"));
    assert(d["Layer Materials"].HasMember("Steel"));
    assert(d["Layer Materials"].HasMember("Rubber"));

    double rho_steel = d["Layer Materials"]["Steel"]["Density"].GetDouble();
    ChVector<> E_steel = LoadVectorJSON(d["Layer Materials"]["Steel"]["E"]);
    ChVector<> nu_steel = LoadVectorJSON(d["Layer Materials"]["Steel"]["nu"]);
    ChVector<> G_steel = LoadVectorJSON(d["Layer Materials"]["Steel"]["G"]);
    SetRubberLayerMaterial(rho_steel, E_steel, nu_steel, G_steel);

    double rho_rubber = d["Layer Materials"]["Rubber"]["Density"].GetDouble();
    ChVector<> E_rubber = LoadVectorJSON(d["Layer Materials"]["Rubber"]["E"]);
    ChVector<> nu_rubber = LoadVectorJSON(d["Layer Materials"]["Rubber"]["nu"]);
    ChVector<> G_rubber = LoadVectorJSON(d["Layer Materials"]["Rubber"]["G"]);
    SetRubberLayerMaterial(rho_rubber, E_rubber, nu_rubber, G_rubber);

    ChVector<> ply_angles = LoadVectorJSON(d["Layer Materials"]["Ply Angles"]);
    SetLayerFiberAngles(ply_angles[0], ply_angles[1], ply_angles[2]);

    double alpha = d["Layer Materials"]["Structural Damping Coefficient"].GetDouble();
    SetElementStructuralDamping(alpha);

    // Read web contact material properties
    assert(d.HasMember("Mesh Contact Material"));

    SetContactSurfaceType(ChTrackAssemblyBandANCF::TRIANGLE_MESH);

    float mu = d["Mesh Contact Material"]["Coefficient of Friction"].GetFloat();
    float cr = d["Mesh Contact Material"]["Coefficient of Restitution"].GetFloat();
    SetContactFrictionCoefficient(mu);
    SetContactRestitutionCoefficient(cr);

    if (d["Mesh Contact Material"].HasMember("Properties")) {
        float ym = d["Mesh Contact Material"]["Properties"]["Young Modulus"].GetFloat();
        float pr = d["Mesh Contact Material"]["Properties"]["Poisson Ratio"].GetFloat();
        SetContactMaterialProperties(ym, pr);
    }
    if (d["Mesh Contact Material"].HasMember("Coefficients")) {
        float kn = d["Mesh Contact Material"]["Coefficients"]["Normal Stiffness"].GetFloat();
        float gn = d["Mesh Contact Material"]["Coefficients"]["Normal Damping"].GetFloat();
        float kt = d["Mesh Contact Material"]["Coefficients"]["Tangential Stiffness"].GetFloat();
        float gt = d["Mesh Contact Material"]["Coefficients"]["Tangential Damping"].GetFloat();
        SetContactMaterialCoefficients(kn, gn, kt, gt);
    }
}

}  // end namespace vehicle
}  // end namespace chrono
