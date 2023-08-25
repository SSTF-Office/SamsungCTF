import os.path
import numpy as np
import pyroomacoustics as pra
import soundfile as sf
from scipy.signal import fftconvolve
from pysofaconventions import SOFAFile
from scipy.signal import resample


def environment_simulation_simple(fs, output_dir, sentence):
    sound_event, _ = sf.read("quite.wav")
    ambient_noise, fs2 = sf.read("noise_samples/mixkit-hotel-reception-and-restaurant-454.wav")
    ambient_noise = ambient_noise * 4
    ambient_noise = np.mean(ambient_noise, axis=1)
    if ambient_noise.shape[0] < sound_event.shape[0]:
        ambient_noise = np.concatenate([ambient_noise] * (ambient_noise.shape[0]//ambient_noise.shape[0] + 1))
    ambient_noise = np.tile(ambient_noise[:sound_event.shape[0]], 1)

    additional_event, fs3 = sf.read("noise_samples/crunchy-bites-88726.mp3")
    additional_event = resample(additional_event, int(len(additional_event) * fs/fs3))
    additional_event = additional_event * 4
    if additional_event.shape[0] < sound_event.shape[0]:
        additional_event = np.concatenate([additional_event] * (sound_event.shape[0]//additional_event.shape[0] + 1))
    additional_event = np.tile(additional_event[:sound_event.shape[0]], 1)

    additional_event_2, fs4 = sf.read("noise_samples/dishes-clearing-73302.mp3")
    additional_event_2 = resample(additional_event_2, int(len(additional_event_2) * fs / fs4))
    additional_event_2 = additional_event_2 * 4
    if additional_event_2.shape[0] < sound_event.shape[0]:
        additional_event_2 = np.concatenate([additional_event_2] * (sound_event.shape[0]//additional_event_2.shape[0] + 1))
    additional_event_2 = np.tile(additional_event_2[:sound_event.shape[0]], 1)

    additional_event_3, fs5 = sf.read("noise_samples/stay-down-67845.mp3")
    additional_event_3 = resample(additional_event_3, int(len(additional_event_3) * fs / fs5))
    additional_event_3 = np.mean(additional_event_3, axis=1)
    additional_event_3 = additional_event_3 * 4
    if additional_event_3.shape[0] < sound_event.shape[0]:
        additional_event_3 = np.concatenate([additional_event_3] * (sound_event.shape[0]//additional_event_3.shape[0] + 1))
    additional_event_3 = np.tile(additional_event_3[:sound_event.shape[0]], 1)

    additional_event_4, fs6 = sf.read("noise_samples/large-dining-event-sound-65771.mp3")
    additional_event_4 = additional_event_4[fs6:]
    additional_event_4 = resample(additional_event_4, int(len(additional_event_4) * fs / fs6))
    additional_event_4 = np.mean(additional_event_4, axis=1)
    additional_event_4 = additional_event_4 * 4
    if additional_event_4.shape[0] < sound_event.shape[0]:
        additional_event_4 = np.concatenate([additional_event_4] * (sound_event.shape[0]//additional_event_4.shape[0] + 1))
    additional_event_4 = np.tile(additional_event_4[:sound_event.shape[0]], 1)

    output_signal = sound_event + ambient_noise + additional_event + additional_event_2 + additional_event_3 + additional_event_4

    output_file = os.path.join(output_dir, f"dtmf_tones_with_noise_simple_[{sentence}].wav")
    sf.write(output_file, output_signal, fs)


def environment_simulation_standard(fs, output_dir, sentence):
    sound_event, _ = sf.read("quite.wav")
    room_dimensions = [10, 10, 3]
    room = pra.ShoeBox(room_dimensions, fs=fs, max_order=3)

    ambient_noise, fs2 = sf.read("noise_samples/mixkit-hotel-reception-and-restaurant-454.wav")
    ambient_noise = np.mean(ambient_noise, axis=1)
    ambient_noise = ambient_noise * 4
    ambient_noise = resample(ambient_noise, int(len(ambient_noise) * fs/fs2))
    if ambient_noise.shape[0] < sound_event.shape[0]:
        ambient_noise = np.concatenate([ambient_noise] * (ambient_noise.shape[0]//ambient_noise.shape[0] + 1))

    additional_event, fs3 = sf.read("noise_samples/crunchy-bites-88726.mp3")
    additional_event = resample(additional_event, int(len(additional_event) * fs/fs3))
    additional_event = additional_event * 4
    if additional_event.shape[0] < sound_event.shape[0]:
        additional_event = np.concatenate([additional_event] * (additional_event.shape[0]//additional_event.shape[0] + 1))

    additional_event_2, fs4 = sf.read("noise_samples/dishes-clearing-73302.mp3")
    additional_event_2 = resample(additional_event_2, int(len(additional_event_2) * fs / fs4))
    additional_event_2 = additional_event_2 * 4
    if additional_event_2.shape[0] < sound_event.shape[0]:
        additional_event_2 = np.concatenate([additional_event_2] * (additional_event_2.shape[0]//additional_event_2.shape[0] + 1))

    additional_event_3, fs5 = sf.read("noise_samples/stay-down-67845.mp3")
    additional_event_3 = resample(additional_event_3, int(len(additional_event_3) * fs / fs5))
    additional_event_3 = np.mean(additional_event_3, axis=1)
    additional_event_3 = additional_event_3 * 4
    if additional_event_3.shape[0] < sound_event.shape[0]:
        additional_event_3 = np.concatenate([additional_event_3] * (additional_event_3.shape[0]//additional_event_3.shape[0] + 1))

    additional_event_4, fs6 = sf.read("noise_samples/large-dining-event-sound-65771.mp3")
    additional_event_4 = additional_event_4[fs6:]
    additional_event_4 = resample(additional_event_4, int(len(additional_event_4) * fs / fs6))
    additional_event_4 = np.mean(additional_event_4, axis=1)
    additional_event_4 = additional_event_4 * 4
    if additional_event_4.shape[0] < sound_event.shape[0]:
        additional_event_4 = np.concatenate([additional_event_4] * (additional_event_4.shape[0]//additional_event_4.shape[0] + 1))

    source_position = [8, 8, 1]
    additional_event_position = [2, 4, 1]
    additional_event_2_position = [5, 6, 1]
    additional_event_3_position = [7, 7, 1]
    additional_event_4_position = [6, 4, 1]
    ambient_noise_position = [1, 1, 1]

    mic_center = np.c_[[1, 3, 1]]
    mic_locs = mic_center + np.c_[[0.1, 0.02, 0]]
    room.add_microphone_array(mic_locs)
    room.add_source(source_position, signal=sound_event)
    room.add_source(additional_event_position, signal=additional_event)
    room.add_source(additional_event_2_position, signal=additional_event_2)
    room.add_source(additional_event_3_position, signal=additional_event_3)
    room.add_source(additional_event_4_position, signal=additional_event_4)
    room.add_source(ambient_noise_position, signal=ambient_noise)

    room.simulate()

    output_signal = np.tile(room.mic_array.signals.T[:sound_event.shape[0]], 1)

    output_file = os.path.join(output_dir, f"dtmf_tones_with_noise_standard_[{sentence}].wav")
    sf.write(output_file, output_signal, fs)


def environment_simulation_advanced(fs, output_dir, sentense, conversation=False):
    sofa = SOFAFile("RIRs/Restaurant.sofa", "r")

    brirs = sofa.getDataIR()

    sound_event, _ = sf.read("quite.wav")

    ambient_noise, fs2 = sf.read("noise_samples/mixkit-hotel-reception-and-restaurant-454.wav")
    ambient_noise = np.mean(ambient_noise, axis=1)
    ambient_noise = ambient_noise * 4
    ambient_noise = resample(ambient_noise, int(len(ambient_noise) * fs/fs2))

    additional_event, fs3 = sf.read("noise_samples/crunchy-bites-88726.mp3")
    additional_event = resample(additional_event, int(len(additional_event) * fs/fs3))
    additional_event = additional_event * 4

    additional_event_2, fs4 = sf.read("noise_samples/dishes-clearing-73302.mp3")
    additional_event_2 = resample(additional_event_2, int(len(additional_event_2) * fs / fs4))
    additional_event_2 = additional_event_2 * 4

    additional_event_3, fs5 = sf.read("noise_samples/stay-down-67845.mp3")
    additional_event_3 = resample(additional_event_3, int(len(additional_event_3) * fs / fs5))
    additional_event_3 = np.mean(additional_event_3, axis=1)
    additional_event_3 = additional_event_3 * 4

    additional_event_4, fs6 = sf.read("noise_samples/large-dining-event-sound-65771.mp3")
    additional_event_4 = additional_event_4[fs6:]
    additional_event_4 = resample(additional_event_4, int(len(additional_event_4) * fs / fs6))
    additional_event_4 = np.mean(additional_event_4, axis=1)
    additional_event_4 = additional_event_4 * 4

    rir = brirs[10, :, :]
    binaural_left_event = fftconvolve(sound_event, rir[0][0])
    binaural_right_event = fftconvolve(sound_event, rir[0][1])
    binaural_event = np.asarray([binaural_left_event, binaural_right_event]).swapaxes(-1, 0)

    rir = brirs[15, :, :]
    binaural_left_noise = fftconvolve(ambient_noise, rir[4][0])
    binaural_right_noise = fftconvolve(ambient_noise, rir[4][1])
    binaural_noise = np.asarray([binaural_left_noise, binaural_right_noise]).swapaxes(-1, 0)
    if binaural_noise.shape[0] < binaural_event.shape[0]:
        binaural_noise = np.concatenate([binaural_noise] * (binaural_event.shape[0]//binaural_noise.shape[0] + 1))
    binaural_noise = np.tile(binaural_noise[:binaural_event.shape[0]], 1)

    rir = brirs[10, :, :]
    binaural_left_additional_event = fftconvolve(additional_event, rir[3][0])
    binaural_right_additional_event = fftconvolve(additional_event, rir[3][1])
    binaural_additional_event = np.asarray([binaural_left_additional_event, binaural_right_additional_event]).swapaxes(-1, 0)
    if binaural_additional_event.shape[0] < binaural_event.shape[0]:
        binaural_additional_event = np.concatenate([binaural_additional_event]*(binaural_event.shape[0]//binaural_additional_event.shape[0] + 1))
    binaural_additional_event = np.tile(binaural_additional_event[:binaural_event.shape[0]], 1)

    rir = brirs[25, :, :]
    binaural_left_additional_event_2 = fftconvolve(additional_event_2, rir[2][0])
    binaural_right_additional_event_2 = fftconvolve(additional_event_2, rir[2][1])
    binaural_additional_event_2 = np.asarray([binaural_left_additional_event_2, binaural_right_additional_event_2]).swapaxes(-1, 0)
    if binaural_additional_event_2.shape[0] < binaural_event.shape[0]:
        binaural_additional_event_2 = np.concatenate([binaural_additional_event_2]*(binaural_event.shape[0]//binaural_additional_event_2.shape[0] + 1))
    binaural_additional_event_2 = np.tile(binaural_additional_event_2[:binaural_event.shape[0]], 1)

    rir = brirs[50, :, :]
    binaural_left_additional_event_3 = fftconvolve(additional_event_3, rir[1][0])
    binaural_right_additional_event_3 = fftconvolve(additional_event_3, rir[1][1])
    binaural_additional_event_3 = np.asarray([binaural_left_additional_event_3, binaural_right_additional_event_3]).swapaxes(-1, 0)
    if binaural_additional_event_3.shape[0] < binaural_event.shape[0]:
        binaural_additional_event_3 = np.concatenate([binaural_additional_event_3]*(binaural_event.shape[0]//binaural_additional_event_3.shape[0] + 1))
    binaural_additional_event_3 = np.tile(binaural_additional_event_3[:binaural_event.shape[0]], 1)


    rir = brirs[35, :, :]
    binaural_left_additional_event_4 = fftconvolve(additional_event_4, rir[1][0])
    binaural_right_additional_event_4 = fftconvolve(additional_event_4, rir[1][1])
    binaural_additional_event_4 = np.asarray([binaural_left_additional_event_4, binaural_right_additional_event_4]).swapaxes(-1, 0)
    if binaural_additional_event_4.shape[0] < binaural_event.shape[0]:
        binaural_additional_event_4 = np.concatenate([binaural_additional_event_4]*(binaural_event.shape[0]//binaural_additional_event_4.shape[0] + 1))
    binaural_additional_event_4 = np.tile(binaural_additional_event_4[:binaural_event.shape[0]], 1)

    output_signal = binaural_event + binaural_noise + binaural_additional_event + binaural_additional_event_2 + binaural_additional_event_3 + binaural_additional_event_4

    if conversation:
        rir = brirs[50, :, :]
        sound_event2, _ = sf.read("quite_second.wav")
        binaural_left_event = fftconvolve(sound_event2, rir[5][0])
        binaural_right_event = fftconvolve(sound_event2, rir[5][1])
        binaural_event2 = np.asarray([binaural_left_event, binaural_right_event]).swapaxes(-1, 0)
        if binaural_event2.shape[0] < binaural_event.shape[0]:
            output_signal = np.tile(output_signal[:binaural_event2.shape[0]], 1)
        else:
            binaural_event2 = np.tile(binaural_event2[:binaural_event.shape[0]], 1)
        output_signal += binaural_event2

    output_file = os.path.join(output_dir, f"dtmf_tones_with_noise_advanced_[{sentense}].wav")
    sf.write(output_file, output_signal, fs)
