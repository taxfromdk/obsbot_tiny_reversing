FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive

# Installing dependencies for linux installation of opencv
RUN apt-get update
RUN apt-get install -y git python3 python3-pip nano
RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1
RUN pip3 install python-pcapng libusb1 pyusb 
RUN git clone https://github.com/JohnDMcMaster/usbrply.git
RUN cd usbrply; python3 setup.py install
RUN apt-get install -y mc usbutils
WORKDIR src

