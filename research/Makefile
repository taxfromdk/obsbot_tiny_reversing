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


explore: up.py down.py left.py right.py ai_on_off.py handshake.py manipulatefromoutside.py reset.py zoom.py up_down_left_right.py idle.py idle2.py

up.py: captures/up.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
down.py: captures/up.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
left.py: captures/up.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
right.py: captures/up.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
ai_on_off.py: captures/ai_on_off.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
handshake.py: captures/handshake.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
manipulatefromoutside.py: captures/manipulatefromoutside.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
reset.py: captures/reset.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
zoom.py: captures/zoom.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
up_down_left_right.py: captures/up_down_left_right.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
idle.py: captures/idle.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@
idle2.py: captures/idle2.pcapng
	docker run $(MOUNT) $(USER) --rm $(DOCKERTAG) \
	usbrply --device-hi --sleep --wrapper --comment --setup -p $< > $@



clean:
	rm -rf replay.py \
	up.py \
	down.py \
	left.py \
	right.py \
	

dockerprune:
	docker system prune --all -f


.PHONY: clean interactive builddocker explore
