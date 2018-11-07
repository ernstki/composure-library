prefix () 
{ 
    author 'Kevin Ernst'
    about 'prefix input lines with an arbitrary string'
    param '$1 - string to prefix each input line with'
    param '$2 - (optional) input filename; defaults to stdin'
    example 'prefix "# " somefile.txt'
    group 'textprocessing'

    local prefix=$1

    # If stdin is a terminal (not a pipe), then treat $2 as a file
    if [ -t 0 ]; then
        local file=$2
        if ! test -r "$2"; then
            echo "ACK! Input file '$2' doesn't exist or isn't readable." >&2
            return 1
        fi
        sed "s/\\(.*\\)/$prefix\\1/" "$2"
    else
        echo "UNIMPLEMENTED" >&2
        return 1
    fi
} # vim: ft=sh
