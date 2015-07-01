#!/usr/bin/python2

import sys
import subprocess
import getopt
import time
import socket
import os

lcd_full_path = "/root/cosytech-cosyperf/lcd"

def lcd_output(lcdpath, str1, str2):
    p = subprocess.Popen([lcdpath, str1, str2], stdout=subprocess.PIPE)
    #print(p.communicate())

def main(argv=None):
    print("Bandwidth Monitor started")
    lcd_output(lcd_full_path, "PERFORMING TEST ", "PLEASE WAIT.....")
    time.sleep(4) 
    lcd_output(lcd_full_path, "UPLOADING DATA  ", "PLEASE WAIT.....")
    time.sleep(4)
    my_ip = socket.gethostbyname(socket.gethostname())
    measured_specs = "D0923 U0942 L001"
    lcd_output(lcd_full_path, my_ip, measured_specs)
    time.sleep(10) 
    lcd_output(lcd_full_path, "DONE", "SHUTTINGDOWN")
    time.sleep(1) 
    os.system("shutdown now -h")
    
if __name__ == "__main__":
    sys.exit(main())
