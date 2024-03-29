# The KTTP Server - v0.4


The KTTP Server is a simple http server made by me, a hobbist programmer. I like C and i like networking, so... here we are!

## Current features

- Supports GET requests for static pages
- Supports HEAD requests
- Supports Multiple connections


## Installation


To install the kttp_server, run

```sh
git clone https://github.com/gabricampaa/kttp_server.git
cd kttp_server
sudo make install
#optional 
sudo reboot
```

Now if you go and visit the public ip of your machine, you should see the index page.

## Uninstall
To uninstall the kttp_server
```sh
cd /usr/lib/kttp_server_src/
sudo make uninstall
```

## Documentation

>PLEASE NOTE: this is a hobby project. It's not production ready, it's not safe, it's not optimized.

- This is a list of all the folders that will be created:
  ```sh
    /usr/lib/kttp_server_src/ #src files and the executables 
    /etc/kttp_server/CONFs/  #config files
    /var/kttp_server_files/html_docs/ #default html docs folder
    /var/log/kttp_log #contains the log

    ```
- Under the folder 
    ```sh
    /etc/systemd/system/
    ```
    will be created the service file, kttp_server.service
    
### Customization
- The customization file is located here: 
  ```sh
    /etc/kttp_server/CONFs/ 
    ```
    you can choose on what port you want the server to be running and which one is the default folder for html docs. DO NOT MODIFY what's left of the '=' sign, things might go wrong.

## License

MIT

**Free Software, Hell Yeah!**


## What changed in v0.4   
- better logging
- memory leaking fixed (kinda)
- see issues
