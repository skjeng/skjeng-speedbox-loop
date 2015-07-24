#!/usr/bin/python2
#as aMain
import sys
import subprocess
import getopt
import time
import socket
import os
import numpy
import struct
import requests

lcd_script = "lcd_program"
update_script = "call_update.sh"
upload_script = "upload.sh"
download_script = "download.sh"
check_github_script = "check_github.sh"

#target_ip = 'speedtest.hydracloud.no'
target_ip = '127.0.0.1'
results_ip = 'bayonette.royrvik.org'
results_port = 80

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
        print "OSError({0}): {1}".format(e.errno, e.strerror)
        raise
    except:
        print "Unexpected error:", sys.exc_info()[0]
        raise
    return msg[0]


def lcd_output(lcdpath, str1="", str2=""):
    p = subprocess.Popen([lcdpath, str1.ljust(16), str2.ljust(16)], stdout=subprocess.PIPE)
    #print(p.communicate())

def check_github():
    msg = popen(check_github_script)
    
    if 'up-to-date\n' == msg:
        print("check_github()=up to date")
        return 0
    if b'req pull\n' == msg:
        print("check_github()=need pull")
        return 1
    if b'req push\n' == msg:
        print("check_github()=need push, this is wrong")
        return 2
    else: 
        print("check_github()=critical fault")
        return -1

def update(update_path):
    f.write('update run start')
    p = subprocess.Popen([update_path], stdout=subprocess.PIPE)
    f.write('update run after')

def iperf(checkpath, ip):
    p = subprocess.Popen([checkpath, ip], stdout=subprocess.PIPE)
    output = p.communicate()[0]
    print(len(output))
    if len(output) == 0:
        time.sleep(1)
        print(output)
        lcd_output(lcd_full_path,  "SCRIPT ERROR", "iperf fail")
        time.sleep(2)
        lcd_output(lcd_full_path,  "rebooting", "now")
        os.system("reboot")
    out = output.splitlines()
    print(out)
    out_int = []
    for item in out:
        out_int.append(float(item))
    return numpy.mean(out_int)

def lcd_button(lcdpath):
    p = subprocess.Popen([lcdpath], stdout=subprocess.PIPE)
    msg = p.communicate()
    if b'BUTTON1' == msg[0]:
        return 1
    if b'BUTTON2' == msg[0]:
        return 2
    return 0

def upload_results(up,down,lat, ip, port):
    data = {'up':str(up), 'down':str(down), 'latency':lat}
    r = requests.post('http://'+str(ip)+':'+str(port), data)
    print(r.text)

def quitloop():
    f.write('quitting loop')
    f.close()
    print("Quitting loop")
    time.sleep(1)
    lcd_output(lcd_full_path,  "quitting", "loop")
    time.sleep(1)
    exit()

def main(argv=None):
    #lcd_output(lcd_full_path, "Speedbox", "Running")
    time.sleep(1)
    i = 0
    while True:
        my_ip = socket.gethostbyname(socket.gethostname())
        measured_specs = str(i) + 'seconds'
        print (cwd)
        check_github()
        exit()

        lcd_output(lcd_full_path,  measured_specs, my_ip)
        time.sleep(1)
        i = i + 1
        if lcd_button(lcd_full_path) == 1:
            print ("Button 1 was pushed, aborting python script")
            lcd_output(lcd_full_path,  "BTN1: aborted", my_ip)
            time.sleep(1)
            quitloop()

        if '127.0.0.' not in my_ip:
            lcd_output(lcd_full_path,  "Got full IP", my_ip)
            time.sleep(1)
            lcd_output(lcd_full_path,  "checking for", "new version") 
            if lcd_button(lcd_full_path) == 2:
                print ("Button 2 was pushed, updating python script")
                lcd_output(lcd_full_path,  "BTN2:", "skip sw check") 
            else:
                state = check_github()
                if state == 0:
                    time.sleep(1)
                    lcd_output(lcd_full_path,  "Up to date", "no action") 
                    time.sleep(1)
                elif state == 1:
                    time.sleep(1)
                    lcd_output(lcd_full_path,  "Pull required", "updating") 
                    time.sleep(1)
                    update(update_full_path)
                    time.sleep(1)
                    lcd_output(lcd_full_path,  "Quitting", "loop") 
                    time.sleep(1)
                    quitloop()
                else:
                    time.sleep(1)
                    lcd_output(lcd_full_path,  "GITHUB ERROR", "Call 91752214") 
                    time.sleep(10)
                    quitloop()
            time.sleep(1)
            print('LCD'+str(lcd_button(lcd_full_path))) 
            lcd_output(lcd_full_path, "Testing up", "PLEASE WAIT.....")
            mean_upload = int(numpy.around(iperf(upload_full_path, target_ip)))
            lcd_output(lcd_full_path, "Testing down", "PLEASE WAIT.....")
            mean_download = int(numpy.around(iperf(download_full_path, target_ip)))
            lcd_output(lcd_full_path, "Testing lat", "PLEASE WAIT.....")
            status = 'U'+str(mean_upload)+' D'+str(mean_download)
            time.sleep(1)
            lcd_output(lcd_full_path, "Testresults", status)
            time.sleep(10)
            upload_results(mean_upload, mean_download, 1, results_ip, results_port)
            time.sleep(1)
            lcd_output(lcd_full_path, "Uploading results", "to server")
            time.sleep(1)
            lcd_output(lcd_full_path,  "SHUT", "DOWN")
            time.sleep(1)
            if lcd_button(lcd_full_path) == 1:
                print ("Button 1 was pushed, aborting python script")
                lcd_output(lcd_full_path,  "SCRIPT ABORTED", my_ip)
                time.sleep(1)
                quitloop()
            #os.system("shutdown now -h")

if __name__ == "__main__":
    sys.exit(main())
