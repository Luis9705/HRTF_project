
import serial
from scipy.io import wavfile
import numpy as  np
import sys
import time
import simpleaudio as sa
import sounddevice as sd
import threading

fs, data = wavfile.read('song1.wav')
right_channel, left_channel = data[:,1], data[:,0]
t = np.linspace(0, len(data) / fs, num=len(data))

start_time = 50
duration = 100

BLOCK_SIZE = 640

send_data = []
recieved_data = []

comPort = None

recieve_flag = True
timeout_error = False

def TransmitThread():
    global recieve_flag
    global comPort
    global send_data
    global timeout_error
    current_percent = 0
    if comPort:
        print("Starting Transmission...", flush=True)
        data_list = right_channel[np.where((t > start_time) &( t < start_time + duration))[0]].tolist()
        data_blocks = [data_list[x:x+BLOCK_SIZE] for x in range(0, len(data_list),BLOCK_SIZE)]
        for i, data_block in enumerate(data_blocks):
            if len(data_block) ==  BLOCK_SIZE:
                #print(f"Block {i}")
                send_frame = b""
                for sample in data_block:
                    send_frame+= sample.to_bytes(2, 'big', signed=True)
                    send_data.append(sample)  
                comPort.write(send_frame)
                percentage = (len(data_block)*i)/len(data_list) * 100
                if percentage >= current_percent:
                    print(f"{current_percent}% processed", flush=True)
                    current_percent+= 5
                if timeout_error:
                    break
                time.sleep(0.001)
        time.sleep(0.01)
        recieve_flag = False
        print("Transmission finished!", flush=True)
        #print(send_data[:10], flush=True)

def ReceiveThread():
    global recieve_flag
    global recieved_data
    global timeout_error
    global comPort
    while True:  
        recived_frame = []
        recieved = comPort.read(BLOCK_SIZE*2 + 5)
        if not recieved:
            timeout_error = True
            break
        for c in recieved:
            recived_frame.append(c)
        bytes_list = recived_frame[3:-2]
        composite_list = [bytes_list[x:x+2] for x in range(0, len(bytes_list),2)]
        recieved_bytes = [int.from_bytes(bytearray(byte), byteorder='big', signed=True) for byte in composite_list]
        recieved_data += recieved_bytes
        
        time.sleep(0.001)
        if not recieve_flag:
            break
    print("Reception finished!", flush=True)
        #print(recieved_data[:10], flush=True)

def LoopbackTest(comPortName):
    global comPort
    start = time.time()
    print("Opening port...", flush=True)
    comPort = serial.Serial \
            (
              port=comPortName,
              baudrate=1500000,
              parity=serial.PARITY_NONE,
              stopbits=serial.STOPBITS_ONE,
              bytesize=serial.EIGHTBITS,
              timeout = 2
            )

    transmit_thread = threading.Thread(target=TransmitThread)
    receive_thread = threading.Thread(target=ReceiveThread)

    transmit_thread.start()
    receive_thread.start()


    transmit_thread.join()

    receive_thread.join()

    

    # filename = 'song_output.wav'
    # wave_obj = sa.WaveObject.from_wave_file(filename)
    #play_obj = wave_obj.play()

    end = time.time()
    print(f"Time processed: {round(duration, 3)} seconds")
    print(f"Time elapsed: {round(end - start, 3)} seconds")

    if timeout_error:
        print("ERROR: Device not responding!!", flush=True)
    else:
        print("Playing sample...", flush=True)

        play_obj = sa.play_buffer(np.asarray(recieved_data).astype(np.int16)[:300000], 1, 2, fs)
        play_obj.wait_done()  # Wait until sound has finished playing

        #sd.play(np.asarray(recieved_data).astype(np.int16), fs)
        
        wavfile.write('song_output.wav', fs, np.asarray(recieved_data).astype(np.int16))
        wavfile.write('song_input.wav', fs, np.asarray(send_data).astype(np.int16))
        print("Output saved in song_output.wav", flush=True)


if __name__ == "__main__":
  LoopbackTest("COM7")