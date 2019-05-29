spl () 
{ 
    author 'Kevin Ernst'
    about 'Splits input string or file (or stdin) on common delimiters'
    param '-d <delim>: delimiter list as string (defaults: [ :,\t])'
    example 'echo \$PATH | spl | sort | uniq -d'
    group 'util'

    local file=
    local string=
    local delim=' :,\t'
    local TRACE=${TRACE:-0}

    while (( $# )); do
        if [[ $1 =~ ^--?d* ]]; then
            shift
            delim=$1
        elif [ -f "$1" ]; then
            file=$1
        else
            string=$1
        fi
        shift
    done

    (( TRACE )) && set -x
    # if stdin is a terminal (not reading from a pipe)
    if [[ -t 0 ]]; then
        if [[ -n $string ]]; then
            tr "$delim" '\n' <<<"$string" | sed '/^[    ]*$/d'
        else
            <"$file" tr "$delim" '\n' | sed '/^[    ]*$/d'
        fi
    else
        tr "$delim" '\n' | sed '/^[    ]*$/d'
    fi
    set +x
}

# vim: ft=sh tw=78 sw=4 ts=4 expandtab
