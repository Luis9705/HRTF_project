
import serial
from scipy.io import wavfile
import numpy as  np
import sys
import time
import simpleaudio as sa
import sounddevice as sd
import threading
import queue
import pyaudio #pip install pipwin, #pipwin install pyaudio
import wave
from os import path

start_time = 0
duration = 30
BLOCK_SIZE = 640
baudrate = 1500000

def get_file_path(folder, file_name):
    basepath = path.dirname(__file__)
    filepath = path.abspath(path.join(basepath, "..", folder, file_name))
    return filepath



fs, data = wavfile.read(get_file_path('audio', 'tusa.wav'))
if len(data.shape) == 2:
    right_channel, left_channel = data[:,1], data[:,0]
    mono_data = data.sum(axis=1) / 2
    mono_data = mono_data.astype(np.int16)
else:
    mono_data = data

t = np.linspace(0, len(data) / fs, num=len(data))


p = pyaudio.PyAudio()

stream = p.open(format=p.get_format_from_width(2),
                channels=1,
                rate=fs,
                output=True)



send_data = []
recieved_data_right = []
recieved_data_left = []

comPort = None

recieve_flag = True
timeout_error = False

q = queue.Queue()

#producer-consumer sincronization
empty_lock = threading.Lock()
filled_lock = threading.Lock()

# def PlayAudioThread():
#     global recieve_flag
#     while True:
#         data = q.get()
#         if not recieve_flag:
#             break
#         #stream.write(data)
#     print("Play finished", flush=True)


def TransmitThread():
    global recieve_flag
    global comPort
    global send_data
    global timeout_error
    current_percent = 0
    if comPort:
        print("Starting Transmission...", flush=True)
        #cropping the song to the part we want to process
        data_list = mono_data[np.where((t > start_time) &( t < start_time + duration))[0]].tolist()
        #spliting the data in blocks
        data_blocks = [data_list[x:x+BLOCK_SIZE] for x in range(0, len(data_list),BLOCK_SIZE)]
        #sending each block
        for i, data_block in enumerate(data_blocks):
            if len(data_block) <=  BLOCK_SIZE:
                #doing zero padding
                data_block += [0] * (BLOCK_SIZE - len(data_block))

            empty_lock.acquire() ################################

            #converting the integers to bytes in big endian order
            send_frame = b""
            for sample in data_block:
                send_frame+= sample.to_bytes(2, 'big', signed=True)
                send_data.append(sample)  
            #sending the bytes to the serial port
            comPort.write(send_frame)
            #calculating the amount of data trasfered
            percentage = (len(data_block)*i)/len(data_list) * 100
            if percentage >= current_percent:
                print(f"{current_percent}% processed", flush=True)
                current_percent+= 5
            if timeout_error:
                break

            filled_lock.release() #################################
                
        time.sleep(0.01)
        #telling the recieve thread to finish its execution
        recieve_flag = False
        print("Transmission finished!", flush=True)

def ReceiveThread():
    global recieve_flag
    global recieved_data_right
    global recieved_data_left
    global timeout_error
    global comPort
    while True:  

        filled_lock.acquire() ##########################################

        recived_frame = []
        #reading the serial port
        recieved = comPort.read(BLOCK_SIZE*2*2 + 5)
        if not recieved:
            timeout_error = True
            break
        for c in recieved:
            recived_frame.append(c)
        #cropping the header and the terminator
        bytes_list = recived_frame[3:-2]
        #spliting the bytes in groups of two
        composite_list = [bytes_list[x:x+2] for x in range(0, len(bytes_list),2)]
        #converting the bytes in big endian order to integers
        recieved_bytes = [int.from_bytes(bytearray(byte), byteorder='big', signed=True) for byte in composite_list]
        #apending the integers recieved to the output array to be saved
        recieved_bytes_right = recieved_bytes[:BLOCK_SIZE]
        recieved_bytes_left = recieved_bytes[BLOCK_SIZE:]
        recieved_data_right += recieved_bytes_right
        recieved_data_left += recieved_bytes_left

        #errors = list(filter(lambda x: x[0]*2 != x[1], zip(send_data, recieved_data)))
        #print("SEND DATA: ", len(send_data), "RECIEVE DATA: ", len(recieved_data), flush=True)
        #print("ERRORS: ", len(errors), flush=True)

        #converting from big to little indian to reproduce in speaker
        # data_temp = b""
        # for sample in recieved_bytes:
        #     data_temp+= sample.to_bytes(2, 'little', signed=True)
        # #sending data to reproduce in speaker
        # q.put(data_temp)

        empty_lock.release()##########################################

        if not recieve_flag:
            break
    #sending a last dummy data to force the playAudioThread to exit
    q.put(b"")
    print("Reception finished!", flush=True)


def is_any_thread_alive(threads):
    return True in [t.is_alive() for t in threads]

def LoopbackTest(comPortName):
    global comPort
    start = time.time()

    try:
        wavfile.write(get_file_path('output', 'song_output.wav'), fs, np.zeros(5).astype(np.int16))
        wavfile.write(get_file_path('output', 'song_input.wav'), fs, np.zeros(5).astype(np.int16))
    except:
        print("Please close your current audio players and try again", flush=True)
        return

    print("Opening port...", flush=True)
    comPort = serial.Serial \
            (
              port=comPortName,
              baudrate=baudrate,
              parity=serial.PARITY_NONE,
              stopbits=serial.STOPBITS_ONE,
              bytesize=serial.EIGHTBITS,
              timeout = 2
            )

    transmit_thread = threading.Thread(target=TransmitThread, daemon=True)
    receive_thread = threading.Thread(target=ReceiveThread, daemon=True)
    #play_audio_thread = threading.Thread(target=PlayAudioThread, daemon=True)

    transmit_thread.start()
    receive_thread.start()
    #play_audio_thread.start()


    try:
        while is_any_thread_alive([transmit_thread,receive_thread]):
            time.sleep(0)
        
        comPort.close()
        stream.stop_stream()
        stream.close()
        # close PyAudio (5)
        p.terminate()

        end = time.time()
        print(f"Time processed: {round(duration, 3)} seconds")
        print(f"Time elapsed: {round(end - start, 3)} seconds")
        if timeout_error:
            print("ERROR: Device not responding!!", flush=True)
        else:
            # print("Playing sample...", flush=True)

            # play_obj = sa.play_buffer(np.asarray(recieved_data).astype(np.int16)[:300000], 1, 2, fs)
            # play_obj.wait_done()  # Wait until sound has finished playing

            #sd.play(np.asarray(recieved_data).astype(np.int16), fs)
            

            recieved_data_stereo = np.vstack((recieved_data_left, recieved_data_right))
            recieved_data_stereo = recieved_data_stereo.transpose()
            wavfile.write(get_file_path('output', 'song_output.wav'), fs, np.asarray(recieved_data_stereo).astype(np.int16))
            wavfile.write(get_file_path('output', 'song_input.wav'), fs, np.asarray(send_data).astype(np.int16))
            print("Output saved in song_output.wav", flush=True)

    except KeyboardInterrupt:

        comPort.close()
        stream.stop_stream()
        stream.close()
        # close PyAudio (5)
        p.terminate()

        print("Exiting program...", flush = True)

   
    # filename = 'song_output.wav'
    # wave_obj = sa.WaveObject.from_wave_file(filename)
    #play_obj = wave_obj.play()

if __name__ == "__main__":
    
  LoopbackTest("COM7")