myq ()
{
    author 'Kevin Ernst'
    about 'Query propriate dev/prod MySQL server (using credentials in ~/.my.cnf)'
    param "\$1 - (optional) database host; default 'bmirsch{p|d}04my'"
    param "\$2 - (optional) database name; default 'bedintersect'"
    param "\$3 - (optional) options to 'mysql', e.g., '--skip-column-names'"
    param "\$4 - quoted string of SQL commands to execute"
    example "mq bedintersect 'select * from gene_aliases'"
    example "mq -H localhost cressint 'select * from gene_aliases'"
    group 'dba'
    group 'mysql'

    quiet=
    mysqlhost=
    db=bedintersect
    sql='show tables;'
    # Default arguments to pass to 'mysql'; you can turn column names back on
    # with '--column-names' if you want. The '--table' option is removed below
    # if you specify '-q' | '--quiet'.
    mysqlargs=(
        '--no-auto-rehash'
        '--skip-column-names'
        '--table'
    )

    # Default to the BMI MySQL server on the same side of the research bubble
    # as the host you're logged in to
    if [[ $(hostname -s) =~ p[0-9]$ ]]; then
        mysqlhost=bmirschp04my.chmcres.cchmc.org
    elif [[ $(hostname -s) =~ d[0-9]$ ]]; then
        mysqlhost=bmirschd04my.chmcres.cchmc.org
    fi

    while (( $# )); do
        # If the argument looks like a hostname...
        if [[ $1 =~ ^bmi || $1 = "localhost" ]]; then
            mysqlhost=$1
            shift; continue
        fi

        # If the argument looks like a command-line option...
        if [[ $1 =~ ^- ]]; then
            if [[ $1 =~ ^--?q ]]; then
                # suppress output to stderr
                quiet='ssssssshhhhhh'
                mysqlargs=( ${mysqlargs[*]/'--table'} )
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
        echo "Executing the following SQL on ${shorthost}:" >&2
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
