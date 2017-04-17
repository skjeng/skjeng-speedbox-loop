#!/usr/bin/python3
import sys
import subprocess
import getopt
import time
import socket
import os
import numpy
import struct
import requests

lcd_script = "lcd.out"
upload_script = "upload.sh"
download_script = "download.sh"

upload_full_path = ""
download_full_path = ""
lcd_full_path = ""

target_ip = '192.168.2.1'
results_ip = '192.168.2.1'
results_port = 8080

f = open('logfile.txt', 'a')

cwd = os.getcwd()
print(cwd)

def popen( program, *arg ):
    popen_target_list = [ cwd+'/'+program ]
    msg = ""
    for argument in arg:
        popen_target_list.append( argument )
    try:
        p = subprocess.Popen( popen_target_list, stdout=subprocess.PIPE )
        msg = p.communicate()
    except OSError as e:
        #print "OSError({0}): {1}".format(e.errno, e.strerror)
        raise
    except:
        #print "Unexpected error:", sys.exc_info()[0]
        raise
    return msg[0]

def lcd_output(str1="", str2=""):
    msg = popen(lcd_script, str1.ljust(16), str2.ljust(16))

def upload_results(up,down,lat, ip, port):
    data = {'up':str(up), 'down':str(down), 'latency':lat}
    r = requests.post('http://'+str(ip)+':'+str(port), data)
    print(r.text)

def iperf(checkpath, ip):
    p = subprocess.Popen([checkpath, ip], stdout=subprocess.PIPE)
    output = p.communicate()[0]
    print(len(output))
    if len(output) == 0:
        print(output)
        lcd_output("ERROR", "iperf failed")
    out = output.splitlines()
    print(out)
    out_int = []
    for item in out:
        out_int.append(float(item))
    return numpy.mean(out_int)

def errorShutdown():
    time.sleep(1)
    lcd_output("Error", "Shutting down...")
    time.sleep(1)
    lcd_output("", "")
    exit()

def main(argv=None):
    lcd_output("Speedbox", "Running")
    time.sleep(1)
    i = 0
    while True:
        my_ip = socket.gethostbyname(socket.gethostname())
        measured_specs = str(i) + 's waiting'
        print (cwd)

        upload_full_path = cwd + '/' + upload_script
        download_full_path = cwd + '/' + download_script
        lcd_full_path = cwd + '/' + lcd_script
        
        lcd_output(measured_specs, my_ip)
        time.sleep(1)
        i = i + 1

        if '127.0.0.' not in my_ip:
            lcd_output("Got full IP", my_ip)
            time.sleep(1)
            lcd_output("iperf server", target_ip)
            time.sleep(1)
            lcd_output("http server", results_ip)
            time.sleep(1)
            lcd_output("Testing up", "PLEASE WAIT.....")
            try:
                mean_upload = int(numpy.around(iperf(upload_full_path, target_ip)))
                break
            except ValueError:
                lcd_output("UP:ValueError", "Not a number")
                errorShutdown()
            
            lcd_output("Testing down", "PLEASE WAIT.....")
            try:
                mean_download = int(numpy.around(iperf(download_full_path, target_ip)))
                break
            except ValueError:
                lcd_output("DOWN:ValueError", "Not a number")
                errorShutdown()            
            status = 'U'+str(mean_upload)+' D'+str(mean_download)
            time.sleep(1)
            lcd_output("Testresults", status)
            time.sleep(10)
            upload_results(mean_upload, mean_download, 1, results_ip, results_port)
            time.sleep(1)
            lcd_output("Uploading results", "to server")
            time.sleep(1)

if __name__ == "__main__":
    sys.exit(main())
