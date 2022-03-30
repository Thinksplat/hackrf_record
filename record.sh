# hackrf_transfer -r - -f 100000000  -s 20000000 | python buffer.py  > /mnt/data.raw

# Prime the pump
hackrf_transfer -r -f 100000000 -s 20000000 -n 10000 > /dev/null
hackrf_transfer -r -f 100000000 -s 20000000 -n 10000 > /dev/null

nice -n -5 hackrf_transfer -r - -f 100000000  -s 20000000 | nice -n -4 ./signed_to_unsigned # | python bufferrecord.py
