#include "audioEngine.h"
#include "miniaudio.h"
#include <algorithm>

//float pitch = 1;
int note = 57;
int lastSample = 0;
int currentSound = 0;

audioEngine::audioEngine()
{
    resourceManagerConfig = ma_resource_manager_config_init();
    resourceManagerConfig.decodedFormat     = ma_format_f32;    /* ma_format_f32 should almost always be used as that's what the engine (and most everything else) uses for mixing. */
    resourceManagerConfig.decodedChannels   = 0;                /* Setting the channel count to 0 will cause sounds to use their native channel count. */
    resourceManagerConfig.decodedSampleRate = 48000;            /* Using a consistent sample rate is useful for avoiding expensive resampling in the audio thread. This will result in resampling being performed by the loading thread(s). */

    result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize resource manager.");
//        return -1;
    }


    /* We're going to want a context so we can enumerate our playback devices. */
    result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize context.");
//        return -1;
    }

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
//        return -1;
    }
    audioEngine::loadAudio("../audio/synth.wav");
}

audioEngine::~audioEngine()
{
  cleanupEngine();
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void audioEngine::cleanupEngine()
{
//  ma_device_uninit(&device);
//  ma_decoder_uninit(&decoder);


    /* The application owns the `ma_sound` object which means you're responsible for uninitializing them. */
//    for (iSound = 0; iSound < soundCount; iSound += 1) {
//        ma_sound_uninit(&sounds[iSound]);
//    }

    /* We can now uninitialize each engine. */
//    for (iEngine = 0; iEngine < engineCount; iEngine += 1) {
        ma_engine_uninit(&engine);

        /*
        The engine has been uninitialized so now lets uninitialize the device. Do this first to ensure we don't
        uninitialize the resource manager from under the device while the data callback is running.
        */
//        ma_device_uninit(&device);
//    }

    /*
    Do the resource manager last. This way we can guarantee the data callbacks of each device aren't trying to access
    and data managed by the resource manager.
    */
    ma_resource_manager_uninit(&resourceManager);
}

void audioEngine::playSample(int sample, int n)
{
  if(sample < 0 || sample > lastSample)
    return;

  double pitch = audioEngine::midiNoteToRate(n);
  ma_sound_set_pitch(&sounds[sample][currentSound], pitch);
//  note++;


//  result = ma_sound_start(&sounds[0]);
//  if (result != MA_SUCCESS) {
//            printf("WARNING: Failed to start sound.");
//  }

//    ma_engine_play_sound(&engine, "../audio/synth.wav", NULL);
  result = ma_sound_start(&sounds[sample][currentSound]);
  currentSound ++;
  if(currentSound > MAX_SOUNDS)
    currentSound = 0;
//  ma_engine_play_sound(&engine, argv[1], NULL);
/*
//  deviceConfig = ma_device_config_init(ma_device_type_playback);
    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
    }
*/
}

void audioEngine::stopAll()
{
}

void audioEngine::stopClip(int id)
{
}

int audioEngine::loadAudio(const std::string &uri)
{
  for (int i = 0; i < MAX_SOUNDS; i++)
  {
    result = ma_sound_init_from_file(&engine, uri.c_str(), MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM, NULL, NULL, &sounds[0][i]);
  }


  return 0;
}

double audioEngine::midiNoteToRate(int note)
{
  double ratio = std::pow(2.0, 1.0/12.0);
  double rate = std::pow(ratio, note-57);
  return rate;
}
