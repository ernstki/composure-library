gpg_fetch_key ()
{
    KEYSERVER=${KEYSERVER:-hkp://p80.pool.sks-keyservers.net:80}

    author 'Kevin Ernst'
    about 'fetches a GPG keyring from $KEYSERVER and adds to the default keyring'
    param '$1: the 8-hex-digit GPG key'
    example '$ gpg_fetch_key 88888888'
    group 'gpg'

    if printf "$1" | grep -qE '^[[:xdigit:]]{8}$'; then
        gpg -v --keyserver $KEYSERVER --recv-keys "0x$1"
    else
        echo "ERROR: '$1' does not appear to be a valid GPG key" >&2
        return 1
    fi
}
