#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'EOF'
Usage:
  can_rtr_test.sh [interface] test
  can_rtr_test.sh [interface] load [seconds] [gap_ms]

Examples:
  can_rtr_test.sh can0 test          # Request ADC pairs 0x100-0x103 once
  can_rtr_test.sh can0 load 10 1     # Request each ID 0x100-0x103 every 1 ms
  can_rtr_test.sh can0 load 10 0     # Send as fast as possible for 10 seconds

Requires can-utils and an already configured CAN interface (250 kbit/s).
All requests use DLC 0; STM32 replies with DLC 8.
Load mode runs four generators; gap_ms is the gap per ID.
In another terminal, watch requests and replies with:
  candump can0,100:7FC
EOF
}

if [[ ${1:-} == -h || ${1:-} == --help ]]; then
    usage
    exit 0
fi

interface=${1:-can0}
mode=${2:-test}
duration=${3:-10}
gap_ms=${4:-1}

require_command() {
    command -v "$1" >/dev/null || {
        echo "Missing command: $1" >&2
        exit 1
    }
}

case "$mode" in
    test)
        [[ $# -le 2 ]] || { usage >&2; exit 1; }
        require_command cansend
        for id in 100 101 102 103; do
            echo "Requesting 0x$id on $interface (RTR, DLC 0)"
            cansend "$interface" "${id}#R0"
            sleep 0.1
        done
        echo "Requests sent. Check candump for eight-byte data replies."
        ;;
    load)
        [[ $# -le 4 && $duration =~ ^[0-9]+$ && $duration =~ [1-9] &&
           $gap_ms =~ ^[0-9]+([.][0-9]+)?$ ]] || {
            usage >&2
            exit 1
        }
        require_command cangen
        require_command timeout
        echo "Loading $interface: RTR 0x100-0x103, DLC 0, gap ${gap_ms} ms per ID, ${duration} seconds"
        generator_pids=()
        cleanup() {
            if [[ ${#generator_pids[@]} -gt 0 ]]; then
                kill "${generator_pids[@]}" 2>/dev/null || true
                wait "${generator_pids[@]}" 2>/dev/null || true
            fi
        }
        trap cleanup EXIT
        trap 'exit 130' INT
        trap 'exit 143' TERM
        # Poll for writable space when the socket's transmit queue fills.
        # timeout returns 124 when the requested test duration ends.
        for id in 100 101 102 103; do
            timeout --kill-after=2s "${duration}s" \
                cangen "$interface" -R -I "$id" -L 0 -g "$gap_ms" -p 10 &
            generator_pids+=("$!")
        done
        for pid in "${generator_pids[@]}"; do
            status=0
            wait "$pid" || status=$?
            if [[ $status -ne 0 && $status -ne 124 ]]; then
                exit "$status"
            fi
        done
        generator_pids=()
        echo "Load test finished."
        ;;
    *)
        usage >&2
        exit 1
        ;;
esac
