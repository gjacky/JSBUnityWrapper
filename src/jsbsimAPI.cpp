// jsbsim_c_api.cpp
#include "jsbsimAPI.h"
#include <FGFDMExec.h>
#include <cstring>

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
