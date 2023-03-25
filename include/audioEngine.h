#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <cmath>

#define MAX_DEVICES 2
#define MAX_SOUNDS  32

class audioEngine
{

  public:
    audioEngine();
    ~audioEngine();
    int loadAudio(const std::string& uri);
    void playSample(int sample, int n);
    void stopAll();
    void stopClip(int id);

  private:
    void cleanupEngine();
    double midiNoteToRate(int note);
    ma_result result;
    ma_context context;
    ma_resource_manager_config resourceManagerConfig;
    ma_resource_manager resourceManager;
    ma_engine engine;
    ma_device device;
    ma_uint32 engineCount = 0;
    ma_uint32 iEngine;
    ma_device_info* pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount;
    ma_uint32 iAvailableDevice;
    ma_uint32 iChosenDevice;
    ma_sound sounds[MAX_SOUNDS][MAX_SOUNDS];
    ma_uint32 soundCount;
    ma_uint32 iSound;
    ma_resource_manager_data_source dataSource[MAX_SOUNDS];

};
    void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
