prefix () 
{ 
    author 'Kevin Ernst'
    about 'prefix input lines with an arbitrary string'
    param '-s [INT] - indent by INT spaces; def. 4 (can be "cuddled"; e.g., -s4)'
    param '$1 - (optional) string to prefix each input line; default "# "'
    param '$2 - (optional) input filename; defaults to stdin'
    example 'prefix "# " somefile.txt'
    example 'pbpaste | prefix -s4 | pbcopy'
    group 'textprocessing'

    local DEFAULT_PREFIX='# '
    local DEFAULT_SPACES=4

    local prefix=$DEFAULT_PREFIX
    local spaces=$DEFAULT_SPACES
    local file

    while (( $# )); do
        # recognize '-sINT' or '--spaces=INT' options
        if [[ $1 =~ ^--?s(paces)?=?([1-9][0-9]+)? ]]; then
            shift
            # if the integer argument is "cuddled"
            if [[ ${BASH_REMATCH[2]} ]]; then
                spaces=${BASH_REMATCH[2]}
            # if the next argument looks like an integer
            elif [[ $1 =~ ^[1-9][0-9]+$ ]]; then
                spaces=$1
                shift
            fi
        fi

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
        sed "s/\\(.*\\)/$prefix\\1/"
    fi
} # vim: ft=sh
