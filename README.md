# PAM-USB

Этот репозиторий содержит реализацию PAM (Pluggable Authentication Module) модуля, который позволяет выполнять аутентификацию пользователей на основе подключения USB-устройства с определённым Vendor ID и Product ID.

## Использование

(В исходном коде необходимо указать VENDOR_ID и PRODUCT_ID своего устройства)

```bash
gcc -fPIC -fno-stack-protector -c pam_usb.c -o pam_usb.o
gcc -shared -o pam_usb.so pam_usb.o -lpam -lusb-1.0
sudo cp pam_usb.so /lib/security/
```
После компиляции необходимо добавить этот модуль в конфиги системы.

```bash
sudo nano /etc/pam.d/system-auth
sudo nano /etc/pam.d/kde
sudo nano /etc/pam.d/login
```

Во все эти файлы (в моём случае (gentoo linux на kde)) вписываем строчку

```plaintext
auth       sufficient     pam_usb.so
```

теперь просто 

```bash
sudo reboot
```
