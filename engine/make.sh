#!/bin/bash

# sudo apt-get install build-essential
# sudo apt install poppler-utils ghostscript  python3-dateutil
# sudo apt install uncrustify

galaxy="alpha-32"
src="/home/mdhender/far-horizons-engine/engine"
game="/home/mdhender/${galaxy}"

mkdir -p "${game}" || exit 2

cp "${src}/example.csv" "${game}/players.csv" || exit 2

sed \
    -e "s;bindir:.*;bindir: ${src}/bin;" \
    -e "s;datadir:.*;datadir: ${game};" \
    "${src}/tools/farhorizons.yml.example" > "${game}/farhorizons.yml" || exit 2

cd "${game}" || exit 2

export FH_SEED=

python3 "${src}/tools/game_setup.py" < "${game}/players.csv"

python3 "${src}/tools/create_map.py"

python3 "${src}/tools/game_packet.py"

rm -f "/tmp/${galaxy}.sum" /tmp/beta/sum
cd "${game}" && "${src}/bin/BinaryToAscii"
cd "${game}" && sha1sum *.dat > "/tmp/${galaxy}.sum"
cd ~/beta && sha1sum *.dat > /tmp/beta.sum
diff "/tmp/${galaxy}.sum" /tmp/beta.sum || {
    echo "error: checksums do not match"
    exit 2
}
echo " info: checksums match"

exit 0
