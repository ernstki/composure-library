unpathify ()
{
    author 'Kevin Ernst'
    about 'Remove path separator from PATH-like variables'
    param '$1      - (optional) the PATH-like variable to unpathify (default $PATH)'
    param '-d $SEP - (optional) split on this delimiter (default: colon)'
    example 'unpathify'
    example 'unpathify $LD_LIBRARY_PATH'
    example 'head /etc/passwd | unpathify -d:'
    group 'utility'

    TRACE=${TRACE:-}
    [[ $TRACE = 1 ]] && set -x
    
    local delim=':'
    local var=PATH

    while (( $# )); do
        # handle the '-d' (delimiter) option
        case $1 in
            --d*)
                shift
                delim=$1
                break;;
            -d*)
                delim=${1#-d}
                if [[ "$delim" ]]; then
                    # Strip all leading/trailing whitespace
                    delim=${delim// /}
                else
                    shift
                    delim=$1
                fi
                break;;
            *)
                if [[ ! "${!1}" ]]; then
                    echo "ACK! Variable '$1' is empty" >&2
                    return 1
                fi
                var=$1
                ;;
        esac
        shift
    done

    if [[ ! "$delim" ]]; then
        echo "ACK! The '-d' option requires an option (the delimiter)!" >&2
        return 1
    fi
    # Just return an error if the delimiter isn't found
    if ! ( grep -q $delim <<<${!var} ); then
        echo "ACK! Delimiter '$delim' not found in variable '\$$var'" >&2
        return 1
    fi

    if [ -t 1 ]; then 
        tr $delim '\n' | sort
    else
        echo ${!var} | tr $delim '\n' | sort
    fi
}
# vim: ft=sh