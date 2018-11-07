prefix () 
{ 
    author 'Kevin Ernst'
    about 'prefix input lines with an arbitrary string'
    param '$1 - (optional) string to prefix each input line; default "# "'
    param '$2 - (optional) input filename; defaults to stdin'
    example 'prefix "# " somefile.txt'
    group 'textprocessing'

    local DEFAULT_PREFIX='# '
    local prefix=$DEFAULT_PREFIX
    local file

    while (( $# )); do
        # if the first argument is a filename, then store it off
        if [[ -r $1 ]]; then
            file=$1
        else
            prefix=$1
        fi
        shift
    done

    # If stdin is a terminal (not a pipe), operate on filename argument
    if [ -t 0 ]; then
        if [[ -z $file ]]; then
            echo "ACK! Need an input file." >&2
            return 1
        elif [[ ! -r $file ]]; then
            echo "ACK! Input file '$file doesn't exist or isn't readable." >&2
            return 1
        fi
        sed "s/\\(.*\\)/$prefix\\1/" "$file"
    else
        echo "UNIMPLEMENTED" >&2
        return 1
    fi
} # vim: ft=sh
