#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: asdffds
# Author: SCDP001
# GNU Radio version: v3.8.2.0-57-gd71cd177

from gnuradio import analog
from gnuradio import blocks
from gnuradio import filter
from gnuradio.filter import firdes
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import epy_block_0
import time

class rawsave(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "asdffds")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 20e6
        self.channel_width = channel_width = 200e3
        self.channel_freq = channel_freq = 90.7e6
        self.center_freq = center_freq = 100e6

        ##################################################
        # Blocks
        ##################################################
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=12,
                decimation=5,
                taps=None,
                fractional_bw=None)
        self.low_pass_filter_0 = filter.fir_filter_ccf(
            int(samp_rate/channel_width),
            firdes.low_pass(
                1,
                samp_rate,
                75e3,
                25e3,
                firdes.WIN_BLACKMAN,
                6.76))
        self.epy_block_0 = epy_block_0.blk(example_param=123)
        self.blocks_wavfile_sink_0 = blocks.wavfile_sink('e:\\testpython.wav', 1, 48000, 16)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.analog_wfm_rcv_0 = analog.wfm_rcv(
        	quad_rate=480000,
        	audio_decimation=10,
        )
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, center_freq-channel_freq, 1, 0, 0)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, center_freq-channel_freq, 1, 0, 0)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.epy_block_0, 0))
        self.connect((self.analog_wfm_rcv_0, 0), (self.blocks_wavfile_sink_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.low_pass_filter_0, 0))
        self.connect((self.epy_block_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.low_pass_filter_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.analog_wfm_rcv_0, 0))


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 75e3, 25e3, firdes.WIN_BLACKMAN, 6.76))

    def get_channel_width(self):
        return self.channel_width

    def set_channel_width(self, channel_width):
        self.channel_width = channel_width

    def get_channel_freq(self):
        return self.channel_freq

    def set_channel_freq(self, channel_freq):
        self.channel_freq = channel_freq
        self.analog_sig_source_x_0.set_frequency(self.center_freq-self.channel_freq)
        self.analog_sig_source_x_0_0.set_frequency(self.center_freq-self.channel_freq)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.analog_sig_source_x_0.set_frequency(self.center_freq-self.channel_freq)
        self.analog_sig_source_x_0_0.set_frequency(self.center_freq-self.channel_freq)





def main(top_block_cls=rawsave, options=None):
    tb = top_block_cls()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        sys.exit(0)

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    tb.set_channel_freq(102.5e6)

    tb.start()

    while tb.epy_block_0.done == False:
        try:
            time.sleep(1)
            print("Waking up")
            print(tb.epy_block_0.done)
        except KeyboardInterrupt:
            sig_handler()

    print("DONE")
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
