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
        self.file_index = 0

        self.open_next_file();

        # Skip wave header
        self.file.seek(44)
        self.done = False

    # destructor
    def __del__(self):
        if self.file is not None:
            self.file.close()
    
    def open_next_file(self):
        filename = self.next_filename()
        print("Opening file: " + filename)
        try:
            self.file = open(filename, "rb")
        except:
            print("Could not open file: " + filename)
            self.file = None
            self.done = True
            return
        # Skip wave header
        self.file.seek(44)
    
    def next_filename(self):
        ret = "e:\\hackrf" + str(self.file_index) + "-100000kHz-IQ.wav"
        self.file_index += 1
        return ret

    def next_n_samples_from_open_file(self,n):
        data = self.file.read(n*2);

        if len(data) != n*2:
            # Open the next file in line
            self.file.close()
            self.open_next_file()

        data = np.frombuffer(data, dtype=np.uint8)
        data = data / 255.0
        return data.astype(np.float32).view(np.complex64)
    
    def next_n_samples(self, n):
        # data is empty complex array
        data = self.next_n_samples_from_open_file(n)
        n -= len(data)

        while(self.done == False and n > 0):
            print("Reading more data: " + str(n))
            filedata = self.next_n_samples_from_open_file(n)
            n -= len(filedata)
            # append filedata to data
            data = np.append(data,filedata)
        return data


    def work(self, input_items, output_items):
        """example: multiply with constant"""
        size = len(input_items[0])

        if(self.done):
            # fill with 0
            output_items[0][:] = 0
            return len(output_items[0])

        data = self.next_n_samples(size)
        
        if len(data) != size:
            print("Not enough data in work: " + str(size) + " vs " + str(len(data)))
            print("Setting done to true")
            self.done = True
            # fill output items with zeros
            for i in range(len(output_items[0])):
                output_items[0][i] = 0
            return len(output_items[0])

        output_items[0][:] = data
        # for n in range(0, size):
        #    output_items[0][n] = data[n]

        return len(output_items[0])
