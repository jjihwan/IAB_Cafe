import cv2
import numpy as np
import serial
import requests

ser = serial.Serial("COM10", 115200, timeout=1)
font = cv2.FONT_HERSHEY_SIMPLEX
font_scale = 3
font_color = (0, 0, 0)
font_thickness = 5

pager_ip_1 = '172.30.1.54' 
pager_ip_2 = '172.30.1.56'

base_url_1 = 'http://' + pager_ip_1
base_url_2 = 'http://' + pager_ip_2

data_to_send = ""

def retrieveData(prev_data):
    ser.write(b'1')
    data=prev_data
    while ser.in_waiting:
        data = ser.readline().decode('ascii')
    ser.write(b'0')
    return data


def display(data):
    background = np.ones((600, 1000, 3), dtype=np.uint8) * 255
    ref = [250,330]
    height_diff = 150
    cv2.putText(background, "1: {}".format("ready" if data[0] else "preparing"), (ref[0], ref[1]-height_diff), font, font_scale, font_color, font_thickness)
    cv2.putText(background, "2: {}".format("ready" if data[1] else "preparing"), (ref[0], ref[1]), font, font_scale, font_color, font_thickness)
    cv2.putText(background, "3: {}".format("ready" if data[2] else "preparing"), (ref[0], ref[1]+height_diff), font, font_scale, font_color, font_thickness)
    cv2.imshow("Decorated Scores", background)

    key=cv2.waitKey(50)
    return key

def pager_controller(data):
    endpoint_1 = "/True" if data[0] else "/False"
    endpoint_2 = "/True" if data[1] else "/False"

    response_1 = requests.post(base_url_1+endpoint_1, data=data_to_send)
    response_2 = requests.post(base_url_2+endpoint_2, data=data_to_send)

    if response_1.status_code == 200 and response_2.status_code == 200:
        print('Data sent successfully')
    else:
        print('Failed to send data')


if __name__ == "__main__":
    data="000"
    while True:
        data = retrieveData(data)
        if "\n" in data:
            data= data[:-1]    
        data = [bool(int(i)) for i in data]

        key=display(data)
        if key==ord('q'):
            break
        
        pager_controller(data)