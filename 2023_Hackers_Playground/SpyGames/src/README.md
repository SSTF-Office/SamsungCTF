# CTFAudioTask

## Requirements

- python 3.8+
- Other requirements mentioned in `requirements.txt`

## Usage 

Download `Restaurant sofa` file with RIRs from https://www.phon.ucl.ac.uk/downloads/ebrird/Restaurant.sofa and place it in `RIRs` directory.  

To create DTMF tones for chosen sentence run: 

```bash 
python DTMF.py -i <input_sentence> -o <output_folder> -m <mode> 
```

To create conversation recording update conversation.yaml and run script with `-c` flag.
Note! Script assumes that both speakers have the same number of phrases.

## Modes

### Simple

In this mode script simply overlays generated DTMF tones with noise and additional sound events

### Standard

Standard mode uses pyroomacoustics library for simulating. The script creates virtual room and place generated DTMF tones and additional events as sources in different locations within the space. After that simulation recorded with virtual microphone.  

### Advanced

In advanced mode, the script utilizes pre-recorded Room Impulse Responses (RIRs) in SOFA file format to generate audio scene. For generated DTMF tones and each additional event a corresponding RIR is chosen and audio sources are combined in a convolution process. This results in more realistic audio scene simulation. 