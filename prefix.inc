# shellcheck disable=SC2016 shell=bash
prefix ()
{
    author 'Kevin Ernst'
    about 'prefix input lines with an arbitrary string'
    param '-s [INT] - indent by INT spaces (default=4; can "cuddle"; e.g., -s4)'
    param '$1 - (optional) string to prefix each input line; default "# "'
    param '$2 - (optional) input filename; defaults to stdin'
    example 'prefix "# " somefile.txt'
    example 'pbpaste | prefix -s4 | pbcopy'
    group 'textprocessing'

    local DEFAULT_PREFIX='# '
    local DEFAULT_SPACES=4
    local RE_INTEGER='[1-9][0-9]*'
    local RE_DASH_S='--?s(paces)?(=)?'

    local prefix=$DEFAULT_PREFIX
    local spaces=$DEFAULT_SPACES
    local indent_with_spaces=
    local file

    while (( $# )); do
        # recognize '-s [INT]', '-sINT', '--spaces [INT]', or '--spaces=INT'
        if [[ $1 =~ ^$RE_DASH_S($RE_INTEGER)?$ ]]; then
            indent_with_spaces=1
            shift

            # if the integer argument is "cuddled" (BASH_REMATCH[2] is '=')
            if [[ ${BASH_REMATCH[3]} ]]; then
                spaces=${BASH_REMATCH[3]}
                continue
            fi

            # if the next argument looks like an integer
            if [[ $1 =~ ^$RE_INTEGER$ ]]; then
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

    # build up prefix string if '-s' / '--spaces' option given
    if (( indent_with_spaces )); then
        for (( i = 0; i < spaces; i++ )); do
            prefix+=' '
        done
    fi

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
