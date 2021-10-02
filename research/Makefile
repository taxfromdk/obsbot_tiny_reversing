DOCKERTAG=mydockerimage
#NETWORK=-p 9001:9001/udp
MOUNT=-v $(shell pwd):/src
USER=-u $(shell id -u):$(shell id -g)
USB=--device=/dev/bus/usb/009/004

builddocker:
	docker build -t $(DOCKERTAG) docker 

interactive:
	docker run $(MOUNT) $(USER) -it $(DOCKERTAG) 

replay.py: captures/2.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --vid 0x6130 --pid 0xfef0 --sleep --wrapper --comment --setup -p $< > replay.py
	
clean:
	rm -rf replay.py

dockerprune:
	docker system prune --all -f


.PHONY: clean interactive builddocker
