#include <libusb.h> // for libusb_init
#include <stdlib.h> // for exit
#include <stdio.h> // for printf
int main()
{
    libusb_device **list = NULL;
    ssize_t number_of_devices = 0;
    if(libusb_init(NULL) != 0)
    {
        fprintf(stderr, "%s:%d: unable to initialize libusb\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    number_of_devices = libusb_get_device_list(NULL, &list);
    if(number_of_devices < 0)
    {
        fprintf(stderr, "%s:%d: unable to get the list of USB devices\n", __FILE__, __LINE__);
        libusb_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("number_of_devices = %d\n", number_of_devices);
    int index = 0;
    for(index = 0; index < number_of_devices; ++index)
    {
        libusb_device *device = list[index];
        struct libusb_device_descriptor desc = {0};
        if(libusb_get_device_descriptor(device, &desc) != 0)
        {
            fprintf(stderr, "%s:%d: unable to get the USB device descriptor\n", __FILE__, __LINE__);
            libusb_free_device_list(list, 1);
            libusb_exit(NULL);
            exit(EXIT_FAILURE);
        }
        printf("%d/%d: idVendor: %04x, idProduct: %04x\n", index + 1, number_of_devices, desc.idVendor, desc.idProduct);
        /*if(desc.idVendor == 0x045e && desc.idProduct == 0x028e)*/
        if(desc.idVendor == 0x046d && desc.idProduct == 0xc077)
        {
            printf("USB device found!\n");
            printf("Opening the USB device...\n");
            libusb_device_handle *handle = NULL;
            if(libusb_open(device, &handle) != 0)
            {
                fprintf(stderr, "%s:%d: unable to open the USB device\n", __FILE__, __LINE__);
                libusb_free_device_list(list, 1);
                libusb_exit(NULL);
                exit(EXIT_FAILURE);
            }
            printf("Done!\n");
            if(libusb_kernel_driver_active(handle, 0) != 0)
            {
                printf("Kernel driver is active!\n");
                printf("Detaching the kernel driver...\n");
                if(libusb_detach_kernel_driver(handle, 0) != 0)
                {
                    fprintf(stderr, "%s:%d: unable to detach the kernel driver from the interface of the USB device\n", __FILE__, __LINE__);
                    libusb_close(handle);
                    libusb_free_device_list(list, 1);
                    libusb_exit(NULL);
                    exit(EXIT_FAILURE);
                }
                printf("Done!\n");
            }
            printf("Claiming the interface...\n");
            if(libusb_claim_interface(handle, 0) != 0)
            {
                fprintf(stderr, "%s:%d: unable to claim the interface of the USB device\n", __FILE__, __LINE__);
                libusb_close(handle);
                libusb_free_device_list(list, 1);
                libusb_exit(NULL);
                exit(EXIT_FAILURE);
            }
            printf("Done!\n");
            
            unsigned char data[4];
            int actual_length;
            printf("Reading data...\n");
            int counter = 0;
            while(counter < 100)
            {
                /*int r = libusb_bulk_transfer(handle, LIBUSB_ENDPOINT_IN, data, sizeof(data), &actual_length, 0);*/
                int r = libusb_interrupt_transfer(handle, 0x81, data, sizeof(data), &actual_length, 10);
                if(r == 0)
                {
                    printf("Data: %d %d %d %d\n", data[0], data[1], data[2] , data[3]);
                    counter ++;
                }
                else
                {
                    printf("%d\n", r);
                }
                    
            }
            printf("Releasing the interface...\n");
            if(libusb_release_interface(handle, 0) != 0)
            {
                fprintf(stderr, "%s:%d: unable to detach the kernel driver from the interface of the USB device\n", __FILE__, __LINE__);
                libusb_close(handle);
                libusb_free_device_list(list, 1);
                libusb_exit(NULL);
                exit(EXIT_FAILURE);
            }
            printf("Done!\n");
            printf("Closing the USB device...\n");
            libusb_close(handle);
            printf("Done!\n");
        }
    }
    libusb_free_device_list(list, 1);
    libusb_exit(NULL);
    exit(EXIT_SUCCESS);
}
