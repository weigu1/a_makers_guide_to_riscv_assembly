import numpy as np
import matplotlib.pyplot as plt

def wavetable_from_function(f, size, nr_bit, signed=True):
    """ Generate a wavetable from a function and a specified length.
        You can choose between signed or unsigned output. """
    function = f(2 * np.pi * np.arange(size) / size) # Values between -1 and 1
    if signed: # Scale to signed
        max_val = 2**(nr_bit - 1) - 1
        wavetable = (function * max_val).astype(np.int32)
    else: # Unsigned: shift the sine wave from [-1, 1] to [0, 2], then scale
        max_val = 2**(nr_bit) - 1
        wavetable = (((function + 1) / 2) * max_val).astype(np.uint32)
    return wavetable.tolist()

def waveform_sine(x):
    """Sawtooth with period 2 pi."""
    return np.sin(x)

def waveform_saw(x):
    """Sawtooth with period 2 pi."""
    return (x + np.pi) / np.pi % 2 - 1

def waveform_square(x):
    """Square waveform with period 2 pi."""
    x=x+0.00039
    return np.sign(np.sin(x))

def write_wavetables_assembler_file(wavetables, bit_nr):
    header_text ="""### wavetables.S
### weigu.lu
### Here we have the data for some wavetables

        .section .rodata            # Start of the read only data section in memory
        .balign 4\n"""
    if bit_nr == 8:
        datatype = "        .byte   "
    elif bit_nr == 16:
        datatype = "        .hword  "
    elif bit_nr == 24:
        datatype = "        .byte   "
    elif bit_nr == 32:
        datatype = "        .word   "

    try:
        with open(filename, 'w') as f:
            f.write(header_text)
            for key, value in wavetables.items():
                f.write(f"{key}:")
                counter = 0
                for nr in value:
                    if counter%8 == 0:
                        f.write(f"\n{datatype}")
                    if counter%8 == 7:
                         f.write(f"{nr}")
                    else:
                         f.write(f"{nr}, ")
                    counter += 1
                f.write(f"\n")

    except IOError as e:
        print(f"Cannot create file: {filename}. Error: {e}")
        exit()




# Global parameters
filename = "wavetables.S"
sampling_rate = 48000
wavetable_size = 2048
wavetable_bits = 16
wavetable_signed = True

def main():

# Reading and printing the file

    wt_sine = wavetable_from_function(waveform_sine,
                                      wavetable_size,
                                      wavetable_bits,
                                      wavetable_signed)
    wt_saw = wavetable_from_function(waveform_saw,
                                     wavetable_size,
                                     wavetable_bits,
                                     wavetable_signed)
    wt_square = wavetable_from_function(waveform_square,
                                        wavetable_size,
                                        wavetable_bits,
                                        wavetable_signed)

    wavetables = {"wt_sine":wt_sine, "wt_saw":wt_saw, "wt_square":wt_square}
    write_wavetables_assembler_file(wavetables, wavetable_bits)


    for value in wavetables.values():
        plt.plot(value)

    plt.xlabel('Samples', fontsize=22)  # Set x-axis label font size
    plt.ylabel('Amplitude', fontsize=22)  # Set y-axis label font size
    plt.xticks(fontsize=22)             # Set x-tick font size
    plt.yticks(fontsize=22)             # Set y-tick font size
    plt.show()

#lists_in_dict = [value for value in my_dict.values() if isinstance(value, list)]

if __name__ == '__main__':
    main()
