# Makefile

.PHONY: install clean

install: welcome_script check_root check_gcc create_directories  moveFiles compile serviceSetup printInfo

welcome_script:
	@echo "Welcome to the installation process for the kttp_server!\n=====REMEMBER, YOU MUST BE SUDO!=====";
	@echo "This software will create folders in the /etc, /usr and /var directories (for a complete list of what's gonna happen, check the documentation)."
	@echo "\nAre you sure you want to proceed [y/n]: "
	@read answer; \
	if [ "$$answer" != "y" ]; then \
		echo "Installation aborted."; \
		exit 1; \
	fi

check_root:
	@echo "Checking root privileges..."
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


LOG_DIR = /var/log/kttp_log/
HTML_DIR = /var/kttp_server_files/html_docs/
SRC_DIR = /usr/lib/kttp_server_src/
CONF_DIR = /etc/kttp_server/CONFs/


create_kttp_log:
	@sudo mkdir -p "$(LOG_DIR)"|| { echo "Error: Unable to create directory '$(LOG_DIR)'."; exit 1; }


create_html_directory:
	@echo "Creating HTML directory..."
	@sudo mkdir -p "$(HTML_DIR)" || { echo "Error: Unable to create directory '$(HTML_DIR)'."; exit 1; }
	@echo "Directory '$(HTML_DIR)' created successfully."


create_src_directory:
	@echo "Creating source directory..."
	@sudo mkdir -p "$(SRC_DIR)" || { echo "Error: Unable to create directory '$(SRC_DIR)'."; exit 1; }
	@echo "Directory '$(SRC_DIR)' created successfully."

create_conf_directory:
	@echo "Creating configuration directory..."
	@sudo mkdir -p "$(CONF_DIR)" || { echo "Error: Unable to create directory '$(CONF_DIR)'."; exit 1; }
	@echo "Directory '$(CONF_DIR)' created successfully."


create_directories: create_kttp_log create_html_directory create_src_directory create_conf_directory
	@echo "All directories created successfully."


clean:
	@echo "Removing directories..."
	@sudo rm -rf /var/kttp_server_files/ /usr/lib/kttp_server_src/ /etc/kttp_server/ /var/log/kttp_log/ 
	@sudo rm /etc/systemd/system/kttp_server.service
	@echo "All directories removed."


moveFiles:
	@echo "Moving Files....\n"
	@mv "src/SERVER/" "/usr/lib/kttp_server_src/"
	@mv "src/LOG/" "/usr/lib/kttp_server_src/"
	@mv "src/CONF/userconf.ini" "/etc/kttp_server/CONFs/"
	@mv "src/CONF/" "/usr/lib/kttp_server_src/"
	@mv "html/index.html" "/var/kttp_server_files/html_docs/"
	@mv "html/404.html" "/var/kttp_server_files/html_docs/"
	@mv "html/ttt.html" "/var/kttp_server_files/html_docs/"
	@cp "Makefile" "/usr/lib/kttp_server_src/"


compile:
	@cd "/usr/lib/kttp_server_src/SERVER"; \
	sudo chmod +x "compila"; \
	sudo "./compila"

serviceSetup:
	@echo "Setting up the seervice..."
	@mv "service_setup/kttp_server.service" "/etc/systemd/system/"
	@sudo systemctl daemon-reload
	@sudo systemctl start kttp_server.service
	@sudo systemctl enable kttp_server.service

ip_address := $(shell curl -s https://ifconfig.me)

printInfo:
	@echo "\n\n\n\n\nThe service is up and running. Visit $(ip_address) to see the result!\n"
	@echo "===== PLEASE NOTE =====\n"
	@echo "The service is running as sudo on port 80. Read the documentation for all the info.\n\n\n\n\n"
	@echo "You can now delete the kttp_server folder\n"
	@echo "to configure. also, check run "installer.sh in $(CONF_DIR) to cusomize shit"

uninstall: check_root clean unistall_comand

unistall_comand:
	@echo "Uninstalling the kttp_server..."
	@sudo systemctl stop kttp_server
	@sudo systemctl daemon-reload
