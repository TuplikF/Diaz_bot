#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD 256
#define MAX_LINE 512

void run_command(const char *cmd) {
    char buffer[MAX_LINE];
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        printf("Chyba: nelze spustit příkaz.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), pipe)) {
        printf("%s", buffer);
    }

    pclose(pipe);
}

void detect_pci() {
    printf("\n=== PCI zařízení ===\n");
    run_command("lspci -nn");
}

void detect_usb() {
    printf("\n=== USB zařízení ===\n");
    run_command("lsusb");
}

void suggest_drivers() {
    printf("\n=== Návrh ovladačů ===\n");

    // Jednoduchá ukázková logika
    FILE *fp = popen("lspci -nn", "r");
    if (!fp) return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "NVIDIA")) {
            printf("Nalezen NVIDIA GPU → doporučený ovladač: nvidia-driver\n");
        }
        if (strstr(line, "Intel Corporation Wireless")) {
            printf("Nalezen Intel WiFi → doporučený ovladač: iwlwifi (součást kernelu)\n");
        }
        if (strstr(line, "Realtek")) {
            printf("Nalezen Realtek síťový adaptér → doporučený ovladač: r8168 nebo r8169\n");
        }
    }

    pclose(fp);
}

int main() {
    printf("=== Automatická detekce hardwaru a návrh ovladačů ===\n");

    detect_pci();
    detect_usb();
    suggest_drivers();

    printf("\nHotovo.\n");
    return 0;
}