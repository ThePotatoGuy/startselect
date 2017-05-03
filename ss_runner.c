/**
 * Runner
 * @author Andre Allan Ponce
 * @email andreponce@null.net
 *
 * Launches the program
 */

//#define _WIN32_IE 0x0600

#include <windows.h>
#include <xinput.h>

// extras
/*#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC#include <shlobj.h>
*///#include <new>

//#pragma comment(lib, "comctl32.lib")

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

//#include <nfd.h>

#include "SDL2/SDL.h"

#include "ss_constants.h"
#include "ss_menu.h"
#include "ss_gamecontroller.h"

//#include <math.h> // testing

// internet told me to do this
#ifdef ___cplusplus
extern ="C"
#endif

#define MAX_STR 255

int main(int argc, char *argv[]){
/*
	nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
        
    if ( result == NFD_OKAY ) {
        puts("Success!");
        puts(outPath);
        free(outPath);
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }
*/

    // testing filedialog
    /*
    char *buff;

    const char * filters[2] = {"*.o", "*.stat"};

    buff = tinyfd_saveFileDialog("saving file", "", 2, filters,
            "test files");

    if (buff != NULL){
        printf("%s\n", buff);
    }else{
        printf("null");
    }

    return 0;
*/
    /*
    SDL_Init(SDL_INIT_GAMECONTROLLER); // Event handling init implicity using Video

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); i++){
        if (SDL_IsGameController(i)){
            controller = SDL_GameControllerOpen(i);
            if (controller){
                printf("Controller id: %i\n controler name: %s\n", i,
                        SDL_GameControllerName(controller));
            }else{
                printf("Could not open gamecontroller %i: %s\n",
                        i, SDL_GetError());
            }
        }
    }*/
/*	int res;
	unsigned char buf[256];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;


	struct hid_device_info *devs, *cur_dev;
	
	if (hid_init())
		return -1;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(0x045e, 0x028e, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
	printf("Indexed String 1: %ls\n", wstr);
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	
	// Try to read from the device. There shoud be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);

	// Send a Feature Report to the device
	buf[0] = 0x2;
	buf[1] = 0xa0;
	buf[2] = 0x0a;
	buf[3] = 0x00;
	buf[4] = 0x00;
	res = hid_send_feature_report(handle, buf, 17);
	if (res < 0) {
		printf("Unable to send a feature report.\n");
	}


	memset(buf,0,sizeof(buf));

	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls", hid_error(handle));
	}
	else {
		// Print out the returned buffer.
		printf("Feature Report\n   ");
		for (i = 0; i < res; i++)
			printf("%02hhx ", buf[i]);
		printf("\n");
	}

	memset(buf,0,sizeof(buf));

	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 17);
	if (res < 0) {
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
	}
	

	// Request state (cmd 0x81). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x81;
	hid_write(handle, buf, 17);
	if (res < 0)
		printf("Unable to write() (2)\n");
	
    // Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	long test = 0;
	res = 0;
	while (res == 0) {
		res = hid_read(handle, buf, sizeof(buf));
		if (res == 0)
			printf("waiting...\n");
		if (res < 0)
			printf("Unable to read()\n");

		while(test < 1000000){
			test++;
		}
		test = 0;
	}


	printf("Data read:\n   ");
	// Print out the returned buffer.
	for (i = 0; i < res; i++)
		printf("%02hhx ", buf[i]);
	printf("\n");

	hid_close(handle);

	/* Free static HIDAPI objects. */
//	hid_exit();



    printf("starting\n");

    /*
    XINPUT_STATE state;
    DWORD res;
    if (ss_init_gamecontroller() != SS_RETURN_SUCCESS){
        printf("bad \n");
    }else{
        int control = 0;
        while(1){
            res = XInputGetState(control, &state);
            if(res == ERROR_SUCCESS){
                ss_print_input(&(state.Gamepad));
            }
        }
    }
*/
/*
    printf("%f\n", atan2(100, 100)* 180 / 3.14159);

    printf("%f\n", atan2(0, 20)* 180 / 3.14159);
    printf("%f\n", atan2(-100, 20)* 180 / 3.14159);
    printf("%f\n", atan2(-100, -10)* 180 / 3.14159);
    printf("%f\n", atan2(100, -20)* 180 / 3.14159);
    printf("%f\n", atan2(300, -300)* 180 / 3.14159);
    */

    ss_menu_run();
    printf("ending\n");

    ss_destroy_gamecontroller();
    // for now
    //SDL_Quit();
    return 0;
}

