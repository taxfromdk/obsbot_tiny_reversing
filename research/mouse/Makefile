mouse: mouse.cpp
	g++ $< -o mouse `pkg-config --libs --cflags libusb-1.0`

clean:
	rm -rf mouse

test: mouse
	./mouse


.PHONY: test clean