// jsbsim_c_api.cpp
#include "jsbsimAPI.h"
#include <FGFDMExec.h>
#include <cstring>
#include <models/FGPropagate.h>
#include <models/FGAuxiliary.h>

using namespace JSBSim;

struct JSBSimHandle {
    FGFDMExec* fdm;
    std::string lastError;
};

void* JSBSim_Create(const char* rootDir) {
    JSBSimHandle* handle = new JSBSimHandle();
    handle->fdm = new FGFDMExec();

    if (rootDir) {
        SGPath root(rootDir);
        handle->fdm->SetRootDir(root);
        handle->fdm->SetAircraftPath(root / "aircraft");
        handle->fdm->SetEnginePath(root / "engine");
        handle->fdm->SetSystemsPath(root / "systems");

        handle->lastError = "RootDir: " + root.utf8Str() +
            " | AircraftPath: " + (root / "aircraft").utf8Str();
    }

    return static_cast<void*>(handle);
}

void JSBSim_Destroy(void* fdm) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    delete handle->fdm;
    delete handle;
}

int JSBSim_LoadModel(void* fdm, const char* modelName) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    bool result = handle->fdm->LoadModel(modelName);

    handle->lastError = std::string("LoadModel(") + modelName + ") -> " +
        (result ? "OK" : "FAILED");

    return result ? 1 : 0;
}

void JSBSim_GetLastError(void* fdm, char* buffer, int bufferSize) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    strncpy(buffer, handle->lastError.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

// TODO: aggiornare le funzioni RunIC, Run, SetProperty, GetProperty, GetSimTime per usare handle->fdm invece di fdm direttamente
void JSBSim_RunIC(void* fdm) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    handle->fdm->RunIC();
}

int JSBSim_Run(void* fdm) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    return handle->fdm->Run() ? 1 : 0;
}

void JSBSim_SetPropertyDouble(void* fdm, const char* property, double value) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    handle->fdm->SetPropertyValue(property, value);
}

double JSBSim_GetPropertyDouble(void* fdm, const char* property) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    return handle->fdm->GetPropertyValue(property);
}

double JSBSim_GetSimTime(void* fdm) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    return handle->fdm->GetSimTime();
}

void JSBSim_GetState(void* fdm, JSBSimState* state) {
    JSBSimHandle* handle = static_cast<JSBSimHandle*>(fdm);
    auto propagate = handle->fdm->GetPropagate();
    auto auxiliary = handle->fdm->GetAuxiliary();

    // Posizione
    state->latitude_deg = propagate->GetLatitudeDeg();
    state->longitude_deg = propagate->GetLongitudeDeg();
    state->altitude_asl_ft = propagate->GetAltitudeASL();
    state->altitude_agl_ft = propagate->GetDistanceAGL(); // ft

    // Velocità
    state->vcas_kts = auxiliary->GetVcalibratedKTS();
    state->vtrue_kts = auxiliary->GetVtrueKTS();
    state->vground_kts = auxiliary->GetVground();
    state->vN_fps = propagate->GetVel(1); // componenti NED, indice 1/2/3 = N/E/D
    state->vE_fps = propagate->GetVel(2);
    state->vD_fps = propagate->GetVel(3);

    // Assetto
    state->roll_rad = propagate->GetEuler(1); // phi
    state->pitch_rad = propagate->GetEuler(2); // theta
    state->heading_rad = propagate->GetEuler(3); // psi

    // Velocità angolari
    state->p_rad_s = propagate->GetPQR(1);
    state->q_rad_s = propagate->GetPQR(2);
    state->r_rad_s = propagate->GetPQR(3);
}
