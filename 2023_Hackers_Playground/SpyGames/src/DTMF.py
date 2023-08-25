import os
from argparse import SUPPRESS, ArgumentDefaultsHelpFormatter, ArgumentParser

import numpy as np
import soundfile as sf
import yaml
from environment_simulation import (
    environment_simulation_advanced, environment_simulation_simple, environment_simulation_standard)
from pydub import AudioSegment
from scipy.io.wavfile import write
from scipy.signal import resample

from config import sample_rate, silence_duration, silence_duration_between_words, tone_duration, volume_level_decreasing


def generate_dtmf_tone(digit, duration, sample_rate):
    dtmf_freqs = {
        '1': (697, 1209), '2': (697, 1336), '3': (697, 1477),
        '4': (770, 1209), '5': (770, 1336), '6': (770, 1477),
        '7': (852, 1209), '8': (852, 1336), '9': (852, 1477),
        '*': (941, 1209), '0': (941, 1336), '#': (941, 1477)
    }

    freqs = dtmf_freqs[digit]
    t = np.linspace(0, duration, int(duration * sample_rate), False)

    tone = np.sin(freqs[0] * 2 * np.pi * t) + np.sin(freqs[1] * 2 * np.pi * t)
    tone *= 32767 / np.max(np.abs(tone))
    tone = tone.astype(np.int16)

    return tone


def generate_silence(duration, sample_rate):
    return np.zeros(int(duration * sample_rate), dtype=np.int16)


def sentence_to_digits(sentence):
    keyboard = {
        '.': '1', ',': '.11-','\'': '.111-', '?': '.1111-', '!': '.11111-', '-': '.111111-', '&': '.1111111-', ':': '.11111111-', '1': '.111111111-',
        'a': '2', 'b': '.22-', 'c': '.222-', '2': '.2222-',
        'd': '3', 'e': '.33-', 'f': '.333-', '3': '.3333-',
        'g': '4', 'h': '.44-', 'i': '.444-', '4': '.4444-',
        'j': '5', 'k': '.55-', 'l': '.555-', '5': '.5555-',
        'm': '6', 'n': '.66-', 'o': '.666-', '6': '.6666-',
        'p': '7', 'q': '.77-', 'r': '.777-', 's': '.7777-', '7': '.77777-',
        't': '8', 'u': '.88-', 'v': '.888-', '8': '.8888-',
        'w': '9', 'x': '.99-', 'y': '.999-', 'z': '.9999-', '9': '.99999-',
        ' ': '0', '0': '.00-', '\n': '.000-',
    }

    digits = ''

    for symbol in sentence:
        digits += keyboard[symbol]

    return digits


def digits_to_tones(digits, duration_key, duration_silence_one_key, duration_silence_between_keys):
    tones = []
    for digit in digits:
        if digit == '.':
            pass
        elif digit == '-':
            tones.append(generate_silence(duration_silence_between_keys, sample_rate))
        else:
            tones.append(generate_dtmf_tone(digit, duration_key, sample_rate))
            tones.append(generate_silence(duration_silence_one_key, sample_rate))
    return tones


if __name__ == '__main__':
    parser = ArgumentParser(description="This application provides functionality to generation DTMF tones in different "
                                        "environments",
                            formatter_class=ArgumentDefaultsHelpFormatter, add_help=False)
    required = parser.add_argument_group('required arguments')
    optional = parser.add_argument_group('optional arguments')
    required.add_argument("-o", "--output_dir", dest="output_dir",
                          help="Directory where the created audiofile will be stored", required=True)
    optional.add_argument("-i", "--input_sentence", dest="input_sentence", default="test sentence",
                          help="Sentence to encode in DTMF tones")
    optional.add_argument("-m", "--mode", dest="simulation_mode", default='standard', choices=["simple", "standard", "advanced"],
                          help="Simulation mode")
    optional.add_argument("-c", "--conversation", dest="conversation", default=False, action="store_true",
                          help="Use conversation mode. Conversation text can be changed in conversation.yaml")
    optional.add_argument(
        '-h',
        '--help',
        action='help',
        default=SUPPRESS,
        help='show this help message and exit'
    )
    args = parser.parse_args()

    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    if not args.conversation:
        sentence = args.input_sentence
        output_file = os.path.join(args.output_dir, f'dtmf_tones_[{sentence}].wav')
        digits = sentence_to_digits(sentence)
        print(digits)
        with open("generated_sequence.txt", "w") as f:
            f.write(digits)
        tones = digits_to_tones(digits, tone_duration, silence_duration, silence_duration_between_words)
        sequence = np.concatenate(tones)
        write(output_file, sample_rate, sequence)
        start_silence = AudioSegment.silent(duration=1000)
        sound_event_full_volume = AudioSegment.from_file(output_file, format="wav")
        if not os.path.exists("noise_samples/nokia_3310.wav"):
            msg_signal, fs = sf.read("noise_samples/nokia_3310.mp3")
            msg_signal = np.mean(msg_signal, axis=1)
            msg_signal = resample(msg_signal, int(len(msg_signal) * sample_rate / fs))
            sf.write("noise_samples/nokia_3310.wav", msg_signal, sample_rate)
        msg_signal = AudioSegment.from_file("noise_samples/nokia_3310.wav", format="wav", duration=1000)
        sound_event = sound_event_full_volume - volume_level_decreasing
        sound_event = start_silence + msg_signal + start_silence + sound_event
        sound_event.export("quite.wav", "wav")

        if args.simulation_mode == "standard":
            environment_simulation_standard(sample_rate, args.output_dir, sentence)
        elif args.simulation_mode == "simple":
            environment_simulation_simple(sample_rate, args.output_dir, sentence)
        elif args.simulation_mode == "advanced":
            environment_simulation_advanced(sample_rate, args.output_dir, sentence)

    else:
        tones_first = []
        tones_second = []
        if not os.path.exists("noise_samples/nokia_3310.wav"):
            msg_signal, fs = sf.read("noise_samples/nokia_3310.mp3")
            msg_signal = np.mean(msg_signal, axis=1)
            msg_signal = resample(msg_signal, int(len(msg_signal) * sample_rate / fs))
            sf.write("noise_samples/nokia_3310.wav", msg_signal, sample_rate)
        msg_signal, fs = sf.read("noise_samples/nokia_3310.wav", dtype=np.int16)
        msg_signal = np.concatenate([msg_signal, generate_silence(len(msg_signal)/2/sample_rate, sample_rate)])
        with open("conversation.yaml", "r") as stream:
            conversation = yaml.safe_load(stream)
        digits = []
        for phrase_num in range(len(conversation["first"])):
            digits_first = sentence_to_digits(conversation["first"][phrase_num])
            digits_second = sentence_to_digits(conversation["second"][phrase_num])
            digits.append(digits_first)
            digits.append(digits_second)
            if tones_first:
                tones_temp = digits_to_tones(digits_first, tone_duration, silence_duration,
                                             silence_duration_between_words)
                for elem in tones_temp:
                    tones_first.append(elem)
            else:
                tones_first = digits_to_tones(digits_first, tone_duration, silence_duration,
                                              silence_duration_between_words)
            tones_first.append(generate_silence(len(msg_signal)/sample_rate, sample_rate))
            tones_second.append(generate_silence(len(np.concatenate(digits_to_tones(digits_first, tone_duration, silence_duration,
                                                                     silence_duration_between_words)))/sample_rate, sample_rate))
            tones_second.append(msg_signal)
            tones_temp = digits_to_tones(digits_second, tone_duration, silence_duration,
                                         silence_duration_between_words)
            for elem in tones_temp:
                tones_second.append(elem)
            tones_second.append(generate_silence(len(msg_signal)/sample_rate, sample_rate))
            tones_first.append(generate_silence(len(np.concatenate(digits_to_tones(digits_second, tone_duration, silence_duration,
                                                silence_duration_between_words)))/sample_rate, sample_rate))
            tones_first.append(msg_signal)
        sequence_first = np.concatenate(tones_first)
        sequence_second = np.concatenate(tones_second)
        start_silence = AudioSegment.silent(duration=1000)
        print(digits)

        output_file = os.path.join(args.output_dir, f'dtmf_tones_conversation.wav')
        write(output_file, sample_rate, sequence_first)
        sound_event_full_volume = AudioSegment.from_file(output_file, format="wav")
        sound_event = sound_event_full_volume - volume_level_decreasing
        sound_event = start_silence + sound_event
        sound_event.export("quite.wav", "wav")

        output_file2 = os.path.join(args.output_dir, f'dtmf_tones_conversation2.wav')
        write(output_file2, sample_rate, sequence_second)
        sound_event_full_volume2 = AudioSegment.from_file(output_file2, format="wav")
        sound_event2 = sound_event_full_volume2 - volume_level_decreasing
        sound_event2 = start_silence + sound_event2
        sound_event2.export("quite_second.wav", "wav")

        if args.simulation_mode == "standard":
            environment_simulation_standard(sample_rate, args.output_dir, "conversation")
        elif args.simulation_mode == "simple":
            environment_simulation_simple(sample_rate, args.output_dir, "conversation")
        elif args.simulation_mode == "advanced":
            environment_simulation_advanced(sample_rate, args.output_dir, "conversation", conversation=True)



