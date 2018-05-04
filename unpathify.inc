unpathify ()
{
    author 'Kevin Ernst'
    about 'Translate path separators of PATH-like variables into newlines'
    param '-d <SEP> - (optional) split on this delimiter (default: colon)'
    param '<ENVVAR> - (optional) the PATH-like variable to unpathify, omit "$" (default PATH)'
    example 'unpathify'
    example 'unpathify LD_LIBRARY_PATH'
    example 'head -1 /etc/passwd | unpathify'
    group 'utility'

    # remember 'set +/-x' and restore it when we're through
    SETXTRACE=$( set -o | grep xtrace | awk '{print $2}' )
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

    # If stdin is a terminal, then...
    if [ -t 0 ]; then 
        # Just return an error if the delimiter isn't found
        if ! ( grep -q $delim <<<${!var} ); then
            echo "ACK! Delimiter '$delim' not found in variable '\$$var'" >&2
            return 1
        fi
        echo "${!var}" | tr "$delim" '\n' | sort
    else
        local tmp=$( mktemp -p unpathify-XXXXXX )
        trap "rm -f $tmp" 0 1 15
        trap "echo 'Interrupt received, aborting' >&2;rm -f $tmp;exit 1" 2

        cat > $tmp
        if ! ( grep -q $delim $tmp ); then
            echo "ACK! Delimiter '$delim' not found in input stream" >&2
            return 1
        fi
        <$tmp tr "$delim" '\n' | sort
    fi

    # restore the previous 'xtrace' shell option
    [[ $SETXTRACE = 'on' ]] && set -x || set +x
}
# vim: ft=sh
