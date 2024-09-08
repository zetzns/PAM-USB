#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USB_VENDOR_ID 0x0000
#define USB_PRODUCT_ID 0x0000

int checkUSBdevices(void) {
    /*
        Checks if OUR DEVICE already in system
    */
    libusb_device **devs;
    libusb_context *ctx = NULL;
    struct libusb_device_descriptor desc;
   
    int r = libusb_init(&ctx);
    if (r < 0) {
        fprintf(stderr, "[-] libusb_init failed\n");
        return 0;
    }

    ssize_t cnt;
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "[-] libusb_get_device_list failed\n");
        libusb_exit(ctx);
        return 0;
    }

    int device;
    for (int i = 0; i < cnt; i++)  {
        device = libusb_get_device_descriptor(devs[i], &desc);
        if (device < 0) {
            fprintf(stderr, "[-] libusb_get_device_descriptor failed\n");
            continue;
        }
        if (desc.idProduct == USB_PRODUCT_ID && desc.idVendor == USB_VENDOR_ID) {
            libusb_free_device_list(devs, 1);
            libusb_exit(ctx);
            return 1;
        }
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
} 


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char** argv) {
    /*
        Authenticate if its possible
    */
    const char *user = NULL;
    int retval = pam_get_user(pamh, &user, "Username: ");
    if (retval != PAM_SUCCESS) {
        fprintf(stderr, "[-] ERROR GETTING USERNAME: %s\n", pam_strerror(pamh, retval));
        return retval;
    }

    if (user == NULL) {
        fprintf(stderr, "[-] MISSING USERNAME\n");
        return PAM_AUTH_ERR;
    }

    if (strcmp(user, "zn") == 0 && checkUSBdevices()) {
        return PAM_SUCCESS;
    } else {
        fprintf(stderr, "[-] AUTH ERROR: %s\n", user);
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
