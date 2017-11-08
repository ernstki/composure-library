doesnthave ()
{
    author 'Kevin Ernst'
    about 'Report directories /not/ containing a named (non-empty) file'
    param '$1              - filename (or directory) to search for'
    param "--empty-ok      - (optional) don't report if \$1 exists but is empty"
    param '--has-empty     - (optional) report only if $1 exists and is empty'
    param '--has-non-empty - (optional) report only if $1 exists and is NON-empty'
    param '--directory     - (optional) report only if directory $1 is missing'
    param '--has-directory - (optional) report only if directory $1 is PRESENT'
    param '<dir> ...       - (optional) search within dirs (default: c.w.d. and subdirs)'
    example '$ doesnthave snpcoords.bed bf504a8af2 ba7a23c3bd'
    example '$ doesnthave --has-empty allresults.bed'
    example '$ doesnthave --empty-ok snp_coords.bed'
    example '$ doesnthave --directory bsub_logs'
    group 'lsf'

    # set -x
    local DEBUG=${DEBUG:+-v}
    # skip over (don't print) if this directory passes this test
    local SKIP_IF=
    # by default, consider empty files as "missing"
    local REPORT_EMPTY_OK=
    # report *only* for existing empty files
    local REPORT_ONLY_EMPTY=
    # report *only* for existing non-empty files
    local REPORT_ONLY_NONEMPTY=
    # report if directory $1 doesn't exist
    local REPORT_DIR_MISSING=
    # report if directory DOES exist
    local REPORT_DIR_PRESENT=
    local numflags=0
    local what=
    local dirs=()

    while (( $# )); do
        if [[ $1 =~ ^(-h|--?help) ]]; then
            reference doesnthave
            return
        elif [[ $1 =~ ^--?(allow-empty|empty-ok) ]]; then
            REPORT_EMPTY_OK=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?(only-|is-|has-)empty ]]; then
            REPORT_ONLY_EMPTY=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?(only-|is-|has-)non-?empty ]]; then
            REPORT_ONLY_NONEMPTY=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?dir ]]; then
            REPORT_DIR_MISSING=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--?(has-)dir ]]; then
            REPORT_DIR_PRESENT=1
            numflags=$(( numflags + 1 ))
        elif [[ $1 =~ ^--? ]]; then
                echo "ACK! Unrecognized option '$1'." >&2 && \
                    return 1
        elif [[ -d "$1" ]]; then
            test -n "$DEBUG" && echo "dir='$1'"
            # Trim leading './'
            dirs=( "${dirs[@]}" "${1#./}" )
        else
            # consider this a filename
            if [[ $what ]]; then
                echo "ACK! Multiple search targets not (yet) supported." >&2 && \
                    return 1
            fi
            what=$1
        fi
        shift
    done

    test -z "$what" && \
        echo -e "ACK! You must at *least* specify something to search for.\n" >&2 && \
        return 1

    # error out on mutually-exclusive options
    test $numflags -gt 1 && \
        echo -e "ACK! Search options ('--directory', '--empty-ok', etc.) are" \
                "mutually-exclusive.\n" >&2 && return 1

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

    # The default behavior is "skip this directory if the file exists and it's
    # non-empty"
    SKIP_IF="test -s '$what'"

    if [[ $REPORT_ONLY_EMPTY ]]; then
        # skip if the file isn't there, or if it's non-zero size (only print if
        # the file exists *and* is zero size)
        SKIP_IF="test ! -f '$what' -o -s '$what'"
    elif [[ $REPORT_ONLY_NONEMPTY ]]; then
        # reverse the logic of the above (report only directories where the
        # file exists *and* has non-zero size)
        SKIP_IF="test ! -f '$what' -o ! -s '$what'"
    elif [[ $REPORT_EMPTY_OK ]]; then
        # skip this directory if the file even exists (fine if it's empty)
        SKIP_IF="test -f '$what'"
    elif [[ $REPORT_DIR_MISSING ]]; then
        # skip this directory if the directory $ even exists (fine if it's empty)
        SKIP_IF="test -d '$what'"
    elif [[ $REPORT_DIR_PRESENT ]]; then
        # skip this directory if the file even exists (fine if it's empty)
        SKIP_IF="test ! -d '$what'"
    fi

    # Do it
    parallel $DEBUG cd {} \; $SKIP_IF '||' echo {} ::: "${dirs[@]}"
    # set +x
}

# vim: ft=sh
