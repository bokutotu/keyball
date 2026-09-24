#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "$0")"
nix build -L path:.
sudo -v

keyballs=()
for path in /sys/bus/usb/devices/*; do
    [[ -r "$path/idVendor" && -r "$path/idProduct" ]] || continue
    if [[ $(<"$path/idVendor") == 5957 && $(<"$path/idProduct") == 0200 ]]; then
        keyballs+=("$path")
    fi
done
if ((${#keyballs[@]} != 1)); then
    echo 'Keyball39を1台だけUSB接続してください。' >&2
    exit 1
fi
usb_path=$(readlink -e -- "${keyballs[0]}")

echo "Keyball39 (${usb_path##*/}) のRESETを素早く2回押してください。"
deadline=$((SECONDS + 60))
while ((SECONDS < deadline)); do
    ports=()
    for tty_path in /sys/class/tty/ttyACM*; do
        device=$(readlink -e -- "$tty_path/device" 2>/dev/null) || continue
        if [[ "${device%/*}" == "$usb_path" ]]; then
            ports+=("/dev/${tty_path##*/}")
        fi
    done
    if ((${#ports[@]} > 1)); then
        echo '書き込み用ポートが複数あるため中止します。' >&2
        exit 1
    fi
    if ((${#ports[@]} == 1)) && [[ -c "${ports[0]}" ]]; then
        exec sudo -n "$(command -v avrdude)" \
            -p atmega32u4 -c avr109 -b 57600 -P "${ports[0]}" \
            -U flash:w:result/keyball_keyball39_default.hex:i
    fi
    sleep 0.1
done
echo '書き込み用ポートが見つかりませんでした（60秒経過）。' >&2
exit 1
