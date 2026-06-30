#!/usr/bin/bash

set -euo pipefail

OUT_DIR="$HOME/.local/share/SatelliteTracker"
STATIONS_FILE="$OUT_DIR/stations.json"
SATELLITES_FILE="$OUT_DIR/satellites.json"

mkdir -p "$OUT_DIR"

stations=5
satellites=100

coverage=20

min_passes=1
max_passes=3

rand_int() {
    local min=$1
    local max=$2
    echo $((RANDOM % (max - min + 1) + min))
}

time_to_seconds() {
    echo $((10#$1 * 3600 + 10#$2 * 60 + 10#$3))
}

seconds_to_time() {
    local s=$(( $1 % 86400 ))
    (( s < 0 )) && s=$((s + 86400))

    printf "%02d:%02d:%02d" \
        $((s / 3600)) \
        $(((s % 3600) / 60)) \
        $((s % 60))
}

generate_interval() {
    local start_sec duration end_sec start end h m s

    while :; do
        h=$(rand_int 0 23)
        m=$(rand_int 0 59)
        s=$(rand_int 0 59)

        start_sec=$(time_to_seconds "$h" "$m" "$s")
        duration=$(rand_int 3600 10800)
        end_sec=$((start_sec + duration))

        # строго в пределах одного дня
        if (( end_sec < 86400 )); then
            break
        fi
    done

    start=$(seconds_to_time "$start_sec")
    end=$(seconds_to_time "$end_sec")

    printf '{"start":"%s","end":"%s"}' "$start" "$end"
}

{
    printf "["

    for ((station=1; station<=stations; station++)); do
        ((station>1)) && printf ","

        printf '{"id":"Station%d","satellites":[' "$station"

        first_sat=true

        for ((sat=1; sat<=satellites; sat++)); do

            (( RANDOM % 100 >= coverage )) && continue

            $first_sat || printf ","
            first_sat=false

            printf '{"id":"SAT%03d","times":[' "$sat"

            passes=$(rand_int "$min_passes" "$max_passes")

            for ((p=1; p<=passes; p++)); do
                ((p>1)) && printf ","
                generate_interval
            done

            printf "]}"
        done

        printf "]}"
    done

    printf "]"
} > "$STATIONS_FILE"

{
    printf "["

    first=true

    while IFS= read -r sat; do
        $first || printf ","
        first=false

        printf '{"id":"%s","priority":%d}' \
            "$sat" \
            $((RANDOM % 11))

    done < <(
        jq -r '.[].satellites[].id' "$STATIONS_FILE" | sort -u
    )

    printf "]"
} > "$SATELLITES_FILE"

echo "Generated:"
echo "    $STATIONS_FILE"
echo "    $SATELLITES_FILE"
