split_bed_twice ()
{
    author 'Kevin Ernst'
    about 'Split a BED file in half and append a two-digit serial to the basename'
    param '$1 - (base)name of BED file'
    example ''
    group 'data'

    suffix=.bed

    if [ ! -f "$1" -o ! -f "${1}.bed" ]; then
        echo "ACK! File '$1' doesn't exist or isn't readable." >&2
        exit 1
    fi

    if [[ $1 =~ \.bed$ ]]; then
        basename=${1%.bed}
    fi

    split --suffix-length=2 \
          --number=2 \
          --numeric-suffixes \
          --additional-suffix=${suffix} \
          ${basename}.bed ${basename}-
}

# vim: ft=sh
