doesnthave ()
{
    author 'Kevin Ernst'
    about 'Find directories /not/ containing a named (non-empty) file'
    param '$1           - filename to search for'
    param "--empty-ok   - (optional) if file is empty, don't consider it 'missing' "
    param '--only-empty - (optional) report if file exists but is empty'
    param '<dir> ...    - (optional) search within dirs (default: c.w.d. and subdirs)'
    example '$ doesnthave --is-empty allresults.bed'
    example '$ doesnthave --empty-ok snp_coords.bed'
    example '$ doesnthave snpcoords.bed bf504a8af2 ba7a23c3bd'
    group 'lsf'

    # set -x
    local DEBUG=${DEBUG:+-v}
    local TEST_EXISTS=
    # by default, consider empty files as "missing"
    local EMPTY_OK=
    # report *only* existing empty files
    local REPORT_ONLY_EMPTY=
    local file=
    local dirs=()

    while (( $# )); do
        if [[ $1 =~ ^--?(allow-empty|empty-ok) ]]; then
            EMPTY_OK=1
        elif [[ $1 =~ ^--?(only-|is-)?empty ]]; then
            REPORT_ONLY_EMPTY=1
        elif [[ $1 =~ ^--? ]]; then
                echo "ACK! Unrecognized option '$1'." >&2 && \
                    return 1
        elif [[ -d "$1" ]]; then
            test -n "$DEBUG" && echo "dir='$1'"
            # Trim leading './'
            dirs=( "${dirs[@]}" "${1#./}" )
        else
            # consider this a filename
            if [[ $file ]]; then
                echo "ACK! Multiple filenames not (yet) supported." >&2 && \
                    return 1
            fi
            file=$1
        fi
        shift
    done

    test -z "$file" && \
        echo -e "ACK! You must at *least* specify a filename to search for.\n" >&2 && \
        return 1

    # If *no* directories given on command line; add everything in the c.w.d.
    if [[ -z "${dirs[*]}" ]]; then
        # Ref: https://stackoverflow.com/a/8677566
        while IFS= read -r -d '' dir; do
            test -n "$DEBUG" && echo "dir='$dir'"
            dirs=( "${dirs[@]}" "$dir" )
        done < <(find * -maxdepth 0 -type d -print0)

        # This /almost/ worked, but doesn't preserve embedded spaces in dir names
        #dirs=( $(find * -maxdepth 0 -type d) )
    fi

    # Still nothing (no subdirectories)? Bail out.
    test -z "${dirs[*]}" && \
        echo -e "ACK! Invalid/empty list of directories.\n" >&2 && \
        return 1

    # The default behavior is "does the file exist and is it non-empty?"
    TEST_EXISTS="test -s '$file'"

    if [[ $REPORT_ONLY_EMPTY && $EMPTY_OK ]]; then
        echo -e "ACK! The '--empty-ok' and '--only-empty' options " \
                "are mutually-exclusive.\n" && return 1
    elif [[ $REPORT_ONLY_EMPTY ]]; then
        # file exists and it's *not* non-zero size (i.e., it's empty)
        TEST_EXISTS="test -f '$file' -a ! -s '$file'"
    elif [[ $EMPTY_OK ]]; then
        # file exists, don't care if it's empty
        TEST_EXISTS="test -f '$file'"
    fi

    # Do it
    parallel $DEBUG cd {} \; $TEST_EXISTS '||' echo {} ::: "${dirs[@]}"
    # set +x
}

# vim: ft=sh
