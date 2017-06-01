configure_openssl ()
{
    author 'Kevin Ernst <ernstki -at- mail.uc.edu>'
    about 'Configures OpenSSL for $HOME/opt; also building .so version'
    param ''
    example ''
    group 'build'

    # It looks like Git wants the .so version available, at least with the
    # out-of-the-box setup, hence the 'shared' argument.
    if is_ppc_arch; then
        ./Configure --prefix=$HOME/opt shared linux-ppc64
    else
        ./config shared --prefix=$HOME/opt
    fi
}
