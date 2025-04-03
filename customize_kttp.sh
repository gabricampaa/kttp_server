#!/bin/bash
#to cos
CONFIG_FILE="settings.conf"

# Funzione per acquisire il numero di porta
get_port() {
    PORT=$(dialog --inputbox "Inserisci la porta del server:" 8 40 "80" 2>&1 >/dev/tty)
}

# Funzione per acquisire il percorso della cartella HTML
get_path() {
    PATH=$(dialog --inputbox "Inserisci il percorso della cartella HTML:" 8 60 "/var/kttp_server_files/html_docs/" 2>&1 >/dev/tty)
}

# Funzione per salvare il file di configurazione
save_config() {
    echo "[GENERAL_SETTINGS]" > "$CONFIG_FILE"
    echo "PORT = $PORT" >> "$CONFIG_FILE"
    echo "PATH = $PATH" >> "$CONFIG_FILE"
}

# Mostra un messaggio di benvenuto
dialog --title "Setup KTTP_SERVER" --msgbox "Welcome to the setup page of KTTP_SERVER!" 8 40

# Avvia la configurazione
get_port
get_path
save_config

# Mostra il messaggio di completamento
dialog --title "Completato" --msgbox "Configurazione salvata con successo in $CONFIG_FILE!" 8 50

clear
echo "Configurazione completata!"
cat "$CONFIG_FILE"

