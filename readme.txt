This is a research project where I have been trying to figure out how to operate the Obsbot-Tiny camera from Linux.

https://www.obsbot.com/obsbot-tiny

My intention is to share this code in the hopes of triggering people more gifted than I, and thereby helping a 
Linux driver be developed for the device.

Some disassembly has revealed that the camera is based on the HiSilicon Hi3516CV500

One of the testpads seem to output serial 115200,8,N,1 serial. But I have not seen any u-boot.

The chip seems capable of running the AI part on the device.

The camera seems to be running without step-motors and it seems to have feedback, so it knows where the camera is pointing.

On curious note. When I did the traffic captures I used a laptop, as my main PC is virtualized and cant run virtualbox. 
On that laptop the device popped up as two devices. (lsusb)

#Bus 001 Device 004: ID 13d3:56f9 IMC Networks OBSBOT Tiny
#Bus 001 Device 005: ID 6e30:fef0 Remo Tech Co., Ltd. OBSBOT Tiny

And on my regular virtualized Ubuntu with an usb controller passed through, it reveals itself like this.

#Bus 009 Device 002: ID 6e30:fef0

I suspect that the laptop use USB3.0 and the device behaves differently on USB2.0 and 3.0 busses. 

Beware this is work in progress and is currently very limited in functionality. 

Demo can be seen here: https://www.youtube.com/watch?v=a2n7L8eY6U8

Features above the line are supported. Things below are todo.

*) PTZ movement left, right, up, down and stop. (replay of commands)
*) go to home
-----------------------------
*) Understand PTC protocol
*) Fine grained camera control
*) Read current position
*) Start/stop AI (I suspect the HiSilicon is doing the AI)
*) Zoom
*) Ability to use camera while controlling
*) Figure out to fetch images or use alongside webcam app.
*) 


The current functionality is achieved based on USB traffic captured from a virtual machine with the USB 
camera connected via passthrough. These commands are essentially just replayed.

The replay of commands reveal some "fun" behaviour, as left, right, up and down does not move clean and the speed 
is probably different. Understanding of the protocol is needed.

The communication between the device and host also includes read commands, and this protocol is also needs to be nderstood. 
Hopefully it would contain current settings.

I have used Virtualbox installed on an Ubuntu 20.04. The virtualbox machine is running Windows 10 and the 
official utility for windows.

The capture process worked as follows.

1) Identify the device using lsusb (6e30:fef0)
#lsusb
Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 003 Device 002: ID 1358:c123 Realtek Bluetooth Radio
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 004: ID 13d3:56f9 IMC Networks OBSBOT Tiny
Bus 001 Device 003: ID 27c6:5110 Shenzhen Goodix Technology Co.,Ltd. 
Bus 001 Device 002: ID 05e3:0610 Genesys Logic, Inc. 4-port hub
Bus 001 Device 005: ID 6e30:fef0 Remo Tech Co., Ltd. OBSBOT Tiny
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub


2) Identify the UUID of the device in this list
#VBoxManage list usbhost

Host USB Devices:

UUID:               eb48482b-cba1-4c69-b920-bb4f5a44b128
VendorId:           0x6e30 (6E30)
ProductId:          0xfef0 (FEF0)
Revision:           4.9 (0409)
Port:               1
USB version/speed:  2/High
Manufacturer:       Remo Tech Co., Ltd.
Product:            OBSBOT Tiny
Address:            sysfs:/sys/devices/pci0000:00/0000:00:08.1/0000:03:00.3/usb1/1-2//device:/dev/vboxusb/001/005
Current State:      Busy

3) Start a virtualbox with windows 10 and the obsbot software (note the vm name)

4) enable usb monitoring on the Ubuntu machine
#sudo modprobe usbmon
#sudo setfacl -m u:jesper:r /dev/usbmon*

5) open wireshark and capture the first usbmon device

6) Passthrough the device using
#VBoxManage controlvm Win10_obsbot usbattach eb48482b-cba1-4c69-b920-bb4f5a44b128

*Notice that the VBoxManage command can take a capturefile argument. (I used this method initially, but I was unable to parse these files with the usbrply tool.)

7) Exercise the camera. To record commands.

8) Disconnect the camera
#VBoxManage controlvm Win10_obsbot usbdetach 26e812f4-663d-41d9-9d5c-f772584a981f

9) Save the capture as .pcapng

10) The capture can be converted into libusb commands using (https://github.com/JohnDMcMaster/usbrply.git inside a docker environment)
#make replay.py

*I ended up using a docker environment for the replay as the project was picky about python versions and other dependencies.

11) replay.py contains the sequence of commands 
*Notice you have to edit the camera VID and PID before running

*and possible detach the driver before you can be allowed to write to the device

#Possible detach driver
if dev.kernelDriverActive(0):
    print("detaching", 0)
    dev.detachKernelDriver(0)

*when running, some commands result in error. Try uncommenting and also reduce largest sleeps to speed up things

*When the commands can be sent a rplay can be seen.

12) Commands can be lifted form this replay file to identify camera commands. 

Finally the working commands can be used inside an alternative camera UI. 

I use PyGame as i find it an easy tool for interaction and visual feedback. Beware there is no image yet.

#sudo ./tiny.py



