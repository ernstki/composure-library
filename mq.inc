mq ()
{
    author 'Kevin Ernst';
    about 'Query appropriate dev/prod MySQL server (using credentials in ~/.my.cnf)';
    param "$1 (optional)  my.cnf group suffix (database host); default 'bmirsch{p|d}04my' "
    param "$2 (optional)  database name; default 'bedintersect' ";
    param "$3 (optional)  options to 'mysql', e.g., '--skip-column-names' ";
    param "$4             quoted string of SQL commands to execute";
    param "-q | --quiet   raw output: no borders, no extraneous messages";
    param "-h | --help    print this help message";
    example "$ mq  # default query is 'show tables' ";
    example "$ mq 'select * from gene_aliases' ";
    example "$ mq --html localhost cressint 'select * from gene_aliases' ";
    example "$ mq -q --column-names 'select * from gene_aliases limit 10' ";
    group 'dba';

    quiet=
    mysqlhost=
    db=bedintersect
    sql='show tables;'
    # Default arguments to pass to 'mysql'; the '--table' option is removed and
    # '--skip-column-names' added below if you specify '-q' | '--quiet'. You
    # can restore the columns even with '-q' by passing '--column-names' if you
    # want.
    mysqlargs=(
        '--no-auto-rehash'
        '--table'
    )

    # Default to the BMI MySQL server on the same side of the research bubble
    # as the host you're logged in to
    if [[ $(hostname -s) =~ p[0-9]$ ]]; then
        mysqlhost=bmirschp04my.chmcres.cchmc.org
    elif [[ $(hostname -s) =~ d[0-9]$ ]]; then
        mysqlhost=bmirschd04my.chmcres.cchmc.org
    else
        mysqlhost=localhost
    fi

    while (( $# )); do
        # If the argument looks like a hostname...
        if [[ $1 =~ ^bmi || $1 = "localhost" ]]; then
            mysqlhost=$1
            shift; continue
        fi

        # ...or else if it's one of the "defaults group" suffixes in ~/.my.cnf
        if [[ -f ~/.my.cnf ]]; then
            local -a suffixes
            # split on '[', ']', and ':' and print only if there is a $3
            suffixes=($(awk -F'[]:[]'  '/^\[/{if(length($3)>0){print $3}}' \
                        ~/.my.cnf | sort -u))
            for suffix in "${suffixes[@]}"; do
                if [[ $1 = $suffix ]]; then
                    mysqlhost=$1
                    shift; continue 2
                fi
            done
        fi

        # If the argument looks like a command-line option...
        if [[ $1 =~ ^- ]]; then
            if [[ $1 =~ ^--?q ]]; then
                # suppress output to stderr
                quiet='ssssssshhhhhh'
                # h/t: https://stackoverflow.com/a/16861932
                mysqlargs=( ${mysqlargs[*]/'--table'} '--skip-column-names' )
            elif [[ $1 =~ ^--?h ]]; then
                reference mq
                return
            else
                # FIXME: quoted options almost definitely not preserved here
                mysqlargs=( ${mysqlargs[*]} $1 )
            fi
            shift; continue
        fi
            
        # If the argument looks like a database name...
        if [[ ! $1 =~ [[:space:]] ]]; then
            db=$1
            shift; continue
        fi

        # automatically append a semicolon to SQL if missing
        sql=$1
        [[ $sql =~ \;$ ]] || sql="${sql};"
        shift
    done

    shorthost=${mysqlhost%%.*}
    if [ -z "$quiet" ];  then
        echo >&2
        echo "Running SQL query [db: $db, group suffix (host): ${shorthost}]" >&2
        echo >&2
        echo "  $( fold -sw72 <<<"$sql" | sed 's/\(.*\)/>  \1/' )" >&2
        echo >&2
    fi

    # If a group exists like '[client:hostname]' in ~/.my.cnf, then use all the
    # parameters defined there, including hostname. Refer to this section of
    # the MySQL manual for details:
    # https://dev.mysql.com/doc/refman/5.7/en/option-file-options.html
    if grep -qE "^[[:space:]]*\[client:${shorthost}" ~/.my.cnf; then
        mysqlcmd="mysql --defaults-group-suffix=:$shorthost ${mysqlargs[*]} $db"
    else
        echo "WARNING: No group for '$shorthost' found in ~/.my.cnf" >&2
        echo "         Will use default credentials from '[client]' section" >&2
        mysqlcmd="mysql -h $mysqlhost ${mysqlargs[*]} $db"
    fi

    # Actually do it
    if [ -z "$quiet" ]; then
        (set -x; $mysqlcmd <<<"$sql")
        echo >&2
    else
        $mysqlcmd <<<"$sql"
    fi
}

# vim: ft=sh
