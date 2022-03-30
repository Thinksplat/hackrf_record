"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr


class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, example_param=1.0):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Embedded Python Block',   # will show up in GRC
            in_sig=[np.complex64],
            out_sig=[np.complex64]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.example_param = example_param

        # Open a file
        file = "e:\\hackrf0-100000kHz-IQ.wav"
        self.file = open(file, "rb")
        self.done = False

    # destructor
    def __del__(self):
        self.file.close()

    def work(self, input_items, output_items):
        """example: multiply with constant"""
        size = len(input_items[0])

        # read size samples from wav file
        data = self.file.read(size * 2)
        
        if len(data) != size * 2:
            self.done = True
            # fill output items with zeros
            for i in range(len(output_items[0])):
                output_items[0][i] = 0
            return len(output_items[0])

        # interpret data as array of unsigned chars
        data = np.frombuffer(data, dtype=np.uint8)
        data = data / 255.0

        data = data.astype(np.float32).view(np.complex64)

        output_items[0][:] = data
        # for n in range(0, size):
        #    output_items[0][n] = data[n]

        return len(output_items[0])
