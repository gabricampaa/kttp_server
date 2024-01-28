# Makefile

.PHONY: install clean

install: welcome_script check_root check_gcc create_directories properly_installed moveFiles compile serviceSetup printInfo

welcome_script:
	@echo "Welcome to the installation process for the kttp_server!\n=====REMEMBER, YOU MUST BE SUDO!=====";
	@echo "This software will create folders in the /etc, /usr and /var directories (for a complete list of what's gonna happen, check the make file with cat Makefile)."
	@echo "\nAre you sure you want to proceed [y/n]: "
	@read answer; \
	if [ "$$answer" != "y" ]; then \
		echo "Installation aborted."; \
		exit 1; \
	fi

properly_installed:
	@echo "All the dirs have been properly installed"

check_root:
	@if [ $$(id -u) -ne 0 ]; then \
	        @echo "You must be root to run this target. Please use sudo." >&2; \
	        exit 1; \
	    fi

check_gcc:
	@if ! command -v gcc >/dev/null 2>&1; then \
        echo "GCC is not installed. Attempting to install..."; \
        if command -v apt-get >/dev/null 2>&1; then \
            sudo apt-get update; \
            sudo apt-get install -y gcc; \
        elif command -v yum >/dev/null 2>&1; then \
            sudo yum install -y gcc; \
        else \
            echo "Error: Unsupported package manager. Please install GCC manually."; \
            exit 1; \
        fi; \
        if ! command -v gcc >/dev/null 2>&1; then \
            echo "Error: Unable to install GCC. Please install it manually."; \
            exit 1; \
        fi; \
        echo "GCC has been installed successfully."; \
    else \
        echo "GCC is already installed."; \
    fi

create_directories: create_html_directory create_src_directory create_log_directory create_conf_directory create_kttp_log


create_kttp_log:
	@kttp_log_dir="/var/log/kttp_log/"; \
	sudo mkdir -p "$$kttp_log_dir"; \
        


create_html_directory:
	@html_directory="/var/kttp_server_files/html_docs/"; \
    if [ -d "$$html_directory" ]; then \
        echo "Directory '$$html_directory' already exists."; \
    else \
        sudo mkdir -p "$$html_directory"; \
        if [ $$? -eq 0 ]; then \
            echo "Directory '$$html_directory' created successfully."; \
        else \
            echo "Error: Unable to create directory '$$html_directory'."; \
            exit 1; \
        fi; \
    fi

create_src_directory:
	@src_directory="/usr/lib/kttp_server_src/"; \
    if [ -d "$$src_directory" ]; then \
        echo "Directory '$$src_directory' already exists."; \
    else \
        sudo mkdir -p "$$src_directory"; \
        if [ $$? -eq 0 ]; then \
            echo "Directory '$$src_directory' created successfully."; \
        else \
            echo "Error: Unable to create directory '$$src_directory'."; \
            exit 1; \
        fi; \
    fi

create_log_directory:
	@log_directory="/var/log/kttp_server/"; \
    if [ -d "$$log_directory" ]; then \
        echo "Directory '$$log_directory' already exists."; \
    else \
        sudo mkdir -p "$$log_directory"; \
        if [ $$? -eq 0 ]; then \
            echo "Directory '$$log_directory' created successfully."; \
        else \
            echo "Error: Unable to create directory '$$log_directory'."; \
            exit 1; \
        fi; \
    fi

create_conf_directory:
	@conf_directory="/etc/kttp_server/CONFs/"; \
    if [ -d "$$conf_directory" ]; then \
        echo "Directory '$$conf_directory' already exists."; \
    else \
        sudo mkdir -p "$$conf_directory"; \
        if [ $$? -eq 0 ]; then \
            echo "Directory '$$conf_directory' created successfully."; \
        else \
            echo "Error: Unable to create directory '$$conf_directory'."; \
            exit 1; \
        fi; \
    fi

clean:
	@echo "Removing directories..."
	@sudo rm -rf /var/kttp_server_files/ /usr/lib/kttp_server_src/ /var/log/kttp_server/ /etc/kttp_server/ /var/log/kttp_log/ 
	@sudo rm /etc/systemd/system/kttp_server.service
	@echo "All directories removed."


moveFiles:
	@echo "Moving Files....\n"
	@mv "src/SERVER/" "/usr/lib/kttp_server_src/"
	@mv "src/LOG/" "/usr/lib/kttp_server_src/"
	@mv "src/CONF/userconf.ini" "/etc/kttp_server/CONFs/"
	@mv "src/CONF/" "/usr/lib/kttp_server_src/"
	@mv "html/*" "/var/kttp_server_files/html_docs/"
	@mv "/var/kttp_server_files/html_docs/html/" "/var/kttp_server_files/html_docs/"


compile:
	@cd "/usr/lib/kttp_server_src/SERVER"; \
	sudo chmod +x "compila"; \
	sudo "./compila"

serviceSetup:
	@mv "service_setup/kttp_server.service" "/etc/systemd/system/"
	@sudo systemctl daemon-reload
	@sudo systemctl start kttp_server.service
	@sudo systemctl enable kttp_server.service

ip_address=$(hostname -I)

printInfo:
	@echo "The service is up and runnings. Visit $$ip_address to see the result!\n"
	@echo "===== PLEASE NOTE =====\n"
	@echo "The service is running as sudo on port 80. Read the documentation for all the info..."


uninstall: check_root clean unistall_comand

unistall_comand:
	@echo "Uninstalling the kttp_server..."
	@sudo systemctl stop kttp_server
	@sudo systemctl daemon-reload
