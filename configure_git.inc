configure_git ()
{
    author 'Kevin Ernst <ernstki -at - mail.uc.edu>'
    about 'Build Git from source using locally-installed curl and OpenSSL libraries'
    param ''
    example ''
    group 'build'

    LDFLAGS="-L$HOME/opt/lib -Wl,-R$HOME/opt/lib"
    CPPFLAGS="-I$HOME/opt/include"
    ./configure --with-openssl --with-curl --prefix=$HOME/opt
}
